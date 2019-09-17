#ifndef VC_camera_H
#define VC_camera_H

#include <math.h>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>

#include "vectormath.h"

#include <math.h>

namespace ogllib
{

enum camera_behavior
{
	FirstPerson,
	Flight
};

class camera
{
private:
	const float PI_DIV_180 = 0.01745329251f;

	float _fovY = 45.0f;
	float _aspectRatio = 1.0f;
	float _zNear = 0.10f;
	float _zFar = 1000.0f;

	camera_behavior _behavior = FirstPerson;

	glm::vec3 _eyeLocation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 _forward = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 _right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 _lookat;

	float _pitch, _yaw, _roll;

	glm::mat4 _view = glm::mat4();
	glm::mat4 _projection;

	void orthogonalize_uvn();
	void set_position(const glm::vec3 &pos);
	inline const void reset_projection();

public:
	camera();
	~camera();

	void translate(glm::vec3 disp);
	void move_forward(float units);
	void strafeRight(float units);

	void rotate(glm::quat rotation);
	void rotate(float theta, glm::vec3 &axis);

	void pitch(float theta);
	void yaw(float theta);
	void roll(float theta);
	void rotate(float a_yaw_deg, float a_pitch_deg, float a_roll_deg);
	void rotate_first_person(float a_heading_deg, float a_pitch_deg);
	void rotate_flight(float a_heading_deg, float a_pitch_deg, float a_roll_deg);

	//setters
	void setOrientation(const glm::quat &orientation);
	void setAspectRatio(float aspect_ration);
	void setFovY(float fovy);
	void setZPlanes(float near, float zfar);
	void setBehavior(camera_behavior behavior) { _behavior = behavior; }

	void createViewMat(bool reorthognolaze);
	void apply();

	glm::mat4 &getViewMatrix() { return _view; }
	glm::mat4 &getProjectionMatrix() { return _projection; }
};

} // namespace ogllib

#endif
