#include "PCH.h"
#include "CUnlitShader.h"
#include "GGizmo.h"

#include "CMesh.h"
#include "ODirectX11.h"
#include "SApplication.h"

GGizmo::GGizmo(float InGridGap, float InGridLength, uint64 InGridSize, XMFLOAT4 InGridColor)
	: OGameObject(),
	GridGap(InGridGap), GridLength(InGridLength), GridSize(InGridSize), GridColor(InGridColor)
{
	CMesh* Mesh = new CMesh;
	{
		Mesh->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		int Index = 0;

		// Vertical Mesh.
		{
			Mesh->AddVertex(FVertex(XMFLOAT3(0, 0, GridLength), GridColor, XMFLOAT2(), XMFLOAT3()));
			Mesh->AddVertex(FVertex(XMFLOAT3(0, 0, -GridLength), GridColor, XMFLOAT2(), XMFLOAT3()));
			Mesh->AddIndex(Index++);
			Mesh->AddIndex(Index++);

			for (int i = 1; i <= InGridSize; i++)
			{
				// Right side.
				{
					Mesh->AddVertex(FVertex(XMFLOAT3(GridGap * i, 0, GridLength), GridColor, XMFLOAT2(), XMFLOAT3()));
					Mesh->AddVertex(FVertex(XMFLOAT3(GridGap * i, 0, -GridLength), GridColor, XMFLOAT2(), XMFLOAT3()));
					Mesh->AddIndex(Index++);
					Mesh->AddIndex(Index++);
				}

				// Left side.
				{
					Mesh->AddVertex(FVertex(XMFLOAT3(-GridGap * i, 0, GridLength), GridColor, XMFLOAT2(), XMFLOAT3()));
					Mesh->AddVertex(FVertex(XMFLOAT3(-GridGap * i, 0, -GridLength), GridColor, XMFLOAT2(), XMFLOAT3()));
					Mesh->AddIndex(Index++);
					Mesh->AddIndex(Index++);
				}
			}
		}

		// horizontal Mesh
		{
			Mesh->AddVertex(FVertex(XMFLOAT3(GridLength, 0, 0), GridColor, XMFLOAT2(), XMFLOAT3()));
			Mesh->AddVertex(FVertex(XMFLOAT3(-GridLength, 0, 0), GridColor, XMFLOAT2(), XMFLOAT3()));
			Mesh->AddIndex(Index++);
			Mesh->AddIndex(Index++);

			for (int i = 1; i <= InGridSize; i++)
			{
				// Up side.
				{
					Mesh->AddVertex(FVertex(XMFLOAT3(GridLength, 0, GridGap * i), GridColor, XMFLOAT2(), XMFLOAT3()));
					Mesh->AddVertex(FVertex(XMFLOAT3(-GridLength, 0, GridGap * i), GridColor, XMFLOAT2(), XMFLOAT3()));
					Mesh->AddIndex(Index++);
					Mesh->AddIndex(Index++);
				}


				// Down side.
				{
					Mesh->AddVertex(FVertex(XMFLOAT3(GridLength, 0, -GridGap * i), GridColor, XMFLOAT2(), XMFLOAT3()));
					Mesh->AddVertex(FVertex(XMFLOAT3(-GridLength, 0, -GridGap * i), GridColor, XMFLOAT2(), XMFLOAT3()));
					Mesh->AddIndex(Index++);
					Mesh->AddIndex(Index++);
				}
			}
		}
	}

	TAttachComponent<CMesh>(Mesh);

	CUnlitShader* Shader = new CUnlitShader;
	{
		Shader->Load(ToWString(GET_SHADER_FILE_PATH("UnlitVertexShader.hlsl")), ToWString(GET_SHADER_FILE_PATH("UnlitPixelShader.hlsl")));
	}

	TAttachComponent<CUnlitShader>(Shader);
}

GGizmo::~GGizmo()
{
	Shutdown();
}

void GGizmo::Init()
{
	OGameObject::Init();
}

void GGizmo::Shutdown()
{
	OGameObject::Shutdown();
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
