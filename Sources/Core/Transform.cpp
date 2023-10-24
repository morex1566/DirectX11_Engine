#include "PCH.h"
#include "Transform.h"

Transform::Transform(const GameObject* owner_)
	: Component(owner_), _position(), _rotation(), _scale(), _up(), _lookAt()
{
}

Transform::~Transform()
{
}

XMFLOAT3 Transform::GetPosition()
{
	return _position;
}

XMFLOAT3 Transform::GetRotation()
{
	return _rotation;
}

XMFLOAT3 Transform::GetScale()
{
	return _scale;
}
