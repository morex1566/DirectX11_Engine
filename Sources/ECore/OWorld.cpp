#include "PCH.h"
#include "CMesh.h"
#include "CShader.h"
#include "GCamera.h"
#include "GGizmo.h"
#include "GTestObject.h"
#include "ODirectX11.h"
#include "OWindow.h"
#include "OWorld.h"

OWorld::OWorld(const OWindow& InWindow, const ODirectX11& InDirectX11, const GCamera& InCamera)
	: Object()
{
	Window = &InWindow;
	DirectX11 = &InDirectX11;
	Camera = &InCamera;
}

OWorld::~OWorld()
{
}

void OWorld::Initialize()
{
	Object::Initialize();

	OGameObject& TestGameObject = TCreateGameObject<GTestObject>();
	{
		CMesh* Mesh = TestGameObject.TAddComponent<CMesh>(TestGameObject, *DirectX11);
		{
			Mesh->AddVertex(FVertex(XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(0, 1, 0, 1)));
			Mesh->AddVertex(FVertex(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0, 1, 0, 1)));
			Mesh->AddVertex(FVertex(XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT4(0, 1, 0, 1)));
			Mesh->AddIndex(0);
			Mesh->AddIndex(1);
			Mesh->AddIndex(2);
		}

		CShader* Shader = TestGameObject.TAddComponent<CShader>(TestGameObject, *Window, *DirectX11, *Camera);
		{
			Shader->LoadShader(ToWString(GET_SHADER_FILE_DIR("UnlitVertexShader.hlsl")), ToWString(GET_SHADER_FILE_DIR("UnlitPixelShader.hlsl")));
		}
	}

	OGameObject& Gizmo1 = TCreateGameObject<GGizmo>(*Camera);
	{
		CMesh* Mesh = Gizmo1.TAddComponent<CMesh>(Gizmo1, *DirectX11);
		{
			Mesh->AddVertex(FVertex(XMFLOAT3(-1.5f, 0.0f, -1.5f), XMFLOAT4(-1.5f, 0, 1.5f, 1)));
			Mesh->AddVertex(FVertex(XMFLOAT3(1.5f, 0.0f, -1.5f), XMFLOAT4(1.5f, 0, 1.5f, 1)));
			Mesh->AddIndex(0);
			Mesh->AddIndex(1);
		}

		CShader* Shader = Gizmo1.TAddComponent<CShader>(Gizmo1, *Window, *DirectX11, *Camera);
		{
			Shader->LoadShader(ToWString(GET_SHADER_FILE_DIR("UnlitVertexShader.hlsl")), ToWString(GET_SHADER_FILE_DIR("UnlitPixelShader.hlsl")));
		}
	}

	for (const auto& GameObject : GameObjects)
	{
		GameObject->Initialize();
	}
}

void OWorld::Release()
{
	Object::Release();

	for (const auto& GameObject : GameObjects)
	{
		GameObject->Release();
	}
}

void OWorld::Start()
{
	Object::Start();

	for (const auto& GameObject : GameObjects)
	{
		GameObject->Start();
	}
}

void OWorld::Tick()
{
	Object::Tick();

	for (const auto& GameObject : GameObjects)
	{
		GameObject->Tick();
	}
}

void OWorld::End()
{
	Object::End();

	for (const auto& GameObject : GameObjects)
	{
		GameObject->End();
	}
}