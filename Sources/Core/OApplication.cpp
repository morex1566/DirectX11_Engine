#include "PCH.h"
#include "OApplication.h"
#include "WHierarchy.h"
#include "WTools.h"

uint8							OApplication::bIsPlaying = 1;

OApplication::~OApplication()
{
}

OApplication::OApplication()
	: Object()
{
}

OApplication& OApplication::GetInstance()
{
	static OApplication System;
	return System;
}

void OApplication::Quit()
{
	bIsPlaying = 0;
}

uint8 OApplication::CheckIsPlaying()
{
	return bIsPlaying;
}

Object::EHandleResultType OApplication::Initialize()
{
	Object::Initialize();

	// Create window.
	{
		Window = std::make_shared<OWindow>();
		Objects.push_back(Window);
	}

	// Create directx.
	{
		DirectX11 = std::make_shared<ODirectX11>(*Window);
		Objects.push_back(DirectX11);
	}

	// Create viewport camera.
	{
		Camera = std::make_shared<GCamera>(*Window);
		Objects.push_back(Camera);
	}

	// Create world.
	{
		World = std::make_shared<OWorld>(*Window, *DirectX11, *Camera);
		Objects.push_back(World);
	}

	// Create test gui.
	{
		GUI = std::make_shared<OGUI>(*Window, *DirectX11);
		Objects.push_back(GUI);
		GUI->TAddWidget<WHierarchy>();
		GUI->TAddWidget<WTools>();
	}

	// Initialize all of object.
	for (const auto& Object : Objects)
	{
		Object->Initialize();
	}

	return EHandleResultType::Success;
}

void OApplication::Release()
{
	Object::Release();
	
	for (const auto& object : Objects)
	{
		object->Release();
	}
}

void OApplication::Start()
{
	Object::Start();

	for (const auto& object : Objects)
	{
		if (object->CheckIsEnabled())
		{
			object->Start();
		}
	}
}

void OApplication::Tick()
{
	Object::Tick();

	for (const auto& object : Objects)
	{
		if (object->CheckIsEnabled())
		{
			object->Tick();
		}
	}
}

void OApplication::End()
{
	Object::End();

	for (const auto& object : Objects)
	{
		if (object->CheckIsEnabled())
		{
			object->End();
		}
	}
}

void OApplication::Draw()
{
	DirectX11->Draw();
}