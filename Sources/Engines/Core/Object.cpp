#include "PCH.h"
#include "Object.h"
#include "SConsole.h"

Object::Object(): bIsEnabled(1)
{
}

uint8 Object::CheckIsEnabled() const
{
	return bIsEnabled;
}

std::wstring Object::GetName() const
{
	return Name;
}

void Object::SetIsEnabled(uint8 InOnOff)
{
	bIsEnabled = InOnOff;
}

void Object::SetName(const std::wstring& InName)
{
	Name = InName;
}