#include "PCH.h"
#include "GCamera.h"
#include "GTestObject.h"
#include "SConsole.h"
#include "ODirectX11.h"
#include "OWindow.h"

GTestObject::GTestObject()
	: OGameObject()
{
}

GTestObject::~GTestObject()
{
}

Object::EHandleResultType GTestObject::Initialize()
{
	return OGameObject::Initialize();
}

void GTestObject::Release()
{
	OGameObject::Release();
}

void GTestObject::Start()
{
	OGameObject::Start();
}

void GTestObject::Tick()
{
	OGameObject::Tick();
}

void GTestObject::End()
{
	OGameObject::End();
}
