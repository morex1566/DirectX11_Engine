#include "PCH.h"
#include "SApplication.h"
#include "WContentBrowser.h"
#include "WHierarchy.h"
#include "WInspector.h"
#include "WTools.h"

uint8							SApplication::bIsPlaying = 1;

SApplication& SApplication::GetInstance()
{
	static SApplication Instance;
	return Instance;
}

void SApplication::Quit()
{
	bIsPlaying = 0;
}

uint8 SApplication::CheckIsPlaying()
{
	return bIsPlaying;
}

void SApplication::Initialize()
{
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
		WInspector* Inspector = GUI->TAddWidget<WInspector>(*World);
		WHierarchy* Hierarchy = GUI->TAddWidget<WHierarchy>(*World, Inspector);
		WTools* Tools = GUI->TAddWidget<WTools>(*World);
		WContentBrowser* ContentBrowser = GUI->TAddWidget<WContentBrowser>(*World);
	}

	// Create gizmo.
	

	// Initialize all of object.
	for (const auto& Object : Objects)
	{
		Object->Initialize();
	}
}

void SApplication::Release()
{
	for (const auto& object : Objects)
	{
		object->Release();
	}
}

void SApplication::Start()
{
	for (const auto& object : Objects)
	{
		if (object->CheckIsEnabled())
		{
			object->Start();
		}
	}
}

void SApplication::Tick()
{
	for (const auto& object : Objects)
	{
		if (object->CheckIsEnabled())
		{
			object->Tick();
		}
	}
}

void SApplication::End()
{
	for (const auto& object : Objects)
	{
		if (object->CheckIsEnabled())
		{
			object->End();
		}
	}
}

void SApplication::Draw()
{
	DirectX11->Draw();
}