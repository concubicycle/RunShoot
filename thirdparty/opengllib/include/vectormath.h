#ifndef VC_VECTORMATH_H__
#define VC_VECTORMATH_H__

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/epsilon.hpp>

#include <stdlib.h>

const float PI_2 = 1.5707963f;
const float TWO_PI = 6.283185307f;

void set_translation(glm::mat4 &transform, const glm::vec3 &translation);
void set_translation(glm::mat4 &transform, float x, float y, float z);

void get_translation(const glm::mat4 &transform, glm::vec3 *translation);


glm::quat get_rotation_from(const glm::vec3 &source, const glm::vec3 &dest);
glm::quat get_rotation_from(const glm::vec4 &source, const glm::vec4 &dest);

#endif