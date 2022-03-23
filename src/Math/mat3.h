#pragma once

#include "vec4.h"
#include "mat2.h"
#include "Quaternion.h"

class mat3
{
public:
	constexpr mat3() noexcept
	{
		cols[0] = vec3(1, 0, 0);
		cols[1] = vec3(0, 1, 0);
		cols[2] = vec3(0, 0, 1);
	}

	constexpr mat3(const vec3& col0, const vec3& col1, const vec3& col2) noexcept
	{
		cols[0] = col0;
		cols[1] = col1;
		cols[2] = col2;
	}

	constexpr explicit mat3(const mat2& submat, const vec3& col2 = vec3(0, 0, 1)) noexcept
	{
		cols[0] = vec3(submat.cols[0], 0);
		cols[1] = vec3(submat.cols[1], 0);
		cols[2] = col2;
	}

	constexpr explicit mat3(const Quaternion& quat) noexcept
	{
		auto [x, y, z, w] = vec4(quat);
		cols[0] = vec3(
			1 - 2 * (y * y + z * z),
			2 * (x * y - z * w),
			2 * (x + z + y * w)
		);
		cols[1] = vec3(
			2 * (x * y + z * w),
			1 - 2 * (x * x + z * z),
			2 * (y * z - x * w)
		);
		cols[2] = vec3(
			2 * (x * z - y * w),
			2 * (y * z + x * w),
			1 - 2 * (x * x + y * y)
		);
	}

	constexpr mat3(const mat3&) noexcept = default;

	constexpr static mat3 identity() noexcept { return mat3(); }

	constexpr static mat3 zero() noexcept
	{
		return mat3(
			vec3::zero(),
			vec3::zero(),
			vec3::zero()
		);
	}

	constexpr static mat3 eye(float scalar) noexcept
	{
		return mat3(
			vec3(scalar, 0, 0),
			vec3(0, scalar, 0),
			vec3(0, 0, scalar)
		);
	}

	constexpr mat3 operator+() const noexcept { return *this; }

	constexpr mat3 operator-() const noexcept { return mat3(-cols[0], -cols[1], -cols[2]); }

	constexpr mat3& operator+=(const mat3& rhs) noexcept
	{
		cols[0] += rhs.cols[0];
		cols[1] += rhs.cols[1];
		cols[2] += rhs.cols[2];
		return *this;
	}

	constexpr mat3& operator-=(const mat3& rhs) noexcept
	{
		cols[0] -= rhs.cols[0];
		cols[1] -= rhs.cols[1];
		cols[2] -= rhs.cols[2];
		return *this;
	}

	constexpr mat3& operator*=(float scalar) noexcept
	{
		cols[0] *= scalar;
		cols[1] *= scalar;
		cols[2] *= scalar;
		return *this;
	}

	constexpr mat3& operator/=(float scalar) noexcept
	{
		cols[0] /= scalar;
		cols[1] /= scalar;
		cols[2] /= scalar;
		return *this;
	}

	constexpr mat3 operator+(const mat3& rhs) const noexcept
	{
		return mat3(
			cols[0] + rhs.cols[0],
			cols[1] + rhs.cols[1],
			cols[2] + rhs.cols[2]
		);
	}

	constexpr mat3 operator-(const mat3& rhs) const noexcept
	{
		return mat3(
			cols[0] - rhs.cols[0],
			cols[1] - rhs.cols[1],
			cols[2] - rhs.cols[2]
		);
	}

	constexpr mat3 operator*(const mat3& rhs) const noexcept
	{
		return mat3(
			operator*(rhs.cols[0]),
			operator*(rhs.cols[1]),
			operator*(rhs.cols[2])
		);
	}

	constexpr vec3 operator*(const vec3& v) const noexcept
	{
		return vec3(
			cols[0].x * v.x + cols[1].x * v.y + cols[2].x * v.z,
			cols[0].y * v.x + cols[1].y * v.y + cols[2].y * v.z,
			cols[0].z * v.x + cols[1].z * v.y + cols[2].z * v.z
		);
	}

	constexpr mat3 operator*(float scalar) const noexcept
	{
		return mat3(
			cols[0] * scalar,
			cols[1] * scalar,
			cols[2] * scalar
		);
	}

	constexpr mat3 operator/(float scalar) const noexcept
	{
		return mat3(
			cols[0] / scalar,
			cols[1] / scalar,
			cols[2] / scalar
		);
	}

	constexpr mat3 transposed() const noexcept
	{
		return mat3(
			vec3(cols[0].x, cols[1].x, cols[2].x),
			vec3(cols[0].y, cols[1].y, cols[2].y),
			vec3(cols[0].z, cols[1].z, cols[2].z)
		);
	}

	constexpr void transpose() noexcept
	{
		std::swap(cols[0].get<1>(), cols[1].get<0>());
		std::swap(cols[0].get<2>(), cols[2].get<0>());
		std::swap(cols[2].get<1>(), cols[1].get<2>());
	}

	constexpr mat3 inverse() const noexcept
	{
		mat3 adj = mat3(
			vec3(
				cols[1].get<1>() * cols[2].get<2>() - cols[1].get<2>() * cols[2].get<1>(),
				cols[2].get<1>() * cols[0].get<2>() - cols[2].get<2>() * cols[0].get<1>(),
				cols[0].get<1>() * cols[1].get<2>() - cols[0].get<2>() * cols[1].get<1>()
			),
			vec3(
				cols[1].get<2>() * cols[2].get<0>() - cols[1].get<0>() * cols[2].get<2>(),
				cols[2].get<2>() * cols[0].get<0>() - cols[2].get<0>() * cols[0].get<2>(),
				cols[0].get<2>() * cols[1].get<0>() - cols[0].get<0>() * cols[1].get<2>()
			),
			vec3(
				cols[1].get<0>() * cols[2].get<1>() - cols[1].get<1>() * cols[2].get<0>(),
				cols[2].get<0>() * cols[0].get<1>() - cols[2].get<1>() * cols[0].get<0>(),
				cols[0].get<0>() * cols[1].get<1>() - cols[0].get<1>() * cols[1].get<0>()
			)
		);
		float det = vec3(cols[0].x, cols[1].x, cols[2].x).dot_product(adj.cols[0]);
		return adj / det;
	}

	vec3 cols[3];
};