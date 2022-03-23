#pragma once

#include "..\Math\vec4.h"

class SunLightObject
{
public:
	vec3 direction = vec3(-1, -1, -1).normalized();
	Color color = Color::white();
	Color ambient = Color(0.25f, 0.25f, 0.25f);
	float intensity = 1.0f;
};

class OmniLightObject
{
public:
	vec3 position = vec3(2.0f, 2.0f, 2.0f);
	vec3 direction = vec3(-1, -1, -1).normalized();
	Color color = Color::white();
	float intensity = 1.0f;
	float attenuation = 0.1f;
	float squareAttenuation = 0.01f;
	float coneAngle = Mathf::Pi / 3;
	bool spot = false;
};