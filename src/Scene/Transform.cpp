#include "Transform.h"
#include "Node.h"

#include "..\Math\mat3.h"

Transform::Transform(Node* node) :
	Component(node), m_Scale(1, 1, 1), m_WorldScale(1, 1, 1), m_bDirty(false)
{
}

Transform::~Transform()
{
}

const vec3& Transform::GetPosition() const noexcept
{
	return m_Position;
}

const vec3& Transform::GetWorldPosition() const noexcept
{
	if (m_bDirty)
		UpdateWorldTransform();
	return m_WorldPosition;
}

const vec3& Transform::GetScale() const noexcept
{
	return m_Scale;
}

const vec3& Transform::GetWorldScale() const noexcept
{
	if (m_bDirty)
		UpdateWorldTransform();
	return m_WorldScale;
}

const Quaternion& Transform::GetRotation() const noexcept
{
	return m_Rotation;
}

const Quaternion& Transform::GetWorldRotation() const noexcept
{
	if (m_bDirty)
		UpdateWorldTransform();
	return m_WorldRotation;
}

void Transform::SetPosition(const vec3& position) noexcept
{
	m_Position = position;
	MarkDirty();
}

void Transform::SetWorldPosition(const vec3& position) noexcept
{
	if (Node* parent = GetNode()->GetParent(); parent)
		SetPosition(parent->GetTransform()->WorldToLocal(position));
	else
		SetPosition(position);
}

void Transform::SetScale(const vec3& scales) noexcept
{
	m_Scale = scales;
	MarkDirty();
}

void Transform::SetWorldScale(const vec3& scales) noexcept
{
	if (Node* parent = GetNode()->GetParent(); parent)
		SetScale(scales / parent->GetTransform()->GetWorldScale());
	else
		SetScale(scales);
}

void Transform::SetRotation(const Quaternion& rotation) noexcept
{
	m_Rotation = rotation;
	MarkDirty();
}

void Transform::SetWorldRotation(const Quaternion& rotation) noexcept
{
	if (Node* parent = GetNode()->GetParent(); parent)
		SetRotation(parent->GetTransform()->GetWorldRotation().inverse() * rotation);
	else
		SetRotation(rotation);
}

void Transform::Translate(const vec3& translation, TransformSpace space) noexcept
{
	vec3 _trans = translation;
	if (space == TransformSpace::World)
	{
		if (Node* parent = GetNode()->GetParent(); parent)
			_trans = parent->GetTransform()->WorldToLocal(translation);
	}
	else if (space == TransformSpace::Self)
	{
		_trans = LocalToParent(translation);
	}
	m_Position += _trans;
	MarkDirty();
}

void Transform::Scale(const vec3& scales, TransformSpace space) noexcept
{
	vec3 _scales = scales;
	if (space == TransformSpace::World)
	{
		if (Node* parent = GetNode()->GetParent(); parent)
			_scales = scales / parent->GetTransform()->GetWorldScale();
	}
	m_Scale *= _scales;
	MarkDirty();
}

void Transform::Rotate(const Quaternion& rotation, TransformSpace space) noexcept
{
	Quaternion rot = rotation;
	if (space == TransformSpace::World)
	{
		if (Node* parent = GetNode()->GetParent(); parent)
			rot = parent->GetTransform()->GetWorldRotation().inverse() * rotation;
	}
	else if (space == TransformSpace::Self)
	{
		rot = m_Rotation * rotation;
	}
	m_Rotation = rot * m_Rotation;
	MarkDirty();
}

vec3 Transform::LocalToParent(const vec3& v) const noexcept
{
	return m_Position + m_Scale * (mat3(m_Rotation) * v);
}

vec3 Transform::ParentToLocal(const vec3& v) const noexcept
{
	return mat3(m_Rotation.inverse()) * ((v - m_Position) / m_Scale);
}

vec3 Transform::LocalToWorld(const vec3& v) const noexcept
{
	if (m_bDirty)
		UpdateWorldTransform();
	return m_WorldPosition + m_WorldScale * (mat3(m_WorldRotation) * v);
}

vec3 Transform::WorldToLocal(const vec3& v) const noexcept
{
	if (m_bDirty)
		UpdateWorldTransform();
	return mat3(m_WorldRotation.inverse()) * ((v - m_WorldPosition) / m_WorldScale);
}

void Transform::MarkDirty() const noexcept
{
	if (m_bDirty)
		return;
	m_bDirty = true;
	for (const auto& p : GetNode()->m_Children)
		p->GetTransform()->MarkDirty();
}

void Transform::UpdateWorldTransform() const noexcept
{
	Node* parent = GetNode()->GetParent();
	if (parent)
	{
		Transform* pt = parent->GetTransform();
		pt->UpdateWorldTransform();
		m_WorldPosition = pt->m_WorldPosition + pt->LocalToWorld(m_Position);
		m_WorldScale = pt->m_WorldScale * m_Scale;
		m_WorldRotation = pt->m_WorldRotation * m_Rotation;
	}
	else
	{
		m_WorldPosition = m_Position;
		m_WorldScale = m_Scale;
		m_WorldRotation = m_Rotation;
	}
	m_bDirty = false;
}