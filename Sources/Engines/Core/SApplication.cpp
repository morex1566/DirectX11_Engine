#include "PCH.h"
#include "SApplication.h"

std::vector<std::shared_ptr<Object>>	SApplication::Objects;
std::shared_ptr<OTime>					SApplication::Time;
std::shared_ptr<OWindow>				SApplication::Window;
std::shared_ptr<ODirectX11>				SApplication::DirectX11;
std::shared_ptr<OInput>					SApplication::Input;
std::shared_ptr<GCamera>				SApplication::Camera;
std::shared_ptr<OGUI>					SApplication::GUI;
std::shared_ptr<OWorld>					SApplication::World;
uint8									SApplication::bIsPlaying = 1;
HINSTANCE								SApplication::HInstance;

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

void SApplication::Init(HINSTANCE InHInstance)
{
	HInstance = InHInstance;

	// Create time.
	{
		Time = std::make_shared<OTime>();
		Objects.push_back(Time);
	}

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

	// Create input.
	{
		Input = std::make_shared<OInput>();
		Objects.push_back(Input);
	}

	// Create viewport camera.
	{
		Camera = std::make_shared<GCamera>(*Window);
		Objects.push_back(Camera);
	}

	// Create world.
	{
		World = std::make_shared<OWorld>();
		Objects.push_back(World);
	}

	// Create test gui.
	{
		GUI = std::make_shared<OGUI>(*Window, *DirectX11, *World);
		Objects.push_back(GUI);
	}

	// Initialize all of object.
	for (const auto& Object : Objects)
	{
		Object->Init();
	}
}

void SApplication::Shutdown()
{
	for (const auto& object : Objects)
	{
		object->Shutdown();
	}
}

void SApplication::Start()
{
	for (const auto& object : Objects)
	{
		object->Start();
	}
}

void SApplication::Tick()
{
	for (const auto& object : Objects)
	{
		object->Tick();
	}
}

void SApplication::End()
{
	for (const auto& object : Objects)
	{
		object->End();
	}
}

void SApplication::Draw()
{
	DirectX11->Draw();
}