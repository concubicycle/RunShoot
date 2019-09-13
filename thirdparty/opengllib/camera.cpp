#include "camera.h"

ogllib::camera::camera()
{
	_pitch = _yaw = _roll = 0;
	reset_projection();
}

ogllib::camera::~camera()
{
}

void ogllib::camera::setAspectRatio(float aspect_ration)
{
	_aspectRatio = aspect_ration;
	reset_projection();
}
void ogllib::camera::setFovY(float fovy)
{
	_fovY = fovy;
	reset_projection();
}

void ogllib::camera::orthogonalize_uvn()
{
	_forward = glm::normalize(_forward);

	_up = glm::cross(_forward, _right);
	_up = glm::normalize(_up);

	_right = glm::cross(_up, _forward);
	_right = glm::normalize(_right);
}

void ogllib::camera::translate(glm::vec3 disp)
{
	_eyeLocation += disp;
}

void ogllib::camera::rotate(glm::quat rotation)
{
	_view = glm::mat4_cast(rotation) * _view;
	orthogonalize_uvn();
}

void ogllib::camera::rotate(float theta, glm::vec3 &axis)
{
	_view = glm::rotate(_view, theta, axis);
	orthogonalize_uvn();
}

void ogllib::camera::rotate(float a_yaw_deg, float a_pitch_deg, float a_roll_deg)
{
	switch (_behavior)
	{
	case FirstPerson:
		rotate_first_person(a_yaw_deg, a_pitch_deg);
		break;

	case Flight:
		rotate_flight(a_yaw_deg, a_pitch_deg, a_roll_deg);
		break;
	}
}

void ogllib::camera::set_position(const glm::vec3 &pos)
{
	_eyeLocation = pos;
	createViewMat(false);
}

void ogllib::camera::setOrientation(const glm::quat &orientation)
{
	glm::vec3 prev_pos;
	get_translation(_view, &prev_pos);
	_view = glm::mat4_cast(orientation);
	set_translation(_view, prev_pos);
}

void ogllib::camera::pitch(float theta)
{
	_pitch += theta;

	if (_pitch >= PI_2)
		_pitch = PI_2;
	if (_pitch < -PI_2)
		_pitch = -PI_2;
}

void ogllib::camera::yaw(float theta)
{
	_yaw += theta;

	if (_yaw >= TWO_PI)
		_yaw -= TWO_PI;
	if (_yaw < 0.0f)
		_yaw += TWO_PI;
}

void ogllib::camera::roll(float theta)
{
	_roll += theta;

	if (_roll >= TWO_PI)
		_roll -= TWO_PI;
	if (_roll < 0.0f)
		_roll += TWO_PI;
}

void ogllib::camera::move_forward(float units)
{
	_eyeLocation += _forward * units;
}

void ogllib::camera::strafeRight(float units)
{
	_eyeLocation += _right * units;
}

void ogllib::camera::createViewMat(bool reorthognolaze)
{
	if (reorthognolaze)
		orthogonalize_uvn();

	float *mat = glm::value_ptr(_view);

	mat[0] = _right.x;
	mat[4] = _right.y;
	mat[8] = _right.z;
	mat[12] = -glm::dot(_right, _eyeLocation);

	mat[1] = _up.x;
	mat[5] = _up.y;
	mat[9] = _up.z;
	mat[13] = -glm::dot(_up, _eyeLocation);

	mat[2] = _forward.x;
	mat[6] = _forward.y;
	mat[10] = _forward.z;
	mat[14] = -glm::dot(_forward, _eyeLocation);

	mat[3] = 0;
	mat[7] = 0;
	mat[11] = 0;
	mat[15] = 1.0f;
}

void ogllib::camera::setZPlanes(float znear, float zfar)
{
	_zNear = znear;
	_zFar = zfar;

	reset_projection();
}

inline const void ogllib::camera::reset_projection()
{
	_projection = glm::perspective(_fovY, _aspectRatio, _zNear, _zFar);
}

void ogllib::camera::rotate_first_person(float a_heading_deg, float a_pitch_deg)
{
	_pitch += a_pitch_deg;

	if (_pitch > 90.0f)
	{
		a_pitch_deg = PI_2 - (_pitch - a_pitch_deg);
		_pitch = PI_2;
	}

	if (_pitch < -PI_2)
	{
		a_pitch_deg = -PI_2 - (_pitch - a_pitch_deg);
		_pitch = -90.0f;
	}

	glm::mat4 rotMtx;

	// Rotate camera's existing x and z axes about the world y axis.
	if (a_heading_deg != 0.0f)
	{
		rotMtx = glm::rotate(glm::mat4(1), a_heading_deg, glm::vec3(0, 1, 0));
		_right = glm::vec3(glm::vec4(_right, 0) * rotMtx);	 //m_xAxis * rotMtx;
		_forward = glm::vec3(glm::vec4(_forward, 0) * rotMtx); //m_zAxis * rotMtx;
	}

	// Rotate camera's existing y and z axes about its existing x axis.
	if (a_pitch_deg != 0.0f)
	{
		rotMtx = glm::rotate(glm::mat4(1), a_pitch_deg, _right);
		//rotMtx.rotate(m_xAxis, pitchDegrees);
		_up = glm::vec3(glm::vec4(_up, 0) * rotMtx);		   //m_yAxis * rotMtx;
		_forward = glm::vec3(glm::vec4(_forward, 0) * rotMtx); //m_zAxis * rotMtx;
	}
}

void ogllib::camera::apply()
{
	// Calculate the new Front vector
	_forward.x = cos(_yaw) * cos(_pitch);
	_forward.y = sin(_pitch);
	_forward.z = sin(_yaw) * cos(_pitch);
	_forward = glm::normalize(_forward);

	// Also re-calculate the Right and Up vector
	_right = glm::normalize(glm::cross(_forward, glm::vec3(0, 1, 0))); // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	_up = glm::normalize(glm::cross(_right, _forward));

	;
	_lookat = _eyeLocation + _forward;
	_view = glm::lookAt(_eyeLocation, _lookat, _up);
}

void ogllib::camera::rotate_flight(float a_heading_deg, float a_pitch_deg, float a_roll_deg)
{
	glm::mat4 rotMtx;

	// Rotate camera's existing x and z axes about its existing y axis.
	if (a_heading_deg != 0.0f)
	{
		rotMtx = glm::rotate(glm::mat4(1), a_heading_deg, _up);
		_right = glm::vec3(glm::vec4(_right, 0) * rotMtx);	 //m_xAxis * rotMtx;
		_forward = glm::vec3(glm::vec4(_forward, 0) * rotMtx); //m_zAxis * rotMtx;
	}

	// Rotate camera's existing y and z axes about its existing x axis.
	if (a_pitch_deg != 0.0f)
	{
		rotMtx = glm::rotate(glm::mat4(1), a_pitch_deg, _right);
		_up = glm::vec3(glm::vec4(_up, 0) * rotMtx);		   //m_yAxis * rotMtx;
		_forward = glm::vec3(glm::vec4(_forward, 0) * rotMtx); //m_zAxis * rotMtx;
	}

	// Rotate camera's existing x and y axes about its existing z axis.
	if (a_roll_deg != 0.0f)
	{
		rotMtx = glm::rotate(glm::mat4(1), a_roll_deg, _forward);
		_right = glm::vec3(glm::vec4(_right, 0) * rotMtx); //m_xAxis * rotMtx;
		_up = glm::vec3(glm::vec4(_up, 0) * rotMtx);	   //m_xAxis * rotMtx;
	}
}
