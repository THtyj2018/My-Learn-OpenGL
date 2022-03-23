#pragma once

#include "..\Utilities\Traits.h"

#include <utility>
#include <random>
#include <limits>
#include <cmath>

class Mathf
{
public:
	static constexpr double PiD = 3.141592653589793;
	static constexpr float Pi = static_cast<float>(PiD);
	static constexpr float Deg2Rad = static_cast<float>(PiD / 180);
	static constexpr float Rad2Deg = static_cast<float>(180 / PiD);

	static constexpr float Epsilon = std::numeric_limits<float>::epsilon();

	template <typename T, size_t N>
	static constexpr size_t array_len(const T(&arr)[N]) noexcept { return N; }

	template <Number T>
	static constexpr T abs(T val) noexcept
	{
		return val > 0 ? val : -val;
	}

	template <Number T>
	static constexpr T min(T lhs, T rhs) noexcept
	{
		return (lhs > rhs) ? rhs : lhs;
	}

	template <Number T>
	static constexpr T min(T a, T b, T c) noexcept
	{
		return min(min(a, b), c);
	}

	template <Number T>
	static constexpr T max(T lhs, T rhs) noexcept
	{
		return (lhs > rhs) ? lhs : rhs;
	}

	template <Number T>
	static constexpr T max(T a, T b, T c) noexcept
	{
		return max(max(a, b), c);
	}

	template <Number T>
	static constexpr T clamp(T val, T left, T right) noexcept
	{
		if (left > right)
			std::swap(left, right);
		return (val < left) ? left :
			(val > right) ? right : val;
	}

	template <Floating T>
	static constexpr T clamp01(T val) noexcept
	{
		return clamp<T>(val, 0, 1);
	}

	static constexpr float lerp(float left, float right, float t) noexcept
	{
		return left + (right - left) * t;
	}

	template <Number T>
	static constexpr T square(T val) noexcept
	{
		return val * val;
	}

	static float sqrt(float val) noexcept
	{
		return ::std::sqrtf(val);
	}

	static float rsqrt(float val) noexcept
	{
		// TODO: intrin rsqrt
		return 1 / sqrt(val);
	}

	static float sin(float rad) noexcept
	{
		return ::std::sinf(rad);
	}

	static float cos(float rad) noexcept
	{
		return ::std::cosf(rad);
	}

	static float tan(float rad) noexcept
	{
		return ::std::tanf(rad);
	}

	static float asin(float val) noexcept
	{
		return ::std::asinf(val);
	}

	static float acos(float val) noexcept
	{
		return ::std::acosf(val);
	}

	static float atan(float val) noexcept
	{
		return ::std::atanf(val);
	}

	static float atan2(float y, float x) noexcept
	{
		return ::std::atan2f(y, x);
	}

	static constexpr bool fuzzy_zero(float val) noexcept
	{
		return Mathf::abs(val) < Epsilon;
	}

	static constexpr bool fuzzy_eq(float x, float y) noexcept
	{
		return Mathf::abs(x - y) * (1 / Epsilon) < Mathf::min(x, y);
	}

	template <Number T, size_t N>
	static constexpr T sum(const T(&arr)[N]) noexcept
	{
		T ret = 0;
		for (size_t i = 0; i < N; ++i)
			ret += arr[i];
		return ret;
	}

	static void srand(unsigned seed) noexcept
	{
		random_engine.seed(seed);
	}

	/// @brief Generate a random float value.
	/// @return A random float value between (0.0f, 1.0f)
	static float random() noexcept
	{
		return float_random_range(random_engine);
	}

	/// @brief Generate a random float value
	/// @param a Random range left
	/// @param b Random range right
	/// @return A random float value between (a, b)
	static float random(float a, float b) noexcept
	{
		return lerp(a, b, random());
	}

private:
	inline static std::default_random_engine random_engine{};
	inline static std::uniform_real_distribution<float> float_random_range{0.0f, 1.0f};
};