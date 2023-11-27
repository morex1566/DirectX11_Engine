#include "PCH.h"
#include "CMesh.h"
#include "CShader.h"
#include "GTestObject.h"
#include "SConsole.h"
#include "SInput.h"

GTestObject::GTestObject()
	: OGameObject()
{
	CMesh* Mesh = TAddComponent<CMesh>();
	{
		Mesh->AddVertex(FVertex(XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(0, 1, 0, 1)),
			FVertex(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0, 1, 0, 1)),
			FVertex(XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT4(0, 1, 0, 1)));

		Mesh->AddIndex(0);
		Mesh->AddIndex(1);
		Mesh->AddIndex(2);
	}

	CShader* Shader = TAddComponent<CShader>();
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
