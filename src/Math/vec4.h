#pragma once

#include "Mathf.h"

#include "vec3.h"

#include <tuple>

class vec4;

/// @brief RGBA Color
using Color = vec4;

/// @brief 4-dimensional math vector
class vec4
{
public:
	constexpr vec4() noexcept : x(0), y(0), z(0), w(0) {}
	constexpr explicit vec4(float scalar) noexcept : x(scalar), y(scalar), z(scalar), w(scalar) {}
	constexpr vec4(float _x, float _y, float _z, float _w = 1.0f) noexcept : x(_x), y(_y), z(_z), w(_w) {}
	constexpr explicit vec4(const vec3& xyz, float _w = 1.0f) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(_w) {}
	constexpr vec4(const vec2& xy, float _z, float _w = 1.0f) noexcept : x(xy.x), y(xy.y), z(_z), w(_w) {}
	constexpr vec4(const vec2& xy, const vec2& zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}

	constexpr vec4(const vec4&) noexcept = default;

	constexpr explicit operator vec3() const noexcept
	{
		return vec3(x, y, z);
	}

	operator const float* () const noexcept { return &x; }
	operator float* () noexcept { return &x; }

	constexpr static vec4 zero() noexcept { return vec4(); }
	constexpr static vec4 one() noexcept { return vec4(1); }

	constexpr static vec4 right() noexcept { return vec4(1, 0, 0, 1); }
	constexpr static vec4 left() noexcept { return vec4(-1, 0, 0, 1); }
	constexpr static vec4 up() noexcept { return vec4(0, 1, 0, 1); }
	constexpr static vec4 down() noexcept { return vec4(0, -1, 0, 1); }
	constexpr static vec4 front() noexcept { return vec4(0, 0, 1, 1); }
	constexpr static vec4 back() noexcept { return vec4(0, 0, -1, 1); }

	constexpr static Color white() noexcept { return vec4(1.0f); }
	constexpr static Color black() noexcept { return vec4(0.0f, 0.0f, 0.0f, 1.0f); }
	constexpr static Color gray() noexcept { return vec4(0.5f, 0.5f, 0.5f, 1.0f); }
	constexpr static Color red() noexcept { return vec4(1.0f, 0.0f, 0.0f, 1.0f); }
	constexpr static Color green() noexcept { return vec4(0.0f, 1.0f, 0.0f, 1.0f); }
	constexpr static Color blue() noexcept { return vec4(0.0f, 0.0f, 1.0f, 1.0f); }
	constexpr static Color cyan() noexcept { return vec4(0.0f, 1.0f, 1.0f, 1.0f); }
	constexpr static Color magenta() noexcept { return vec4(1.0f, 0.0f, 1.0f, 1.0f); }
	constexpr static Color yellow() noexcept { return vec4(1.0f, 1.0f, 0.0f, 1.0f); }

	constexpr bool operator==(const vec4& rhs) const noexcept
	{
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}

	constexpr bool eqzero() const noexcept { return *this == zero(); }

	constexpr bool fuzzy_zero() const noexcept
	{
		return Mathf::fuzzy_zero(x) && Mathf::fuzzy_zero(y) &&
			Mathf::fuzzy_zero(z) && Mathf::fuzzy_zero(w);
	}

	constexpr bool fuzzy_eq(const vec4& rhs) const noexcept
	{
		return Mathf::fuzzy_eq(x, rhs.x) && Mathf::fuzzy_eq(y, rhs.y) &&
			Mathf::fuzzy_eq(z, rhs.z) && Mathf::fuzzy_eq(w, rhs.w);
	}

	constexpr Color clamp01() const noexcept
	{
		return vec4(Mathf::clamp01(x), Mathf::clamp01(y), Mathf::clamp01(z), Mathf::clamp01(w));
	}

	constexpr vec4 operator+() const noexcept { return *this; }
	constexpr vec4 operator-() const noexcept { return vec4(-x, -y, -z, -w); }

	constexpr vec4& operator+=(const vec4& rhs) noexcept
	{
		x += rhs.x; y += rhs.y;
		z += rhs.z; w += rhs.w;
		return *this;
	}

	constexpr vec4& operator-=(const vec4& rhs) noexcept
	{
		x -= rhs.x; y -= rhs.y;
		z -= rhs.z; w -= rhs.w;
		return *this;
	}

	constexpr vec4& operator*=(const vec4& rhs) noexcept
	{
		x *= rhs.x; y *= rhs.y;
		z *= rhs.z; w *= rhs.w;
		return *this;
	}

	constexpr vec4& operator/=(const vec4& rhs) noexcept
	{
		x /= rhs.x; y /= rhs.y;
		z /= rhs.z; w /= rhs.w;
		return *this;
	}

	constexpr vec4 operator+(const vec4& rhs) const noexcept
	{
		return vec4(
			x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w
		);
	}

	constexpr vec4 operator-(const vec4& rhs) const noexcept
	{
		return vec4(
			x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w
		);
	}

	constexpr vec4 operator*(const vec4& rhs) const noexcept
	{
		return vec4(
			x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w
		);
	}

	constexpr vec4 operator/(const vec4& rhs) const noexcept
	{
		return vec4(
			x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w
		);
	}

	constexpr vec4& operator*=(float scalar) noexcept
	{
		x *= scalar; y *= scalar;
		z *= scalar, w *= scalar;
		return *this;
	}

	constexpr vec4& operator/=(float scalar) noexcept
	{
		x /= scalar; y /= scalar;
		z /= scalar; w /= scalar;
		return *this;
	}

	constexpr vec4 operator*(float scalar) const noexcept
	{
		return vec4(
			x * scalar, y * scalar, z * scalar, w * scalar
		);
	}

	constexpr vec4 operator/(float scalar) const noexcept
	{
		return vec4(
			x / scalar, y / scalar, z / scalar, w / scalar
		);
	}

	constexpr friend vec4 operator*(float scalar, const vec4& rhs) noexcept
	{
		return rhs * scalar;
	}

	constexpr friend vec4 operator/(float scalar, const vec4& rhs) noexcept
	{
		return vec4(scalar / rhs.x, scalar / rhs.y, scalar / rhs.z, scalar / rhs.w);
	}

	constexpr float dot_product(const vec4& rhs) const noexcept
	{
		vec4 mul = operator*(rhs);
		return mul.x + mul.y + mul.z + mul.w;
	}

	constexpr vec4 conjugation() const noexcept
	{
		return vec4(-x, -y, -z, w);
	}

	constexpr float manhatten() const noexcept
	{
		return Mathf::abs(x) + Mathf::abs(y) + Mathf::abs(z) + Mathf::abs(w);
	}

	float length_squared() const noexcept { return dot_product(*this); }
	float length() const noexcept { return Mathf::sqrt(length_squared()); }
	float rlength() const noexcept { return Mathf::rsqrt(length_squared()); }

	vec4 normalized() const noexcept
	{
		vec4 ret = *this;
		float ls = length_squared();
		if (!eqzero() && !Mathf::fuzzy_eq(ls, 1.0f))
			ret *= Mathf::rsqrt(ls);
		return ret;
	}

	void normalize() noexcept
	{
		float ls = length_squared();
		if (!eqzero() && !Mathf::fuzzy_eq(ls, 1.0f))
			*this *= Mathf::rsqrt(ls);
	}

	static vec4 random() noexcept
	{
		return vec4(
			Mathf::random(), Mathf::random(), Mathf::random(), Mathf::random()
		);
	}

	template <size_t idx> requires (idx < 4)
		constexpr float get() const noexcept
	{
		if constexpr (idx == 0)
			return x;
		else if constexpr (idx == 1)
			return y;
		else if constexpr (idx == 2)
			return z;
		else
			return w;
	}

	template <size_t idx> requires (idx < 4)
		constexpr float& get() noexcept
	{
		if constexpr (idx == 0)
			return x;
		else if constexpr (idx == 1)
			return y;
		else if constexpr (idx == 2)
			return z;
		else
			return w;
	}

	template <size_t i, size_t j> requires (i < 4 && j < 4)
		constexpr vec2 swizzle() const noexcept
	{
		return vec2(get<i>(), get<j>());
	}

	template <size_t i, size_t j, size_t k> requires (i < 4 && j < 4 && k < 4)
		constexpr vec3 swizzle() const noexcept
	{
		return vec3(get<i>(), get<j>(), get<k>());
	}

	template <size_t i, size_t j, size_t k, size_t m> requires (i < 4 && j < 4 && k < 4 && m < 4)
		constexpr vec4 swizzle() const noexcept
	{
		return vec4(get<i>(), get<j>(), get<k>(), get<m>());
	}

	union
	{
		struct { float x, y, z, w; };
		struct { vec3 _vec3; float _w; };
		struct { float r, g, b, a; };
	};
};

namespace std
{
	template <>
	struct tuple_size<vec4> : std::integral_constant<size_t, 4> {};

	template <size_t idx> requires (idx < 4)
	struct tuple_element<idx, vec4>
	{
		using type = float;
	};
}