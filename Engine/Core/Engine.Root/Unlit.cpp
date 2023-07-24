#include "pch.h"
#include "Unlit.h"

Unlit::Unlit(const std::string& vsPath_, const std::string& psPath_)
	: Shader(std::move(vsPath_), std::move(psPath_))
{
	
}

Unlit::~Unlit()
{
}

void Unlit::Destroy()
{
}
