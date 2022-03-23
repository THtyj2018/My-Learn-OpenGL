#pragma once

#include "Traits.h"

#include <unordered_map>

template <typename T>
concept Hashable = requires (const T & t) {
	{std::hash<T>{}(t)} -> SameAs<size_t>;
};

template <Hashable K, typename V>
using HashMap = std::unordered_map<K, V>;