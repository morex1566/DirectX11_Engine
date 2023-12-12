#include "PCH.h"
#include "CShader.h"
#include "GGizmo.h"

#include "CMesh.h"
#include "ODirectX11.h"
#include "SApplication.h"

GGizmo::GGizmo(float InGridGap, float InGridLength, uint64 InGridSize, XMFLOAT4 InGridColor)
	: OGameObject()
{
	GridGap = InGridGap;
	GridLength = InGridLength;
	GridSize = InGridSize;
	GridColor = InGridColor;

	CMesh* Mesh = TAddComponent<CMesh>();
	{
		Mesh->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		int Index = 0;

		// Vertical Mesh.
		{
			Mesh->AddVertex(FVertex(XMFLOAT3(0, 0, GridLength), GridColor));
			Mesh->AddVertex(FVertex(XMFLOAT3(0, 0, -GridLength), GridColor));
			Mesh->AddIndex(Index++);
			Mesh->AddIndex(Index++);

			for (int i = 1; i <= InGridSize; i++)
			{
				// Right side.
				{
					Mesh->AddVertex(FVertex(XMFLOAT3(GridGap * i, 0, GridLength), GridColor));
					Mesh->AddVertex(FVertex(XMFLOAT3(GridGap * i, 0, -GridLength), GridColor));
					Mesh->AddIndex(Index++);
					Mesh->AddIndex(Index++);
				}

				// Left side.
				{
					Mesh->AddVertex(FVertex(XMFLOAT3(-GridGap * i, 0, GridLength), GridColor));
					Mesh->AddVertex(FVertex(XMFLOAT3(-GridGap * i, 0, -GridLength), GridColor));
					Mesh->AddIndex(Index++);
					Mesh->AddIndex(Index++);
				}
			}
		}

		// horizontal Mesh
		{
			Mesh->AddVertex(FVertex(XMFLOAT3(GridLength, 0, 0), GridColor));
			Mesh->AddVertex(FVertex(XMFLOAT3(-GridLength, 0, 0), GridColor));
			Mesh->AddIndex(Index++);
			Mesh->AddIndex(Index++);

			for (int i = 1; i <= InGridSize; i++)
			{
				// Up side.
				{
					Mesh->AddVertex(FVertex(XMFLOAT3(GridLength, 0, GridGap * i), GridColor));
					Mesh->AddVertex(FVertex(XMFLOAT3(-GridLength, 0, GridGap * i), GridColor));
					Mesh->AddIndex(Index++);
					Mesh->AddIndex(Index++);
				}


				// Down side.
				{
					Mesh->AddVertex(FVertex(XMFLOAT3(GridLength, 0, -GridGap * i), GridColor));
					Mesh->AddVertex(FVertex(XMFLOAT3(-GridLength, 0, -GridGap * i), GridColor));
					Mesh->AddIndex(Index++);
					Mesh->AddIndex(Index++);
				}
			}
		}
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
	OGameObject::Initialize();
}

void GGizmo::Release()
{
	OGameObject::Release();
}

void GGizmo::Start()
{
	OGameObject::Start();
}

void GGizmo::Tick()
{
	OGameObject::Tick();
}

void GGizmo::End()
{
	OGameObject::End();
}
