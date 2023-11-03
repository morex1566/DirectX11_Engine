#include "PCH.h"
#include "GCamera.h"
#include "GTestObject.h"
#include "OApplication.h"

uint8							OApplication::bIsPlaying = 1;

OApplication::~OApplication()
{
}

OApplication::OApplication()
{
}

OApplication& OApplication::GetInstance()
{
	static OApplication System;
	return System;
}

Object::EHandleResultType OApplication::Initialize()
{
	// Create window.
	{
		Window = std::make_shared<OWindow>();

		if (Window->Initialize() != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}
		else
		{
			Objects.push_back(Window);
		}
	}

	// Create directx.
	{
		DirectX11 = std::make_shared<ODirectX11>(*Window);

		if (DirectX11->Initialize() != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}
		else
		{
			Objects.push_back(DirectX11);
		}
	}

	// Create world.
	{
		std::shared_ptr<OWorld> World = std::make_shared<OWorld>();

		if (World->Initialize() != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}
		else
		{
			Objects.push_back(World);
		}
	}

	// Create viewport camera.
	{
		Camera = std::make_shared<GCamera>(*Window);

		if (Camera->Initialize() != EHandleResultType::Success)
		{
			return EHandleResultType::Failed;
		}

		Camera->SetPosition(XMFLOAT3(0, 0, -5));
	}

	// Create test Object.
	{
		TestObject = std::make_shared<GTestObject>(*DirectX11);
		TestObject->Initialize();
	}

	// Create test shader.
	{
		Shader = std::make_shared<OShader>(*DirectX11, *Window, GET_SHADER_FILE_DIR("UnlitVertexShader.hlsl"), GET_SHADER_FILE_DIR("UnlitPixelShader.hlsl"));
		Shader->Initialize();
	}

	// Create test gui.
	{
		GUI = std::make_shared<OGUI>(*Window, *DirectX11);
		GUI->Initialize();
	}

	return EHandleResultType::Success;
}

void OApplication::Release()
{
	for (const auto& object : Objects)
	{
		object->Release();
	}
}

void OApplication::Start()
{
	for (const auto& object : Objects)
	{
		object->Start();
	}
}

void OApplication::Tick()
{
	for (const auto& object : Objects)
	{
		object->Tick();
	}

	Camera->Tick();

	// Clear before store graphical results.
	{
		DirectX11->ClearRenderTargetView(ODirectX11::ERenderModeType::Model);
		DirectX11->ClearRenderTargetView(ODirectX11::ERenderModeType::Interface);
		DirectX11->ClearDepthStencilView(ODirectX11::ERenderModeType::Model);
		DirectX11->ClearDepthStencilView(ODirectX11::ERenderModeType::Interface);
	}

	// Set as model mode.
	{
		DirectX11->SetDepthStencilState(ODirectX11::ERenderModeType::Model);
		DirectX11->SetRasterizerState(ODirectX11::ERenderModeType::Model);
		DirectX11->SetRenderTargets(ODirectX11::ERenderModeType::Model);
		DirectX11->SetViewport(ODirectX11::ERenderModeType::Model);

		// Render models here.
		{
			TestObject->Render();
			Shader->SetShaderParameters(XMMatrixIdentity(), Camera->GetViewMatrix(), Camera->GetProjectionMatrix());
			Shader->Process(TestObject->GetIndexCount());
		}
	}

	// Set as interface mode.
	{
		DirectX11->SetDepthStencilState(ODirectX11::ERenderModeType::Interface);
		DirectX11->SetRasterizerState(ODirectX11::ERenderModeType::Interface);
		DirectX11->SetRenderTargets(ODirectX11::ERenderModeType::Interface);
		DirectX11->SetViewport(ODirectX11::ERenderModeType::Interface);

		// Render interfaces here.
		{
			GUI->Render();
		}
	}

	DirectX11->Draw();
}

void OApplication::End()
{
	for (const auto& object : Objects)
	{
		object->End();
	}
}

void OApplication::Quit()
{
	bIsPlaying = 0;
}

uint8 OApplication::GetIsPlaying() const
{
	return bIsPlaying;
}

const OWindow& OApplication::GetWindow() const
{
	return *Window;
}
