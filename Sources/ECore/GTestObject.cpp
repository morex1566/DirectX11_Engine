#include "PCH.h"
#include "CMesh.h"
#include "CShader.h"
#include "GTestObject.h"

GTestObject::GTestObject(const GCamera& InCamera, const OWindow& InWindow, const ODirectX11& InDirectX11)
	: OGameObject()
{
	Camera = &InCamera;
	DirectX11 = &InDirectX11;
	Window = &InWindow;

	CMesh* Mesh = TAddComponent<CMesh>(*DirectX11);
	{
		Mesh->AddVertex(FVertex(XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(0, 1, 0, 1)),
			FVertex(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0, 1, 0, 1)),
			FVertex(XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT4(0, 1, 0, 1)));

		Mesh->AddIndex(0);
		Mesh->AddIndex(1);
		Mesh->AddIndex(2);
	}

	CShader* Shader = TAddComponent<CShader>(*Window, *DirectX11, *Camera);
	{
		Shader->LoadShader(ToWString(GET_SHADER_FILE_DIR("UnlitVertexShader.hlsl")), ToWString(GET_SHADER_FILE_DIR("UnlitPixelShader.hlsl")));
	}
}

GTestObject::~GTestObject()
{
}

void GTestObject::Initialize()
{
	OGameObject::Initialize();
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
