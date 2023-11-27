#include "PCH.h"
#include "SInput.h"

uint8				SInput::Keys[256];
std::queue<uint8>   SInput::History;

SInput& SInput::GetInstance()
{
	static SInput Instance;
	return Instance;
}

void SInput::Initialize()
{
	for (int i = 0; i < 256; i++)
	{
		Keys[i] = 0;
	}
}

void SInput::Release()
{
}

void SInput::Start()
{
}

void SInput::Tick()
{
}

void SInput::End()
{
}

void SInput::Clear()
{
	while (!History.empty())
	{
		uint8 Key = History.back();
		History.pop();

		Keys[Key] = 0;
	}
}
