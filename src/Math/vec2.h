#pragma once

#include "Mathf.h"

#include <tuple>

class vec2
{
public:
	constexpr vec2() noexcept : x(0), y(0) {}
	constexpr explicit vec2(float scala) noexcept : x(scala), y(scala) {}
	constexpr vec2(float _x, float _y) noexcept : x(_x), y(_y) {}

	constexpr vec2(const vec2&) noexcept = default;

	constexpr static vec2 zero() noexcept { return vec2(); }
	constexpr static vec2 one() noexcept { return vec2(1); }
	constexpr static vec2 half() noexcept { return vec2(0.5f); }
	constexpr static vec2 right() noexcept { return vec2(1, 0); }
	constexpr static vec2 up() noexcept { return vec2(0, 1); }
	constexpr static vec2 left() noexcept { return vec2(-1, 0); }
	constexpr static vec2 down() noexcept { return vec2(0, -1); }

	static vec2 unit(float rad) noexcept { return vec2(Mathf::cos(rad), Mathf::sin(rad)); }

	constexpr bool operator==(const vec2& rhs) const noexcept { return x == rhs.x && y == rhs.y; }

	constexpr bool eqzero() const noexcept { return x == 0 && y == 0; }
	constexpr bool fuzzy_zero() const noexcept { return Mathf::fuzzy_zero(x) && Mathf::fuzzy_zero(y); }
	constexpr bool fuzzy_eq(const vec2& rhs) const noexcept { return Mathf::fuzzy_eq(x, rhs.x) && Mathf::fuzzy_eq(y, rhs.y); }

	constexpr vec2 operator+() const noexcept { return *this; }
	constexpr vec2 operator-() const noexcept { return vec2(-x, -y); }

	constexpr vec2& operator+=(const vec2& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
	constexpr vec2& operator-=(const vec2& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
	constexpr vec2& operator*=(const vec2& rhs) noexcept { x *= rhs.x; y *= rhs.y; return *this; }
	constexpr vec2& operator/=(const vec2& rhs) noexcept { x /= rhs.x; y /= rhs.y; return *this; }

	constexpr vec2 operator+(const vec2& rhs) const noexcept { return vec2(x + rhs.x, y + rhs.y); }
	constexpr vec2 operator-(const vec2& rhs) const noexcept { return vec2(x - rhs.x, y - rhs.y); }
	constexpr vec2 operator*(const vec2& rhs) const noexcept { return vec2(x * rhs.x, y * rhs.y); }
	constexpr vec2 operator/(const vec2& rhs) const noexcept { return vec2(x / rhs.x, y / rhs.y); }

	constexpr vec2& operator*=(float scalar) noexcept { x *= scalar; y *= scalar; return *this; }
	constexpr vec2& operator/=(float scalar) noexcept { x /= scalar; y /= scalar; return *this; }
	constexpr vec2 operator*(float scalar) const noexcept { return vec2(x * scalar, y * scalar); }
	constexpr vec2 operator/(float scalar) const noexcept { return vec2(x / scalar, y / scalar); }

	constexpr friend vec2 operator*(float scalar, const vec2& rhs) noexcept { return rhs * scalar; }
	constexpr friend vec2 operator/(float scalar, const vec2& rhs) noexcept { return vec2(scalar / rhs.x, scalar / rhs.y); }

	constexpr float dot_product(const vec2& rhs) const noexcept
	{
		return x * rhs.x + y * rhs.y;
	}

	constexpr vec2 complex_product(const vec2& rhs) const noexcept
	{
		return vec2(x * rhs.x - y * rhs.y, x * rhs.y + y * rhs.x);
	}

	constexpr vec2 complex_div(const vec2& rhs) const noexcept
	{
		return vec2(x * rhs.x + y * rhs.y, -x * rhs.y + y * rhs.x)
			/ rhs.length_squared();
	}

	constexpr vec2 conjugation() const noexcept
	{
		return vec2(x, -y);
	}

	constexpr float manhatten() const noexcept { return Mathf::abs(x) + Mathf::abs(y); }
	constexpr float length_squared() const noexcept { return dot_product(*this); }
	float length() const noexcept { return Mathf::sqrt(length_squared()); }
	float rlength() const noexcept { return Mathf::rsqrt(length_squared()); }

	vec2 normalized() const noexcept
	{
		vec2 ret = *this;
		if (!eqzero())
			ret *= rlength();
		return ret;
	}

	void normalize() noexcept
	{
		if (!eqzero())
			*this *= rlength();
	}

	float argz() const noexcept { return Mathf::atan2(y, x); }

	float arg(const vec2& rhs) const noexcept { return complex_div(rhs).argz(); }

	vec2 rotated(float rad) const noexcept { return complex_product(unit(rad)); }
	void rotate(float rad) noexcept { *this = rotated(rad); }

	template <size_t idx> requires (idx < 2)
		constexpr float get() const noexcept
	{
		if constexpr (idx == 0)
			return x;
		else
			return y;
	}

	template <size_t idx> requires (idx < 2)
		constexpr float& get() noexcept
	{
		if constexpr (idx == 0)
			return x;
		else
			return y;
	}

	template <size_t i, size_t j> requires (i < 2 && j < 2)
		constexpr vec2 swizzle() const noexcept { return vec2(get<i>(), get<j>()); }

	union
	{
		struct { float x, y; };
		struct { float u, v; };
		float vals[2];
	};
};

namespace std
{
	template <>
	struct tuple_size<vec2> : std::integral_constant<size_t, 2> {};

	template <size_t idx> requires (idx < 2)
	struct tuple_element<idx, vec2> {
		using type = float;
	};
}