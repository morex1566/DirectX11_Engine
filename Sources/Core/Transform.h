#pragma once
#include "Component.h"

class Transform : public Component, public Management
{
public:
	Transform(const GameObject* owner_);
	Transform(const Transform&)						= delete;
	Transform& operator=(const Transform&)			= delete;
	Transform(Transform&&) noexcept					= delete;
	Transform& operator=(Transform&&) noexcept		= delete;
	~Transform() override;

	XMFLOAT3									GetPosition();
	XMFLOAT3									GetRotation();
	XMFLOAT3									GetScale();

private:
	XMFLOAT3									_position;
	XMFLOAT3									_rotation;
	XMFLOAT3									_scale;
	XMFLOAT3									_up;
	XMFLOAT3									_lookAt;
};