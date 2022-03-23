#include "Node.h"
#include "Component.h"
#include "Transform.h"

Node::Node(AnsiStringView name) :
	m_Name(name), m_Parent(nullptr)
{
	m_Components.emplace_back(new Transform(this));
}

Node::~Node()
{
}

bool Node::SetName(AnsiStringView name)
{
	if (m_Parent)
	{
		if (m_Parent->HasChild(name))
			return false;
	}
	m_Name = name;
	return true;
}

bool Node::HasChild(AnsiStringView name) const noexcept
{
	for (const auto& p : m_Children)
	{
		if (p->m_Name == name)
			return true;
	}
	return false;
}

bool Node::HasChild(const Node* node) const noexcept
{
	for (const auto& p : m_Children)
	{
		if (p.get() == node)
			return true;
	}
	return false;
}

bool Node::IsAncestorOf(const Node* node) const noexcept
{
	if (node == nullptr || node == this)
		return false;
	Node* cur = node->m_Parent;
	while (cur)
	{
		if (cur == this)
			return true;
		cur = cur->m_Parent;
	}
	return false;
}

bool Node::TestAppendChild(const Node* node) const noexcept
{
	if (node == nullptr || node == this || node->IsAncestorOf(this) ||
		HasChild(node) || HasChild(node->m_Name))
		return false;
	return true;
}

bool Node::AppendChild(Node* node)
{
	if (TestAppendChild(node))
	{
		if (node->m_Parent)
			node->m_Parent->ReleaseChild(node);
		else
			node->GetTransform()->MarkDirty();
		m_Children.push_back(UniquePtr<Node>(node));
		node->m_Parent = this;
		return true;
	}
	return false;
}

Node* Node::CreateChild(AnsiStringView name)
{
	if (HasChild(name))
		return nullptr;
	Node* node = new Node(name);
	m_Children.emplace_back(node);
	node->GetTransform()->MarkDirty();
	return node;
}

Node* Node::ReleaseChild(Node* node)
{
	if (node == nullptr)
		return nullptr;
	auto it = m_Children.begin();
	for (; it != m_Children.end(); ++it)
	{
		if (it->get() == node)
			break;
	}
	if (it != m_Children.end())
	{
		Node* _node = it->release();
		m_Children.erase(it);
		_node->GetTransform()->MarkDirty();
		return _node;
	}
	return nullptr;
}

Node* Node::GetChild(AnsiStringView name) const noexcept
{
	for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		if (it->get()->m_Name == name)
			return it->get();
	}
	return nullptr;
}

Transform* Node::GetTransform() const noexcept
{
	return static_cast<Transform*>(m_Components[0].get());
}
