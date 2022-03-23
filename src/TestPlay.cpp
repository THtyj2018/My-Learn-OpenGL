#include "TestPlay.h"

#include "Graphics/Graphics.h"
#include "Graphics/PhongPipeline.h"
#include "Graphics/CameraObject.h"
#include "Graphics/LightObject.h"
#include "Graphics/Mesh.h"
#include "Graphics/Material.h"
#include "Graphics/Texture.h"

#include "Resources/Image.h"

#include "Math/mat4.h"

TestPlay::TestPlay()
{
}

TestPlay::~TestPlay()
{
}

int TestPlay::Initialize()
{
	Phong = MakeShared<PhongPipeline>();
	if (Phong->Valid() == false)
		return -1;
	Graphics::GetSingleton()->UsePipeline(Phong);
	Phong->SetShaderParam("Config.UseHalfLambert", true);
	Phong->SetShaderParam("Config.UseBlinnPhong", true);

	TestCamera = MakeUnique<CameraObject>();
	TestCamera->lookAt = mat4(vec3(2, 1.5f, 2.5f), vec3(0), vec3::up());
	TestCamera->type = CameraType::Perspective;
	TestCamera->nearClip = 0.1f;

	DirLight = MakeUnique<SunLightObject>();
	DirLight->ambient = Color(vec3(0.25f));
	DirLight->direction = vec3(-2, -5, 2);
	DirLight->intensity = 0.75f;

	Light01 = MakeUnique<OmniLightObject>();
	Light01->position = vec3(3, 4, -3);
	Light01->direction = -Light01->position;
	Light01->color = Color::white();
	Light01->intensity = 1.0f;
	Light01->attenuation = 0.05f;
	Light01->coneAngle = Mathf::Pi / 3;
	Light01->spot = false;

	Light02 = MakeUnique<OmniLightObject>();
	Light02->position = vec3(-3, 4, 3);
	Light02->direction = vec3::down();
	Light02->color = Color::magenta();
	Light02->intensity = 1.0f;
	Light02->coneAngle = Mathf::Pi / 3;
	Light02->spot = false;

	Material01 = MakeShared<Material>();
	Material01->diffuse = Color::white();
	Material01->glosiness = 16.f;
	Material01->specular = 0.4f;

	auto marble_img = MakeShared<Image>("marble.jpg");
	auto marble_tex = MakeShared<Texture>(marble_img);
	Material02 = MakeShared<Material>();
	Material02->diffuse = marble_tex;
	Material02->glosiness = 24.0f;
	Material02->specular = 0.7f;

	auto stone_brick_img = MakeShared<Image>("stone_brick.jpg");
	auto stone_brick_tex = MakeShared<Texture>(stone_brick_img);
	Material03 = MakeShared<Material>();
	Material03->diffuse = stone_brick_tex;
	Material03->glosiness = 10.0f;
	Material03->specular = 0.3f;

	Ground = UniquePtr<Mesh>(Mesh::NewCuboid(Phong, 10.0f, 0.2f, 10.0f, VertexAttributes(VertexAttrib::TexCoord), Material03));
	Ground->FillCuboidTexCoords(0, 0, 5.0f, 5.0f);
	Ground->BindGPUResources();
	Ground->transform.cols[3] = vec4(0, -0.6f, 0);

	Cube01 = UniquePtr<Mesh>(Mesh::NewCube(Phong, 1.5f, VertexAttributes(VertexAttrib::TexCoord), Material02));
	Cube01->FillCuboidTexCoords(0, 0, 1, 1);
	Cube01->BindGPUResources();

	Sphere01 = UniquePtr<Mesh>(Mesh::NewSphere(Phong, 1.0f, 30, VertexAttributes(VertexAttrib::Color), Material01));
	Sphere01->FillColor(Color::cyan());
	Sphere01->BindGPUResources();
	Sphere01->transform.cols[3] = vec4(-2.0f, 1.0f, 1.0f);

	Sphere02 = UniquePtr<Mesh>(Mesh::NewSphere(Phong, 0.8f, 25, VertexAttributes(VertexAttrib::TexCoord), Material02));
	Sphere02->FillEllipsoidTexCoords();
	Sphere02->BindGPUResources();
	Sphere02->transform.cols[3] = vec4(1.0f, 1.3f, 1.5f);

	return 0;
}

void TestPlay::Finalize()
{
}

void TestPlay::Update(float dt)
{
	static float t = 0;
	vec2 xz = vec2(2, 2.5f).rotated(-t * Mathf::Pi * 0.3f);
	TestCamera->lookAt = mat4(vec3(xz.x, 2, xz.y), vec3(0), vec3::up());

	Light02->intensity = 0.5f + 0.5f * Mathf::cos(t * Mathf::Pi * 0.8f);

	t += dt;
}

void TestPlay::Draw()
{
	Phong->SetCameraParams(TestCamera.get());
	Phong->SetLightParams(DirLight.get());
	Phong->SetLightParams(1, Light01.get());
	Phong->SetLightParams(2, Light02.get());

	Phong->SetMaterialParams(Material01.get());
	Sphere01->Draw();

	Phong->SetMaterialParams(Material02.get());
	Cube01->Draw();
	Sphere02->Draw();

	Phong->SetMaterialParams(Material03.get());
	Ground->Draw();
}