#include "PCH.h"
#include "Cube.h"

Cube::Cube()
	: GameObject()
{
	Component* component = new Component;
	{
		AttachComponent(component);
		DetachComponent(component);
		FindComponent<Component>();
	}
}

void Cube::Start()
{
	GameObject::Start();
}

void Cube::Update()
{
	GameObject::Update();
}

void Cube::Shutdown()
{
	GameObject::Shutdown();
}
