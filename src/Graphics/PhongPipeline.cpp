#include "PhongPipeline.h"

#include "../Utilities/String.h"
using namespace std::string_view_literals;

namespace
{
	static const auto PhongVSSrc =
		"#version 440\n"
		""
		"uniform struct MVPMatrices {"
		"	mat4 Model;"
		"	mat4 View;"
		"	mat4 Projection;"
		"} MVP;\n"
		""
		"struct ShaderVariants {"
		"	vec3 Position;"
		"	vec3 Normal;"
		"	vec4 Color;"
		"	vec2 TexCoord;"
		"};"
		""
		"in ShaderVariants VSV;"
		"out ShaderVariants PSV;"
		""
		"void main() {"
		"	PSV.Position = vec3(MVP.Model * vec4(VSV.Position, 1));"
		"	PSV.Normal = normalize(vec3(MVP.Model * vec4(VSV.Normal, 1)));"
		"	PSV.Color = VSV.Color;"
		"	PSV.TexCoord = vec2(VSV.TexCoord.x, 1 - VSV.TexCoord.y);"
		"	gl_Position = MVP.Projection * MVP.View * vec4(PSV.Position, 1);"
		"}"sv;

	static const auto PhongPSSrc =
		"#version 440\n"
		""
		"struct ProcessedLightInfo {"
		"	vec3 SrcDir;"
		"	vec3 Color;"
		"	float Intensity;"
		"};\n"
		""
		"struct SunLightInfo {"
		"	vec3 Direction;"
		"	vec4 Color;"
		"	vec4 Ambient;"
		"	float Intensity;"
		"	bool Enabled;"
		"};\n"
		""
		"struct OmniLightInfo {"
		"	vec3 Position;"
		"	vec3 Direction;"
		"	vec4 Color;"
		"	float Intensity;"
		"	float Attenuation;"
		"	float SquareAttenuation;"
		"	float ConeAngle;"
		"	bool Spot;"
		"	bool Enabled;"
		"};\n"
		""
		"uniform struct ConfigInfo {"
		"	bool UseHalfLambert;"
		"	bool UseBlinnPhong;"
		"} Config;"
		""
		"uniform struct MaterialInfo {"
		"	vec4 Diffuse;"
		"	sampler2D DiffuseMap;"
		"	bool UseDiffuseMap;"
		"	float Specular;"
		"	float Glossiness;"
		"} Material;\n"
		""
		"uniform struct MVPMatrices {"
		"	mat4 Model;"
		"	mat4 View;"
		"	mat4 Projection;"
		"} MVP;\n"
		""
		"#define MAX_OMNI_LIGHTS 64\n"
		"uniform SunLightInfo SunLight;"
		"uniform OmniLightInfo OmniLights[MAX_OMNI_LIGHTS];"
		""
		"in struct ShaderVariants {"
		"	vec3 Position;"
		"	vec3 Normal;"
		"	vec4 Color;"
		"	vec2 TexCoord;"
		"} PSV;"
		"out vec4 fColor;\n"
		""
		"vec3 calculate_processed_light(ProcessedLightInfo light) {"
		"	vec3 normal = normalize(PSV.Normal);\n"
		""
		"	float diffuse_factor = 0;"
		"	vec4 mat_diffuse;"
		"	if (Config.UseHalfLambert)"
		"		diffuse_factor = 0.5f * dot(light.SrcDir, normal) + 0.5f;"
		"	else"
		"		diffuse_factor = max(0, dot(light.SrcDir, normal));"
		""
		"	if (Material.UseDiffuseMap) "
		"		mat_diffuse = texture(Material.DiffuseMap, PSV.TexCoord);"
		"	else"
		"		mat_diffuse = Material.Diffuse;"
		""
		"	vec3 diffuse = light.Intensity * vec3(mat_diffuse) * light.Color * diffuse_factor;"
		"	if (Config.UseHalfLambert)"
		"		diffuse = diffuse * diffuse * PSV.Color.xyz;"
		"	else"
		"		diffuse = diffuse * PSV.Color.xyz;\n"
		""
		"	vec3 view_dir = normalize(inverse(MVP.View)[3].xyz - PSV.Position);"
		"	float spec_factor = 0;"
		"	if (Config.UseBlinnPhong) {"
		"		vec3 h = normalize(view_dir + light.SrcDir);"
		"		spec_factor = max(0, dot(normal, h));"
		"	}"
		"	else {"
		"		vec3 reflect_dir = reflect(-light.SrcDir, normal);"
		"		spec_factor = max(0, dot(view_dir, reflect_dir));"
		"	}"
		"	vec3 specular = light.Color.xyz * light.Intensity * Material.Specular * pow(spec_factor, Material.Glossiness);\n"
		""
		"	return diffuse + specular;"
		"}\n"
		""
		"vec3 calculate_directional_light() {"
		"	ProcessedLightInfo light;"
		"	light.SrcDir = normalize(-SunLight.Direction);"
		"	light.Color = SunLight.Color.xyz;"
		"	light.Intensity = SunLight.Intensity;"
		"	return calculate_processed_light(light) + vec3(SunLight.Ambient) * PSV.Color.xyz;"
		"}\n"
		""
		"vec3 calculate_omni_light(int idx) {"
		"	ProcessedLightInfo light;"
		"	light.SrcDir = normalize(OmniLights[idx].Position - PSV.Position);"
		"	light.Color = OmniLights[idx].Color.xyz;\n"
		""
		"	float distance = length(OmniLights[idx].Position - PSV.Position);"
		"	light.Intensity = OmniLights[idx].Intensity /"
		"		(1 + OmniLights[idx].Attenuation * distance + OmniLights[idx].SquareAttenuation * distance * distance);"
		"	if (OmniLights[idx].Spot == true) {"
		"		if (dot(normalize(-OmniLights[idx].Direction), light.SrcDir) < cos(OmniLights[idx].ConeAngle * 0.5f))"
		"			light.Intensity = 0;"
		"	}"
		"	return calculate_processed_light(light);"
		"}\n"
		""
		"void main() {"
		"	vec3 light_result = vec3(0, 0, 0);"
		"	if (SunLight.Enabled) {"
		"		light_result += calculate_directional_light();"
		"	}"
		"	for (int idx = 0; idx < MAX_OMNI_LIGHTS; ++idx) {"
		"		if (OmniLights[idx].Enabled)"
		"			light_result += calculate_omni_light(idx);"
		"	}\n"
		""
		"	fColor = vec4(light_result, PSV.Color.w);"
		"}"sv;
}

PhongPipeline::PhongPipeline() : Pipeline(PhongVSSrc, PhongPSSrc)
{
}