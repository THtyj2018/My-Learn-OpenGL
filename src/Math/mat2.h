#pragma once

#include "vec2.h"

class mat2
{
public:
	constexpr mat2() noexcept
	{
		cols[0] = vec2(1, 0);
		cols[1] = vec2(0, 1);
	}

	constexpr mat2(const vec2& col0, const vec2& col1) noexcept
	{
		cols[0] = col0;
		cols[1] = col1;
	}

	constexpr mat2(float m00, float m10, float m01, float m11) noexcept
	{
		cols[0].x = m00;
		cols[0].y = m10;
		cols[1].x = m01;
		cols[1].y = m11;
	}

	constexpr mat2(const mat2&) = default;

	constexpr static mat2 identity() noexcept { return mat2(); }

	constexpr static mat2 zero() noexcept
	{
		return mat2(vec2(), vec2());
	}

	constexpr static mat2 eye(float e) noexcept
	{
		return mat2(vec2(e, 0), vec2(0, e));
	}

	constexpr bool operator==(const mat2& rhs) const noexcept = default;

	constexpr bool eqzero() const noexcept { return cols[0] == vec2::zero() && cols[1] == vec2::zero(); }

	constexpr mat2 operator+() const noexcept { return *this; }
	constexpr mat2 operator-() const noexcept { return mat2(-cols[0], -cols[1]); }

	constexpr mat2& operator+=(const mat2& rhs) noexcept
	{
		cols[0] += rhs.cols[0];
		cols[1] += rhs.cols[1];
		return *this;
	}

	constexpr mat2& operator-=(const mat2& rhs) noexcept
	{
		cols[0] -= rhs.cols[0];
		cols[1] -= rhs.cols[1];
		return *this;
	}

	constexpr mat2& operator*=(float scalar) noexcept
	{
		cols[0] *= scalar;
		cols[1] *= scalar;
		return *this;
	}

	constexpr mat2& operator/=(float scalar) noexcept
	{
		cols[0] /= scalar;
		cols[1] /= scalar;
		return *this;
	}

	constexpr mat2 operator+(const mat2& rhs) const noexcept
	{
		return mat2(cols[0] + rhs.cols[0], cols[1] + rhs.cols[1]);
	}

	constexpr mat2 operator-(const mat2& rhs) const noexcept
	{
		return mat2(cols[0] - rhs.cols[0], cols[1] - rhs.cols[1]);
	}

	constexpr mat2 operator*(const mat2& rhs) const noexcept
	{
		return mat2(
			operator*(rhs.cols[0]),
			operator*(rhs.cols[1])
		);
	}

	constexpr vec2 operator*(const vec2& rhs) const noexcept
	{
		return vec2(
			cols[0].x * rhs.x + cols[1].x * rhs.y,
			cols[0].y * rhs.x + cols[1].y * rhs.y
		);
	}

	constexpr mat2 operator*(float scalar) const noexcept
	{
		return mat2(
			cols[0] * scalar,
			cols[1] * scalar
		);
	}

	constexpr mat2 operator/(float scalar) const noexcept
	{
		return mat2(
			cols[0] / scalar,
			cols[1] / scalar
		);
	}

	constexpr mat2 transposed() const noexcept
	{
		return mat2(
			cols[0].x,
			cols[1].x,
			cols[0].y,
			cols[1].y
		);
	}

	constexpr void transpose() noexcept
	{
		std::swap(cols[0].get<1>(), cols[1].get<0>());
	}

	constexpr float det() const noexcept
	{
		return (cols[0].x * cols[1].y - cols[1].x * cols[0].y);
	}

	constexpr mat2 inverse() const noexcept
	{
		return mat2(cols[1].y, -cols[0].y, -cols[1].x, cols[0].x) / det();
	}

	vec2 cols[2];
};