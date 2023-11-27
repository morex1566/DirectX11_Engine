#include "PCH.h"
#include "CLine.h"
#include "CShader.h"
#include "GGizmo.h"
#include "ODirectX11.h"
#include "SApplication.h"

GGizmo::GGizmo(float InSpaceBtwGridByGri, XMFLOAT4 InGridColor)
	: OGameObject()
{
	SpaceBtwGridByGrid = InSpaceBtwGridByGri;
	GridColor = InGridColor;
	Line = TAddComponent<CLine>();

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

	//// Create grid.
	//{
	//	int Index = 0;

	//	// Vertical line.
	//	{
	//		Line->AddVertex(FVertex(XMFLOAT3(0, 0, 10), GridColor),
	//						FVertex(XMFLOAT3(0, 0, -10), GridColor));
	//		Line->AddIndex(Index++);
	//		Line->AddIndex(Index++);

	//		for (int i = 1; i <= 10; i++)
	//		{
	//			// Right side.
	//			{
	//				Line->AddVertex(FVertex(XMFLOAT3(10 * i, 0, 10), GridColor),
	//					FVertex(XMFLOAT3(10 * i, 0, -10), GridColor));
	//				Line->AddIndex(Index++);
	//				Line->AddIndex(Index++);
	//			}

	//			// Left side.
	//			{
	//				Line->AddVertex(FVertex(XMFLOAT3(-10 * i, 0, 10), GridColor),
	//					FVertex(XMFLOAT3(-10 * i, 0, -10), GridColor));
	//				Line->AddIndex(Index++);
	//				Line->AddIndex(Index++);
	//			}
	//		}
	//	}

	//	// horizontal line
	//	{
	//		Line->AddVertex(FVertex(XMFLOAT3(10, 0, 0), GridColor),
	//						FVertex(XMFLOAT3(-10, 0, 0), GridColor));
	//		Line->AddIndex(Index++);
	//		Line->AddIndex(Index++);

	//		for (int i = 0; i < 10; i++)
	//		{
	//			// Up side.
	//			{
	//				Line->AddVertex(FVertex(XMFLOAT3(10, 10 * i, 0), GridColor),
	//								FVertex(XMFLOAT3(-10, 10 * i, 0), GridColor));
	//				Line->AddIndex(Index++);
	//				Line->AddIndex(Index++);
	//			}


	//			// Down side.
	//			{
	//				Line->AddVertex(FVertex(XMFLOAT3(10, -10 * i, 0), GridColor),
	//								FVertex(XMFLOAT3(-10, -10 * i, 0), GridColor));
	//				Line->AddIndex(Index++);
	//				Line->AddIndex(Index++);
	//			}
	//		}
	//	}
	//}
}

void GGizmo::Tick()
{
	Object::Tick();
}

void GGizmo::End()
{
	Object::End();
}
