#pragma once

#include "Traits.h"

#include <memory>

template <typename T>
using SharedPtr = std::shared_ptr<T>;

template <typename T>
using WeakPtr = std::weak_ptr<T>;

template <typename T>
using UniquePtr = std::unique_ptr<T>;

template <typename T, typename... Args>
requires (ConstructibleFrom<T, Args...> && sizeof...(Args) > 0)
SharedPtr<T> MakeShared(Args&&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template <DefaultConstructible T>
SharedPtr<T> MakeShared()
{
	return std::make_shared<T>();
}

template <UnboundedArray T>
SharedPtr<T> MakeShared(size_t count)
{
	return std::make_shared<T>(count);
}

template <typename T, typename... Args>
requires (ConstructibleFrom<T, Args...> && sizeof...(Args) > 0)
UniquePtr<T> MakeUnique(Args&&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <DefaultConstructible T>
UniquePtr<T> MakeUnique()
{
	return std::make_unique<T>();
}

template <UnboundedArray T>
UniquePtr<T> MakeUnique(size_t count)
{
	return std::make_unique<T>(count);
}