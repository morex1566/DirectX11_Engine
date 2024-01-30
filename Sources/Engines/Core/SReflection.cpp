#include "PCH.h"
#include "SConsole.h"
#include "SReflection.h"

std::map<size_t, std::vector<void*>>				SReflection::ClassInfos;


SReflection& SReflection::GetInstance()
{
	static SReflection Instance;
	return Instance;
}

void SReflection::Init()
{

}

