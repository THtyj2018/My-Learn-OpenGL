#pragma once

#include "../Utilities/Traits.h"
#include "../Utilities/String.h"
#include "../Utilities/Array.h"
#include "../Utilities/Pointer.h"

#include <typeinfo>

class Component;
class Transform;

template <typename T>
concept ComponentType = (DerivedFrom<T, Component> && !SameAs<T, Component>);

class Node final
{
	friend class Transform;

public:
	explicit Node(AnsiStringView name = "");

	Node(const Node&) = delete;

	Node& operator=(const Node&) = delete;

	~Node();

	bool SetName(AnsiStringView name);

	const AnsiString& GetName() const noexcept { return m_Name; }

	Node* GetParent() const noexcept { return m_Parent; }

	bool HasChild(AnsiStringView name) const noexcept;

	bool HasChild(const Node* node) const noexcept;

	bool IsAncestorOf(const Node* node) const noexcept;

	bool TestAppendChild(const Node* node) const noexcept;

	bool AppendChild(Node* node);

	Node* CreateChild(AnsiStringView name);

	Node* ReleaseChild(Node* node);

	Node* GetChild(AnsiStringView name) const noexcept;

	template <ComponentType T>
	bool HasComponent() const noexcept
	{
		for (const auto& p : m_Components)
		{
			if (typeid(*p) == typeid(T))
				return true;
		}
		return false;
	}

	template <ComponentType T>
	T* GetOrCreateComponnet()
	{
		if (HasComponent<T>())
			return nullptr;
		return m_Components.emplace_back(new T(this)).get();
	}

	template <ComponentType T>
	T* GetComponent() const noexcept
	{
		for (const auto& p : m_Components)
		{
			if (typeid(*p) == typeid(T))
				return static_cast<T*>(p.get());
		}
		return nullptr;
	}

	template <ComponentType T>
	void RemoveComponent()
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
		{
			if (typeid(*(it->get())) == typeid(T))
			{
				m_Components.erase(it);
				return;
			}
		}
	}

	Transform* GetTransform() const noexcept;

private:
	AnsiString m_Name;
	Node* m_Parent;
	Array<UniquePtr<Node>> m_Children;
	Array<UniquePtr<Component>> m_Components;
};