#include "PCH.h"
#include "SReflection.h"

SReflection& SReflection::GetInstance()
{
	static SReflection Instance;
	return Instance;
}

void SReflection::Initialize()
{
}
