#pragma once

class Node;

class Component
{
	friend class Node;

public:
	Component(const Component&) = delete;

	Component& operator=(const Component&) = delete;

	virtual ~Component();

	Node* GetNode() const noexcept { return m_pNode; }

protected:
	Component(Node* node);

private:
	Node* m_pNode;
};