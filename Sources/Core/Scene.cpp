#include "pch.h"
#include "Scene.h"

std::string Scene::GetName()
{
	return _name;
}

void Scene::SetName(const std::string& name_)
{
	_name = name_;
}
