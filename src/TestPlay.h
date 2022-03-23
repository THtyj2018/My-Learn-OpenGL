#pragma once

#include "Control/GamePlay.h"
#include "Utilities/Pointer.h"

class PhongPipeline;
class CameraObject;
class SunLightObject;
class OmniLightObject;
class Mesh;
class Material;

class TestPlay : public GamePlay
{
public:
	TestPlay();

	~TestPlay() override;

protected:
	int Initialize() override;

	void Finalize() override;

	void Update(float dt) override;

	void Draw() override;

private:
	SharedPtr<PhongPipeline> Phong;
	UniquePtr<CameraObject> TestCamera;
	UniquePtr<SunLightObject> DirLight;
	UniquePtr<OmniLightObject> Light01;
	UniquePtr<OmniLightObject> Light02;
	UniquePtr<Mesh> Ground;
	UniquePtr<Mesh> Cube01;
	UniquePtr<Mesh> Sphere01;
	UniquePtr<Mesh> Sphere02;
	SharedPtr<Material> Material01;
	SharedPtr<Material> Material02;
	SharedPtr<Material> Material03;
};

