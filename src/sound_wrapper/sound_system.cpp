//
// Created by sava on 11/30/19.
//

#include <sound_wrapper/sound_system.hpp>

#include <string>
#include <fmod_errors.h>
#include <spdlog/spdlog.h>
#include <sound_wrapper/sound_emitter_component.hpp>

#include <events/event_type.hpp>
#include <sound_wrapper/sound_listener_component.hpp>

bool succeededOrWarn(const std::string &message, FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        spdlog::error("{0}: {1} {3}", message, result, FMOD_ErrorString(result));
        return false;
    }
    return true;
}

FMOD_RESULT F_CALLBACK channelGroupCallback(FMOD_CHANNELCONTROL *channelControl,
                                            FMOD_CHANNELCONTROL_TYPE controlType,
                                            FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType,
                                            void *commandData1,
                                            void *commandData2)
{
    // too noisy spdlog::info("FMod: channel control {0}, control type {1}");
    return FMOD_OK;
}

sound::sound_system::sound_system(string_table& strings,events::event_exchange& events)
    : _app_strings(strings)
    , _events(events)
{
    FMOD_RESULT result;

    // Create the main system object.
    result = FMOD::System_Create(&_system);
    if (!succeededOrWarn("FMOD: Failed to create system object", result))
        throw;

    // Initialize FMOD.
    result = _system->init(512, FMOD_INIT_NORMAL, nullptr);
    if (!succeededOrWarn("FMOD: Failed to initialise system object", result))
        throw;

    // Create the channel group.
    result = _system->createChannelGroup("inGameSoundEffects", &_channelGroup);
    if (!succeededOrWarn("FMOD: Failed to create in-game sound_wrapper effects channel group", result))
        throw;


    _system->set3DSettings(1.f, 40.f, 1.f);

    auto grab_cam_fn = std::function([this](ecs::entity &e) { grab_entity(e); });
    auto forget_cam_fn = std::function([this](ecs::entity &e) { forget_entity(e); });
    _entity_create_listener_id = _events.subscribe<ecs::entity &>(events::entity_created, grab_cam_fn);
    _entity_destroy_listener_id = _events.subscribe(events::entity_destroyed, forget_cam_fn);
}


sound::sound_system::~sound_system()
{
    spdlog::info("Releasing FMOD items");

    for (auto& pair : _sounds)
        pair.second->release();

    _channelGroup->release();
    _system->release();

    _events.unsubscribe(events::entity_created, _entity_create_listener_id);
    _events.unsubscribe(events::entity_destroyed, _entity_destroy_listener_id);
}


sound::playback_id sound::sound_system::play_sound_3d(size_t path_hash, bool loop)
{
    auto mode = FMOD_3D;
    if (loop) mode |= FMOD_LOOP_NORMAL;
    auto sound = get_sound(path_hash, mode);
    return play_sound(sound, path_hash);
}


sound::playback_id sound::sound_system::play_sound_one_off(size_t path_hash, bool loop)
{
    auto mode = FMOD_DEFAULT;
    if (loop) mode |= FMOD_LOOP_NORMAL;
    auto sound = get_sound(path_hash, mode);
    return play_sound(sound, path_hash);
}

sound::playback_id sound::sound_system::play_sound(FMOD::Sound *sound, size_t path_hash)
{
    FMOD_RESULT result;

    // Play the sound_wrapper.
    FMOD::Channel *channel = nullptr;
    result = _system->playSound(sound, nullptr, false, &channel);
    if (!succeededOrWarn("FMOD: Failed to play sound_wrapper", result))
        throw;

    channel->setVolume(1.0f);

    // Assign the channel to the group.
    result = channel->setChannelGroup(_channelGroup);
    if (!succeededOrWarn("FMOD: Failed to set channel group on", result))
        throw;

    auto playback_id = _next_playback_id++;
    _playbacks[playback_id] = std::pair(sound, channel);
    _sound_to_playback[path_hash] = playback_id;

    channel->setCallback(&channelGroupCallback);
    if (!succeededOrWarn("FMOD: Failed to set callback for sound_wrapper", result))
        throw;

    return playback_id;
}


FMOD::Sound *sound::sound_system::get_sound(size_t hash, FMOD_MODE mode)
{
    if (_sounds.find(hash) == _sounds.end())
    {
        FMOD::Sound *sound = nullptr;
        auto& track = _app_strings[hash];
        _system->createSound(track.c_str(), mode, nullptr, &sound);
        _sounds.insert({hash, sound});
    }

    return _sounds.find(hash)->second;
}


void sound::sound_system::stop_sound(playback_id playback)
{
    if (_playbacks.find(playback) == _playbacks.end()) return;

    auto& sound_channel = _playbacks.find(playback)->second;
    sound_channel.second->stop();
    _playbacks.erase(playback);
}

void sound::sound_system::update()
{
    sync_transform_to_listener();
    update_emitters();

    _system->update();
}


void sound::sound_system::grab_entity(ecs::entity &e)
{
    if (e.has<sound::sound_emitter_component>())
        _emitters.emplace_back(e);

    if (e.has<sound::sound_listener_component>())
        _listeners.emplace_back(e);
}

void sound::sound_system::forget_entity(ecs::entity &e)
{
    auto it = std::find_if(
        _emitters.begin(), _emitters.end(),
        [&e](ecs::entity &x) { return x.id() == e.id(); });
    if (it != _emitters.end()) _emitters.erase(it);

    auto it_l = std::find_if(
        _listeners.begin(), _listeners.end(),
        [&e](ecs::entity &x) { return x.id() == e.id(); });
    if (it_l != _listeners.end()) _listeners.erase(it_l);
}

void sound::sound_system::sync_transform_to_listener()
{
    for (int i = 0; i < _listeners.size(); ++i)
    {
        auto& listener = _listeners[i].get();
        auto &t = listener.get_component<ecs::transform_component>();
        auto &l = listener.get_component<sound::sound_listener_component>();
        auto rb_opt = listener.get_component_opt<ecs::rigid_body_component>();
        auto cam_opt = listener.get_component_opt<ecs::camera_component>();

        glm::mat3 rot = cam_opt ? cam_opt->get().right_up_fwd() : glm::mat3(t.to_mat4());

        l.listener_position = t.pos;
        l.listener_velocity = rb_opt ? rb_opt->get().velocity : glm::vec3(0.f);
        l.listener_up = rot[1];
        l.listener_forward = -rot[2];

        FMOD_VECTOR pos = {l.listener_position.x, l.listener_position.y, l.listener_position.z};
        FMOD_VECTOR vel = {l.listener_velocity.x, l.listener_velocity.y, l.listener_velocity.z};
        FMOD_VECTOR fwd = {l.listener_forward.x, l.listener_forward.y, l.listener_forward.z};
        FMOD_VECTOR up = {l.listener_up.x, l.listener_up.y, l.listener_up.z};

        _system->set3DListenerAttributes(i, &pos, &vel, &fwd, &up);
    }
}

void sound::sound_system::update_emitters()
{
    for(auto& emitter : _emitters)
    {
        if (!emitter.get().has<ecs::transform_component>()) continue;

        auto t = emitter.get().graph_node->absolute_transform();
        auto t_pos = t[3];
        auto& e = emitter.get().get_component<sound::sound_emitter_component>();
        auto rb_opt = emitter.get().get_component_opt<ecs::rigid_body_component>();

        // TODO: Refactor
        for (std::uint32_t i = 0; i < e.sound_count; ++i)
        {
            auto& emitter_sound = e.emitter_sounds[i];

            switch (emitter_sound.state)
            {
                case sound_state::stopped:
                {
                    stop_sound(emitter_sound.playback);
                    emitter_sound.playback = 0;
                    break;
                }
                case sound_state::playing:
                    emitter_sound.playback = play_sound_3d(emitter_sound.path_hash, emitter_sound.loop);
                default:
                {
                    if (_playbacks.find(emitter_sound.playback) == _playbacks.end()) break;

                    auto& sound_channel =_playbacks.find(emitter_sound.playback)->second;
                    FMOD_VECTOR pos = { t_pos.x, t_pos.y, t_pos.z };
                    FMOD_VECTOR vel = rb_opt
                                      ? FMOD_VECTOR { rb_opt->get().velocity.x, rb_opt->get().velocity.y, rb_opt->get().velocity.z }
                                      : FMOD_VECTOR { 0.f, 0.f, 0.f };

                    sound_channel.second->set3DAttributes(&pos, &vel);

                    sound_channel.second->setVolume(emitter_sound.volume);

//                    if (emitter_sound.loop)
//                    {
//                        FMOD_MODE mode;
//                        sound_channel.second->getMode(&mode);
//                        sound_channel.second->setMode(mode | FMOD_LOOP_NORMAL);
//                    }
                }
            }

            emitter_sound.state = sound_state::unchanged;
        }
    }
}
