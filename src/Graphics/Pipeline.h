#pragma once

#include "VertexAttributes.h"

#include "../Utilities/String.h"
#include "../Utilities/String.h"
#include <cstdint>

class vec3;
class vec4;
class mat4;
class CameraObject;
class SunLightObject;
class OmniLightObject;
class Material;

class Pipeline
{
public:
	Pipeline(AnsiStringView VSSrc, AnsiStringView PSSrc);

	Pipeline(const Pipeline&) = delete;

	Pipeline& operator=(const Pipeline&) = delete;

	~Pipeline();

	bool Valid() const noexcept;

	uint32_t GetProgramID() const noexcept { return m_Program; }

	int32_t GetAttribLocation(VertexAttrib attrib) const;

	void SetShaderParam(const AnsiString& name, bool b);

	void SetShaderParam(const AnsiString& name, int v);

	void SetShaderParam(const AnsiString& name, float v);

	void SetShaderParam(const AnsiString& name, const vec3& v);

	void SetShaderParam(const AnsiString& name, const vec4& v);

	void SetShaderParam(const AnsiString& name, const mat4& m);

	void SetCameraParams(CameraObject* camera);

	void SetLightParams(SunLightObject* light);

	void SetLightParams(unsigned light_id, OmniLightObject* light);

	void SetMaterialParams(Material* material);

private:
	int32_t GetParamLocation(const AnsiString& name) const;

	uint32_t m_Program;
};