#include "PCH.h"
#include "CLine.h"
#include "CShader.h"
#include "GGizmo.h"
#include "ODirectX11.h"

GGizmo::GGizmo()
	: OGameObject()
{
	CLine* Line = TAddComponent<CLine>();
	{
		
	}

	CShader* Shader = TAddComponent<CShader>();
	{
		Shader->LoadShader(ToWString(GET_SHADER_FILE_DIR("UnlitVertexShader.hlsl")), ToWString(GET_SHADER_FILE_DIR("UnlitPixelShader.hlsl")));
	}
}

GGizmo::~GGizmo()
{
}

void GGizmo::Initialize()
{
	Object::Initialize();
}

void GGizmo::Release()
{
	Object::Release();
}

void GGizmo::Start()
{
	Object::Start();
}

void GGizmo::Tick()
{
	Object::Tick();
}

void GGizmo::End()
{
	Object::End();
}
