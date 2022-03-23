#pragma once

#include "Component.h"
#include "../Math/Quaternion.h"

enum class TransformSpace
{
	Parent,
	World,
	Self
};

class Transform final : public Component
{
	friend class Node;

public:
	~Transform() override;

	const vec3& GetPosition() const noexcept;

	const vec3& GetWorldPosition() const noexcept;

	const vec3& GetScale() const noexcept;

	const vec3& GetWorldScale() const noexcept;

	const Quaternion& GetRotation() const noexcept;

	const Quaternion& GetWorldRotation() const noexcept;

	void SetPosition(const vec3& position) noexcept;

	void SetWorldPosition(const vec3& position) noexcept;

	void SetScale(const vec3& scales) noexcept;

	void SetWorldScale(const vec3& scales) noexcept;

	void SetRotation(const Quaternion& rotation) noexcept;

	void SetWorldRotation(const Quaternion& rotation) noexcept;

	void Translate(const vec3& translation, TransformSpace space = TransformSpace::Parent) noexcept;

	void Scale(const vec3& scales, TransformSpace space = TransformSpace::Parent) noexcept;

	void Rotate(const Quaternion& rotation, TransformSpace space = TransformSpace::Parent) noexcept;

	vec3 LocalToParent(const vec3& v) const noexcept;

	vec3 ParentToLocal(const vec3& v) const noexcept;

	vec3 LocalToWorld(const vec3& v) const noexcept;

	vec3 WorldToLocal(const vec3& v) const noexcept;

private:
	Transform(Node* node);

	void MarkDirty() const noexcept;

	void UpdateWorldTransform() const noexcept;

	vec3 m_Position;
	vec3 m_Scale;
	Quaternion m_Rotation;
	mutable vec3 m_WorldPosition;
	mutable vec3 m_WorldScale;
	mutable Quaternion m_WorldRotation;
	mutable bool m_bDirty;
};