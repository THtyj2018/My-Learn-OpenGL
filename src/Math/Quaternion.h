#pragma once

#include "vec4.h"

/// @brief Quaternion represents rotation in 3-dimensional space
class Quaternion
{
public:
	constexpr Quaternion() noexcept : v(0, 0, 0, 1) {}

	Quaternion(const vec3& axis, float angle) noexcept :
		v(axis.normalized()* Mathf::sin(angle * 0.5f), Mathf::cos(angle * 0.5f)) {}

	constexpr Quaternion(const Quaternion&) = default;

	constexpr explicit operator vec4() const noexcept { return v; }

	constexpr bool operator==(const Quaternion& rhs) const noexcept { return v == rhs.v; }

	/// @brief Quaternion product
	/// @param rhs Another Quatrenion(vec4)
	/// @return Quaternion
	constexpr Quaternion operator*(const Quaternion& _rhs) const noexcept
	{
		auto [x, y, z, w] = v;
		const auto& rhs = _rhs.v;
		return Quaternion(
			w * rhs.x + x * rhs.w + z * rhs.y - y * rhs.z,
			w * rhs.y + y * rhs.w + x * rhs.z - z * rhs.x,
			w * rhs.z + z * rhs.w + y * rhs.z - x * rhs.z,
			w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z
		);
	}

	constexpr Quaternion conjugation() const noexcept
	{
		return Quaternion(-v.x, -v.y, -v.z, v.w);
	}

	/// @brief Quaternion inverse
	/// @return Inverse
	constexpr Quaternion inverse() const noexcept
	{
		return conjugation();
	}

	constexpr Quaternion delta(const Quaternion& rhs) const noexcept
	{
		return *this * rhs.inverse();
	}

	constexpr float dot_product(const Quaternion& rhs) const noexcept
	{
		return v.dot_product(rhs.v);
	}

	static Quaternion slerp(const Quaternion& lhs, const Quaternion& rhs, float t) noexcept
	{
		float theta = Mathf::acos(lhs.dot_product(rhs));
		float rs = 1 / Mathf::sin(theta);
		float snt = Mathf::sin((1 - t) * theta);
		float st = Mathf::sin(t * theta);
		return Quaternion((snt * lhs.v + st * rhs.v) * rs);
	}

private:
	constexpr Quaternion(float x, float y, float z, float w) noexcept : v(x, y, z, w) {}

	constexpr explicit Quaternion(const vec4& v) noexcept : v(v) {}

	vec4 v;
};