//
// Created by sava on 10/16/19.
//

#include "shader_program_uniform_assign.h"



// for each OpenGL type, like GLuint, GLenum, GLfloat...etc,
// implement this method, and tell the program how to set that type
// of uniform, given the value "val"
template <>
void ogllib::AssignmentProxy<GLfloat>::operator=(GLfloat &val)
{
    glUniform1f(_location, val);
}

template <>
void ogllib::AssignmentProxy<glm::mat4>::operator=(glm::mat4 &val)
{
    glUniformMatrix4fv(_location, 1, GL_FALSE, glm::value_ptr(val));
}

template <>
void ogllib::AssignmentProxy<glm::vec3>::operator=(glm::vec3 &val)
{
    glUniform3fv(_location, 1, glm::value_ptr(val));
}
// ...
