#pragma once

#include "Mathf.h"

#include "vec2.h"

#include <tuple>

class vec3;

/// @brief Eular angle
using Eular = vec3;
/// @brief RGB Color
using Color3 = vec3;

/// @brief 3-dimensional math vector.
class vec3
{
public:
	constexpr vec3() noexcept : x(0), y(0), z(0) {}
	constexpr explicit vec3(float scalar) noexcept : x(scalar), y(scalar), z(scalar) {}
	constexpr vec3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}
	constexpr vec3(const vec2& _xy, float _z) noexcept : x(_xy.x), y(_xy.y), z(_z) {}

	constexpr vec3(const vec3&) noexcept = default;

	operator const float*() const noexcept { return &x; }
	operator float*() noexcept { return &x; }

	constexpr static vec3 zero() noexcept { return vec3(); }
	constexpr static vec3 one() noexcept { return vec3(1); }
	constexpr static vec3 half() noexcept { return vec3(0.5f); }
	
	constexpr static vec3 right() noexcept { return vec3(1, 0, 0); }
	constexpr static vec3 left() noexcept { return vec3(-1, 0, 0); }
	constexpr static vec3 up() noexcept { return vec3(0, 1, 0); }
	constexpr static vec3 down() noexcept { return vec3(0, -1, 0); }
	constexpr static vec3 front() noexcept { return vec3(0, 0, 1); }
	constexpr static vec3 back() noexcept { return vec3(0, 0, -1); }

	constexpr static Color3 white() noexcept { return vec3(1.0f); }
	constexpr static Color3 black() noexcept { return vec3(0.0f); }
	constexpr static Color3 gray() noexcept { return vec3(0.5f); }
	constexpr static Color3 red() noexcept { return vec3(1.0f, 0.0f, 0.0f); }
	constexpr static Color3 green() noexcept { return vec3(0.0f, 1.0f, 0.0f); }
	constexpr static Color3 blue() noexcept { return vec3(0.0f, 0.0f, 1.0f); }
	constexpr static Color3 cyan() noexcept { return vec3(0.0f, 1.0f, 1.0f); }
	constexpr static Color3 magenta() noexcept { return vec3(1.0f, 0.0f, 1.0f); }
	constexpr static Color3 yellow() noexcept { return vec3(1.0f, 1.0f, 0.0f); }

	constexpr bool operator==(const vec3& rhs) const noexcept
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	constexpr bool eqzero() const noexcept { return *this == zero(); }

	constexpr bool fuzzy_zero() const noexcept
	{
		return Mathf::fuzzy_zero(x) && Mathf::fuzzy_zero(y) && Mathf::fuzzy_zero(z);
	}

	constexpr bool fuzzy_eq(const vec3& rhs) const noexcept
	{
		return Mathf::fuzzy_eq(x, rhs.x) && Mathf::fuzzy_eq(y, rhs.y) && Mathf::fuzzy_eq(z, rhs.z);
	}

	constexpr vec3 clamp01() const noexcept
	{
		return vec3(Mathf::clamp01(x), Mathf::clamp01(y), Mathf::clamp01(z));
	}

	constexpr vec3 operator+() const noexcept { return *this; }
	constexpr vec3 operator-() const noexcept { return vec3(-x, -y, -z); }

	constexpr vec3& operator+=(const vec3& rhs) noexcept
	{
		x += rhs.x; y += rhs.y; z += rhs.z;
		return *this;
	}

	constexpr vec3& operator-=(const vec3& rhs) noexcept
	{
		x -= rhs.x; y -= rhs.y; z -= rhs.z;
		return *this;
	}

	constexpr vec3& operator*=(const vec3& rhs) noexcept
	{
		x *= rhs.x; y *= rhs.y; z *= rhs.z;
		return *this;
	}

	constexpr vec3& operator/=(const vec3& rhs) noexcept
	{
		x /= rhs.x; y /= rhs.y; z /= rhs.z;
		return *this;
	}

	constexpr vec3 operator+(const vec3& rhs) const noexcept
	{
		return vec3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	constexpr vec3 operator-(const vec3& rhs) const noexcept
	{
		return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	constexpr vec3 operator*(const vec3& rhs) const noexcept
	{
		return vec3(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	constexpr vec3 operator/(const vec3& rhs) const noexcept
	{
		return vec3(x / rhs.x, y / rhs.y, z / rhs.z);
	}

	constexpr vec3& operator*=(float scalar) noexcept
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	constexpr vec3& operator/=(float scalar) noexcept
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	constexpr vec3 operator*(float scalar) const noexcept
	{
		return vec3(x * scalar, y * scalar, z * scalar);
	}

	constexpr vec3 operator/(float scalar) const noexcept
	{
		return vec3(x / scalar, y / scalar, z / scalar);
	}

	constexpr friend vec3 operator*(float scalar, const vec3& rhs) noexcept
	{
		return rhs * scalar;
	}

	constexpr friend vec3 operator/(float scalar, const vec3& rhs) noexcept
	{
		return vec3(scalar / rhs.x, scalar / rhs.y, scalar / rhs.z);
	}

	constexpr float dot_product(const vec3& rhs) const noexcept
	{
		vec3 mul = operator*(rhs);
		return mul.x + mul.y + mul.z;
	}

	constexpr vec3 cross_product(const vec3& rhs) const noexcept
	{
		return vec3(
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x
		);
	}

	constexpr float manhatten() const noexcept
	{
		return Mathf::abs(x) + Mathf::abs(y) + Mathf::abs(z);
	}

	constexpr float length_squared() const noexcept { return dot_product(*this); }
	float length() const noexcept { return Mathf::sqrt(length_squared()); }
	float rlength() const noexcept { return Mathf::rsqrt(length_squared()); }
	
	vec3 normalized() const noexcept
	{
		vec3 ret = *this;
		if (!eqzero())
			ret *= rlength();
		return ret;
	}

	void normalize() noexcept
	{
		if (!eqzero())
			*this *= rlength();
	}

	float arg(const vec3& rhs) const noexcept
	{
		return Mathf::acos(dot_product(rhs) * rlength() * rhs.rlength());
	}

	template <size_t idx> requires (idx < 3)
		constexpr float get() const noexcept
	{
		if constexpr (idx == 0)
			return x;
		else if constexpr (idx == 1)
			return y;
		else
			return z;
	}

	template <size_t idx> requires (idx < 3)
		constexpr float& get() noexcept
	{
		if constexpr (idx == 0)
			return x;
		else if constexpr (idx == 1)
			return y;
		else
			return z;
	}

	template <size_t i, size_t j> requires (i < 3 && j < 3)
		constexpr vec2 swizzle() const noexcept
	{
		return vec2(get<i>(), get<j>());
	}

	template <size_t i, size_t j, size_t k> requires (i < 3 && j < 3 && k < 3)
		constexpr vec3 swizzle() const noexcept
	{
		return vec3(get<i>(), get<j>(), get<k>());
	}

	union
	{
		struct { float x, y, z; };
		struct { float pitch, yaw, roll; };
		struct { float r, g, b; };
		struct { float u, v, w; };
	};
};

namespace std
{
	template <>
	struct tuple_size<vec3> : std::integral_constant<size_t, 3> {};

	template <size_t idx> requires (idx < 3)
	struct tuple_element<idx, vec3>
	{
		using type = float;
	};
}