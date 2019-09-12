#ifndef __VBO_H_
#define __VBO_H_

#include <cstdint>
#include <glad/glad.h>

namespace buffers
{
class vbo
{
public:
    void generate();

private:
    std::uint32_t _id;
};
} // namespace buffers

#endif
