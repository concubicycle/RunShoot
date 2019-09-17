#include "vectormath.h"

glm::quat get_rotation_from(const glm::vec3 &source, const glm::vec3 &dest)
{

	//early out
	if (glm::length(source) == 0 || glm::length(dest) == 0)
		return glm::quat(); //identity glm::quat?

	//begin
	glm::quat rot;
	glm::vec3 v1 = glm::normalize(source);
	glm::vec3 v2 = glm::normalize(dest);

	float dp = glm::dot(v1, v2);

	//if vectors are aligned or invalid, no need to rotate
	if (dp >= 1)
	{
		return glm::quat(1, 0, 0, 0); //identity glm::quaternion
	}

	//if 180degrees apart, rotate 180 degrees about axis
	if (abs(dp - -1.0f) < glm::epsilon<float>())
	{
		glm::vec3 axis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), source);

		if (abs(axis.length()) < glm::epsilon<float>())
			axis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), source);

		axis = glm::normalize(axis);
		rot = glm::angleAxis(glm::pi<float>(), axis);
	}
	else
	{
		float sq = glm::sqrt((1.f + dp) * 2.0f);
		float inv_sqrt = 1.f / sq;

		glm::vec3 cp = glm::cross(v1, v2);

		rot.x = cp.x * inv_sqrt;
		rot.y = cp.y * inv_sqrt;
		rot.z = cp.z * inv_sqrt;
		rot.w = sq * 0.5f;

		rot = glm::normalize(rot);
	}
	return rot;
}

glm::quat get_rotation_from(const glm::vec4 &source, const glm::vec4 &dest)
{
	return get_rotation_from(glm::vec3(source.x, source.y, source.z), glm::vec3(dest.x, dest.y, dest.z));
}

void set_translation(glm::mat4 &transform, const glm::vec3 &translation)
{
	float *mat = glm::value_ptr(transform);

	mat[12] = translation.x;
	mat[13] = translation.y;
	mat[14] = translation.z;
}

void get_translation(const glm::mat4 &transform, glm::vec3 *translation)
{
	translation->x = transform[3][0];
	translation->y = transform[3][1];
	translation->z = transform[3][2];
}
