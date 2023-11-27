#include "PCH.h"
#include "OTime.h"

float				OTime::DeltaTime;

OTime::OTime()
	: Object()
{
}

OTime::~OTime()
{
}

void OTime::Initialize()
{
	Object::Initialize();

	DeltaTime = 0.0f;
	PrevTime = std::chrono::high_resolution_clock::now();
}

void OTime::Release()
{
	Object::Release();
}

void OTime::Start()
{
	Object::Start();
}

void OTime::Tick()
{
	Object::Tick();

	// Get delta time.
	auto CurrTime = std::chrono::high_resolution_clock::now();
	{
		DeltaTime = std::chrono::duration<float>(CurrTime - PrevTime).count();
		PrevTime = CurrTime;
	}
}
