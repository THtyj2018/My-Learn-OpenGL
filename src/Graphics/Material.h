#pragma once

#include "Texture.h"
#include "../Math/vec3.h"

template <typename T> requires (DefaultConstructible<T> && Copyable<T>)
class MaterialParameterMap
{
public:
	MaterialParameterMap() noexcept : m_Value(), m_Texture(nullptr) {}

	MaterialParameterMap(const T& value) noexcept : m_Value(value), m_Texture(nullptr) {}

	MaterialParameterMap(SharedPtr<Texture> texture) noexcept : m_Texture(texture)
	{
		if (m_Texture == nullptr || !m_Texture->Valid())
			m_Texture = nullptr;
	}

	MaterialParameterMap(const MaterialParameterMap&) noexcept = default;

	MaterialParameterMap& operator=(const MaterialParameterMap<T>&) noexcept = default;

	MaterialParameterMap& operator=(const T& value) noexcept
	{
		m_Value = value;
		m_Texture = nullptr;
		return *this;
	}

	MaterialParameterMap& operator=(SharedPtr<Texture> texture) noexcept
	{
		m_Texture = texture;
		return *this;
	}

	bool UseMap() const noexcept { return m_Texture != nullptr && m_Texture->Valid(); }

	const T& GetValue() const noexcept { return m_Value; }

	SharedPtr<Texture> GetTexture() const noexcept { return m_Texture; }

private:
	T m_Value;
	SharedPtr<Texture> m_Texture;
};

using ScalarMap = MaterialParameterMap<float>;
using NormalMap = MaterialParameterMap<vec3>;
using ColorMap = MaterialParameterMap<Color>;

class Material
{
public:
	static SharedPtr<Material> Default() noexcept
	{
		static SharedPtr<Material> default_mat = MakeShared<Material>();
		return default_mat;
	}

	ColorMap diffuse = Color::white();
	float specular = 0.25f;
	float glosiness = 10.0f;
};