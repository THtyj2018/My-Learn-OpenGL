#pragma once

#include <type_traits>

template <typename T, typename U>
concept SameAs = std::is_same_v<T, U> && std::is_same_v<U, T>;

template <typename T, typename U, typename... Args>
concept AnyOf = (SameAs<T, U> || ... || SameAs<T, Args>);

template <typename T>
concept Integer = std::is_integral_v<T>;

template <typename T>
concept Floating = std::is_floating_point_v<T>;

template <typename T>
concept Number = Integer<T> || Floating<T>;

template <typename T>
concept POD = std::is_trivially_copyable_v<T> && std::is_standard_layout_v<T>;

template <typename T, typename... Args>
concept ConstructibleFrom = std::is_constructible_v<T, Args...>;

template <typename T>
concept DefaultConstructible = std::is_default_constructible_v<T> && std::is_destructible_v<T>;

template <typename T>
concept Copyable = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;

template <typename T, typename U>
concept DerivedFrom = std::is_base_of_v<U, T> && std::is_convertible_v<T*, U*>;

template <typename T>
concept BoundedArray = std::is_bounded_array_v<T>;

template <typename T>
concept UnboundedArray = std::is_unbounded_array_v<T>;