#include "PCH.h"
#include "GameObject.h"

GameObject::~GameObject()
{
}

void GameObject::Start()
{
	for (auto& component : _components)
	{
		component.Start();
	}

	for (auto& child : _children)
	{
		child.Start();
	}
}

void GameObject::Update()
{
	for (auto& component : _components)
	{
		component.Update();
	}

	for (auto& child : _children)
	{
		child.Update();
	}
}

void GameObject::DeleteComponent()
{
}
