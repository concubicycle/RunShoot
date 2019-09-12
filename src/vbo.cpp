#include "renderer/vbo.hpp"

void buffers::vbo::generate()
{
    glGenBuffers(1, &_id);
}
