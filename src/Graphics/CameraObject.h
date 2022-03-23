#pragma once

#include "..\Math\mat4.h"

enum class CameraType
{
	Perspective,
	Orthogonal
};

class CameraObject
{
public:
	mat4 GetProjectionMatrix() const noexcept
	{
		if (type == CameraType::Perspective)
		{
			float t = Mathf::tan(fov * 0.5f);
			return mat4(
				vec4(1 / (t * aspect), 0, 0, 0),
				vec4(0, 1 / t, 0, 0),
				vec4(0, 0, -(farClip + nearClip) / (farClip - nearClip), -1),
				vec4(0, 0, -2 * farClip * nearClip / (farClip - nearClip), 0)
			);
		}
		else
		{
			return mat4(
				vec4(2 / (height * aspect), 0, 0, 0),
				vec4(0, 2 / height, 0, 0),
				vec4(0, 0, -2 / (farClip - nearClip), 0),
				vec4(0, 0, -(farClip + nearClip) / (farClip - nearClip), 1)
			);
		}
	}

	mat4 lookAt = mat4(vec3(2, 1, 0), vec3(0, 1, 0), vec3::up());
	CameraType type = CameraType::Perspective;
	float farClip = 100.0f;
	float nearClip = 1.0f;
	float aspect = 16 / 9.0f;
	float fov = Mathf::Pi / 3;
	float height = 3.0f;
};