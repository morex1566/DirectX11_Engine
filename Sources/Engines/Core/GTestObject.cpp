#include "PCH.h"
#include "CMesh.h"
#include "CUnlitShader.h"
#include "CLitShader.h"
#include "GTestObject.h"
#include "CTexture.h"
#include "CModel.h"

GTestObject::GTestObject()
	: OGameObject()
{
	CMesh* Mesh = new CMesh;
	{
		Mesh->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Mesh->AddVertex(FVertex(XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(0, 1, 0, 1),
								XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
		Mesh->AddVertex(FVertex(XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0, 1, 0, 1),
								XMFLOAT2(0.5f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
		Mesh->AddVertex(FVertex(XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT4(0, 1, 0, 1),
								XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)));
		Mesh->AddIndex(0);
		Mesh->AddIndex(1);
		Mesh->AddIndex(2);
		//Mesh->Load(ToWString(GET_RESOURCE_FILE_PATH("cube.fbx")));
	}

	TAttachComponent<CMesh>(Mesh);

	////CUnlitShader* Shader = TAddComponent<CUnlitShader>();
	////{
	////	Shader->LoadShader(ToWString(GET_SHADER_FILE_DIR("UnlitVertexShader.hlsl")), ToWString(GET_SHADER_FILE_DIR("UnlitPixelShader.hlsl")));
	////}

	CTexture* Texture = new CTexture;
	{
		Texture->Load(ToWString(GET_RESOURCE_FILE_PATH("stone.tga")), ETexture::TGA);
	}

	TAttachComponent<CTexture>(Texture);

	CLitShader* Shader = new CLitShader;
	{
		Shader->Load(ToWString(GET_SHADER_FILE_PATH("LitVertexShader.hlsl")),
						ToWString(GET_SHADER_FILE_PATH("LitPixelShader.hlsl")));
	}

	TAttachComponent<CLitShader>(Shader);

}

GTestObject::~GTestObject()
{
	Shutdown();
}

void GTestObject::Init()
{
	OGameObject::Init();
}

void GTestObject::Shutdown()
{
	OGameObject::Shutdown();
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

