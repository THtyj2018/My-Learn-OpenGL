#include "Pipeline.h"

#include "CameraObject.h"
#include "LightObject.h"
#include "Material.h"

#include "..\Math\mat4.h"

#include <glad/glad.h>

#include <cstdio>
#include <cassert>

enum class ShaderType
{
	VertexShader = 'vert',
	PixelShader = 'frag'
};

static const char* GetAttribName(VertexAttrib attr) noexcept
{
	switch (attr)
	{
	case VertexAttrib::Position:
		return "VSV.Position";
	case VertexAttrib::Normal:
		return "VSV.Normal";
	case VertexAttrib::Color:
		return "VSV.Color";
	case VertexAttrib::TexCoord:
		return "VSV.TexCoord";
	default:
		printf("Error: Unsupported vertex attrib!\n");
		assert(0);
		return "";
	}
}

static uint32_t CompileShader(AnsiStringView src, ShaderType type)
{
	uint32_t shader = glCreateShader(type == ShaderType::VertexShader ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
	const char* str = src.data();
	GLint len = static_cast<GLint>(src.length());
	glShaderSource(shader, 1, &str, &len);
	glCompileShader(shader);

	GLint flag;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &flag);
	if (flag == GL_FALSE)
	{
		char* info = new char[1024];
		glGetShaderInfoLog(shader, 1024, nullptr, info);
		glDeleteShader(shader);
		printf("Error compiling shader: %s\n", info);
		delete[] info;
		return 0;
	}
	return shader;
}

static bool LinkProgram(uint32_t pipeline)
{
	if (pipeline == 0)
		return false;
	glLinkProgram(pipeline);
	GLint flag;
	glGetProgramiv(pipeline, GL_LINK_STATUS, &flag);
	if (flag == GL_FALSE)
	{
		char* info = new char[1024];
		glGetProgramInfoLog(pipeline, 1024, nullptr, info);
		printf("Error linking pipeline: %s\n", info);
		delete[] info;
		return false;
	}
	return true;
}

Pipeline::Pipeline(AnsiStringView VSSrc, AnsiStringView PSSrc) :
	m_Program(0)
{
	uint32_t vs = CompileShader(VSSrc, ShaderType::VertexShader);
	if (vs == 0)
		return;
	uint32_t ps = CompileShader(PSSrc, ShaderType::PixelShader);
	if (ps == 0)
	{
		glDeleteShader(vs);
		return;
	}
	m_Program = glCreateProgram();
	glAttachShader(m_Program, vs);
	glAttachShader(m_Program, ps);
	if (LinkProgram(m_Program) == false)
	{
		glDeleteProgram(m_Program);
		m_Program = 0;
	}
	glDeleteShader(ps);
	glDeleteShader(vs);

	glVertexAttrib4fv(GetAttribLocation(VertexAttrib::Color), Color::white());
}

Pipeline::~Pipeline()
{
	if (m_Program)
		glDeleteProgram(m_Program);
}

bool Pipeline::Valid() const noexcept
{
	return m_Program != 0;
}

int32_t Pipeline::GetAttribLocation(VertexAttrib attrib) const
{
	return glGetAttribLocation(m_Program, GetAttribName(attrib));
}

void Pipeline::SetShaderParam(const AnsiString& name, bool b)
{
	if (GLint loc = GetParamLocation(name); loc >= 0)
		glUniform1i(loc, b ? GL_TRUE : GL_FALSE);
}

void Pipeline::SetShaderParam(const AnsiString& name, int v)
{
	if (GLint loc = GetParamLocation(name); loc >= 0)
		glUniform1i(loc, v);
}

void Pipeline::SetShaderParam(const AnsiString& name, float v)
{
	if (GLint loc = GetParamLocation(name); loc >= 0)
		glUniform1f(loc, v);
}

void Pipeline::SetShaderParam(const AnsiString& name, const vec3& v)
{
	if (GLint loc = GetParamLocation(name); loc >= 0)
		glUniform3fv(loc, 1, v);
}

void Pipeline::SetShaderParam(const AnsiString& name, const vec4& v)
{
	if (GLint loc = GetParamLocation(name); loc >= 0)
		glUniform4fv(loc, 1, v);
}

void Pipeline::SetShaderParam(const AnsiString& name, const mat4& m)
{
	if (GLint loc = GetParamLocation(name); loc >= 0)
		glUniformMatrix4fv(loc, 1, GL_FALSE, m.cols[0]);
}

void Pipeline::SetCameraParams(CameraObject* camera)
{
	if (camera == nullptr)
		return;
	SetShaderParam("MVP.Projection", camera->GetProjectionMatrix());
	SetShaderParam("MVP.View", camera->lookAt);
}

void Pipeline::SetLightParams(SunLightObject* light)
{
	AnsiString prefix = "SunLight.";
	if (light == nullptr)
	{
		SetShaderParam(prefix + "Enabled", false);
		return;
	}
	SetShaderParam(prefix + "Enabled", true);
	SetShaderParam(prefix + "Direction", light->direction);
	SetShaderParam(prefix + "Color", light->color);
	SetShaderParam(prefix + "Ambient", light->ambient);
	SetShaderParam(prefix + "Intensity", light->intensity);
}

void Pipeline::SetLightParams(unsigned light_id, OmniLightObject* light)
{
	AnsiString prefix = "OmniLights[" + std::to_string(light_id & 0xFF) + "].";
	if (light == nullptr)
	{
		SetShaderParam(prefix + "Enabled", false);
		return;
	}
	SetShaderParam(prefix + "Enabled", true);
	SetShaderParam(prefix + "Position", light->position);
	SetShaderParam(prefix + "Direction", light->direction);
	SetShaderParam(prefix + "Color", light->color);
	SetShaderParam(prefix + "Intensity", light->intensity);
	SetShaderParam(prefix + "Attenuation", light->attenuation);
	SetShaderParam(prefix + "SquareAttenuation", light->squareAttenuation);
	SetShaderParam(prefix + "ConeAngle", light->coneAngle);
	SetShaderParam(prefix + "Spot", light->spot);
}

void Pipeline::SetMaterialParams(Material* material)
{
	int gl_tex_off = 0;
	if (material == nullptr)
		return;
	if (material->diffuse.UseMap())
	{
		SetShaderParam("Material.UseDiffuseMap", true);
		glActiveTexture(GL_TEXTURE0 + gl_tex_off);
		glBindTexture(GL_TEXTURE_2D, material->diffuse.GetTexture()->GetTextureID());
		SetShaderParam("Material.DiffuseMap", gl_tex_off);
		++gl_tex_off;
	}
	else
	{
		SetShaderParam("Material.UseDiffuseMap", false);
		SetShaderParam("Material.Diffuse", material->diffuse.GetValue());
	}
	SetShaderParam("Material.Specular", material->specular);
	SetShaderParam("Material.Glossiness", material->glosiness);
}

int32_t Pipeline::GetParamLocation(const AnsiString& name) const
{
	return glGetUniformLocation(m_Program, name.c_str());
}