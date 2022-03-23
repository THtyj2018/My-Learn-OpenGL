#pragma once

#include "vec4.h"
#include "mat3.h"

/// @brief 4x4 matrix
class mat4
{
public:
	constexpr mat4() noexcept
	{
		cols[0] = vec4(1, 0, 0, 0);
		cols[1] = vec4(0, 1, 0, 0);
		cols[2] = vec4(0, 0, 1, 0);
		cols[3] = vec4(0, 0, 0, 1);
	}

	constexpr mat4(const vec4& col0, const vec4& col1, const vec4& col2, const vec4& col3) noexcept
	{
		cols[0] = col0;
		cols[1] = col1;
		cols[2] = col2;
		cols[3] = col3;
	}

	mat4(const vec3& position, const vec3& look_at, const vec3& up) noexcept
	{
		vec3 const f((look_at - position).normalized());
		vec3 const s(f.cross_product(up).normalized());
		vec3 const u(s.cross_product(f));

		cols[0].x = s.x;
		cols[1].x = s.y;
		cols[2].x = s.z;
		cols[0].y = u.x;
		cols[1].y = u.y;
		cols[2].y = u.z;
		cols[0].z = -f.x;
		cols[1].z = -f.y;
		cols[2].z = -f.z;
		cols[3] = vec4(-s.dot_product(position), -u.dot_product(position), f.dot_product(position), 1.0f);
	}

	constexpr mat4(const vec3& position, const vec3& scales, const Quaternion& rotation) noexcept
	{
		mat3 rot = mat3(rotation);
		cols[0] * vec4(rot.cols[0] * scales, 0);
		cols[1] * vec4(rot.cols[1] * scales, 0);
		cols[2] * vec4(rot.cols[2] * scales, 0);
		cols[3] = vec4(position, 1);
	}

	constexpr explicit mat4(const mat3& submat, const vec4& col3 = vec4(0, 0, 0, 1)) noexcept
	{
		cols[0] = vec4(submat.cols[0], 0);
		cols[1] = vec4(submat.cols[1], 0);
		cols[2] = vec4(submat.cols[2], 0);
		cols[3] = col3;
	}

	constexpr explicit mat4(const Quaternion& quat) noexcept : mat4(mat3(quat)) {}

	constexpr mat4(const mat4&) = default;

	constexpr explicit operator mat3() const noexcept
	{
		return mat3(
			cols[0]._vec3,
			cols[1]._vec3,
			cols[2]._vec3
		);
	}

	operator const float* () const noexcept { return cols[0]; }
	operator float* () noexcept { return cols[0]; }

	constexpr static mat4 identity() noexcept { return mat4(); }

	constexpr static mat4 zero() noexcept
	{
		return mat4(vec4::zero(), vec4::zero(), vec4::zero(), vec4::zero());
	}

	constexpr static mat4 eye(float e) noexcept
	{
		return mat4(
			vec4(e, 0, 0, 0),
			vec4(0, e, 0, 0),
			vec4(0, 0, e, 0),
			vec4(0, 0, 0, e)
		);
	}

	constexpr bool operator==(const mat4&) const noexcept = default;

	constexpr bool eqzero() const noexcept
	{
		return cols[0].eqzero() && cols[1].eqzero() && cols[2].eqzero() && cols[3].eqzero();
	}

	constexpr mat4 operator+() const noexcept { return *this; }
	constexpr mat4 operator-() const noexcept { return mat4(-cols[0], -cols[1], -cols[2], -cols[3]); }

	constexpr mat4& operator+=(const mat4& rhs) noexcept
	{
		cols[0] += rhs.cols[0];
		cols[1] += rhs.cols[1];
		cols[2] += rhs.cols[2];
		cols[3] += rhs.cols[3];
		return *this;
	}

	constexpr mat4& operator-=(const mat4& rhs) noexcept
	{
		cols[0] -= rhs.cols[0];
		cols[1] -= rhs.cols[1];
		cols[2] -= rhs.cols[2];
		cols[3] -= rhs.cols[3];
		return *this;
	}

	constexpr mat4& operator*=(float scalar) noexcept
	{
		cols[0] *= scalar;
		cols[1] *= scalar;
		cols[2] *= scalar;
		cols[3] *= scalar;
		return *this;
	}

	constexpr mat4& operator/=(float scalar) noexcept
	{
		cols[0] /= scalar;
		cols[1] /= scalar;
		cols[2] /= scalar;
		cols[3] /= scalar;
		return *this;
	}

	constexpr mat4 operator+(const mat4& rhs) const noexcept
	{
		return mat4(
			cols[0] + rhs.cols[0],
			cols[1] + rhs.cols[1],
			cols[2] + rhs.cols[2],
			cols[3] + rhs.cols[3]
		);
	}

	constexpr mat4 operator-(const mat4& rhs) const noexcept
	{
		return mat4(
			cols[0] - rhs.cols[0],
			cols[1] - rhs.cols[1],
			cols[2] - rhs.cols[2],
			cols[3] - rhs.cols[3]
		);
	}

	constexpr mat4 operator*(const mat4& rhs) const noexcept
	{
		return mat4(
			operator*(rhs.cols[0]),
			operator*(rhs.cols[1]),
			operator*(rhs.cols[2]),
			operator*(rhs.cols[3])
		);
	}

	constexpr vec4 operator*(const vec4& v) const noexcept
	{
		return vec4(
			cols[0].x * v.x + cols[1].x * v.y + cols[2].x * v.z + cols[3].x * v.w,
			cols[0].y * v.x + cols[1].y * v.y + cols[2].y * v.z + cols[3].y * v.w,
			cols[0].z * v.x + cols[1].z * v.y + cols[2].z * v.z + cols[3].z * v.w,
			cols[0].w * v.x + cols[1].w * v.y + cols[2].w * v.z + cols[3].w * v.w
		);
	}

	constexpr mat4 operator*(float scalar) const noexcept
	{
		return mat4(
			cols[0] * scalar,
			cols[1] * scalar,
			cols[2] * scalar,
			cols[3] * scalar
		);
	}

	constexpr mat4 operator/(float scalar) const noexcept
	{
		return mat4(
			cols[0] * scalar,
			cols[1] * scalar,
			cols[2] * scalar,
			cols[3] * scalar
		);
	}

	constexpr mat4 transposed() const noexcept
	{
		return mat4(
			vec4(cols[0].x, cols[1].x, cols[2].x, cols[3].x),
			vec4(cols[0].y, cols[1].y, cols[2].y, cols[3].y),
			vec4(cols[0].z, cols[1].z, cols[2].z, cols[3].z),
			vec4(cols[0].w, cols[1].w, cols[2].w, cols[3].w)
		);
	}

	constexpr void transpose() noexcept
	{
		std::swap(cols[0].get<1>(), cols[1].get<0>());
		std::swap(cols[0].get<2>(), cols[2].get<0>());
		std::swap(cols[0].get<3>(), cols[3].get<0>());
		std::swap(cols[1].get<2>(), cols[2].get<1>());
		std::swap(cols[1].get<3>(), cols[3].get<1>());
		std::swap(cols[2].get<3>(), cols[3].get<2>());
	}

	constexpr mat4 inverse() const noexcept
	{
		mat2 A = mat2(cols[0].x, cols[0].y, cols[1].x, cols[1].y).inverse();
		mat2 B = mat2(cols[2].x, cols[2].y, cols[3].x, cols[3].y);
		mat2 C = mat2(cols[0].z, cols[0].w, cols[1].z, cols[1].w);
		mat2 D = mat2(cols[2].z, cols[2].w, cols[3].z, cols[3].w);

		mat2 AB = A * B;
		mat2 CA = C * A;
		D = (D - C * AB).inverse();
		C = -D * CA;
		mat2 ABD = AB * D;
		B = -ABD;
		A += ABD * CA;

		return mat4(
			vec4(A.cols[0], C.cols[0]),
			vec4(A.cols[1], C.cols[1]),
			vec4(B.cols[0], D.cols[0]),
			vec4(B.cols[1], D.cols[1])
		);
	}

	constexpr void translate(const vec3& v) noexcept
	{
		cols[3] += cols[0] * v.x + cols[1] * v.y + cols[2] * v.z;
	}

	constexpr void scale(const vec3& scales) noexcept
	{
		cols[0] *= scales.x;
		cols[1] *= scales.y;
		cols[2] *= scales.z;
	}

	constexpr vec3 get_translation() const noexcept
	{
		return cols[3]._vec3;
	}

	constexpr vec3 get_scale() const noexcept
	{
		return vec3(
			cols[0].length(),
			cols[1].length(),
			cols[2].length()
		);
	}

	Quaternion get_rotation() const noexcept
	{
		mat3 rot = mat3(*this);
		rot.cols[0].normalize();
		rot.cols[1].normalize();
		rot.cols[2].normalize();
		float tr = rot.cols[0].get<0>() + rot.cols[1].get<1>() + rot.cols[2].get<2>();
		vec4 rotation;
		if (Mathf::fuzzy_zero(1 + tr))
		{
			float m = Mathf::max(rot.cols[0].get<0>(), rot.cols[1].get<1>(), rot.cols[2].get<2>());
			float t = Mathf::sqrt(1 - tr + 2 * m);
			if (m == rot.cols[0].get<0>())
			{
				rotation.w = (rot.cols[1].get<2>() - rot.cols[2].get<1>()) / t;
				rotation.x = t / 4;
				rotation.y = (rot.cols[2].get<0>() + rot.cols[0].get<2>()) / t;
				rotation.z = (rot.cols[0].get<1>() + rot.cols[1].get<0>()) / t;
			}
			else if (m == rot.cols[1].get<1>())
			{
				rotation.w = (rot.cols[2].get<0>() - rot.cols[0].get<2>()) / t;
				rotation.x = (rot.cols[0].get<1>() + rot.cols[1].get<0>()) / t;
				rotation.y = t / 4;
				rotation.z = (rot.cols[1].get<2>() + rot.cols[2].get<1>()) / t;
			}
			else
			{
				rotation.w = (rot.cols[0].get<1>() - rot.cols[1].get<0>()) / t;
				rotation.x = (rot.cols[2].get<0>() + rot.cols[0].get<2>()) / t;
				rotation.y = -(rot.cols[1].get<2>() - rot.cols[2].get<1>()) / t;
				rotation.z = t / 4;
			}
		}
		else
		{
			rotation.w = Mathf::sqrt(1 + tr) * 0.5f;
			rotation.x = (rot.cols[1].get<2>() - rot.cols[2].get<1>()) / rotation.w * 0.25f;
			rotation.y = (rot.cols[2].get<0>() - rot.cols[0].get<2>()) / rotation.w * 0.25f;
			rotation.z = (rot.cols[0].get<1>() - rot.cols[1].get<0>()) / rotation.w * 0.25f;
		}
		return *reinterpret_cast<Quaternion*>(&rotation);
	}

	static mat4 random() noexcept
	{
		mat4 ret;
		float* addr = &ret.cols[0].x;
		for (size_t i = 0; i < 16; ++i)
			*(addr + i) = Mathf::random();
		return ret;
	}

	vec4 cols[4];
};