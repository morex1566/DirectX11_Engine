#include "PCH.h"
#include "CMesh.h"
#include "SApplication.h"
#include "SConsole.h"
#include "ODirectX11.h"

CMesh::CMesh(const OGameObject* InOwner)
	: OComponent(InOwner)
{
}

CMesh::~CMesh()
{
}

void CMesh::Initialize()
{
	OComponent::Initialize();
}

void CMesh::Release()
{
	OComponent::Release();
}

void CMesh::Start()
{
	OComponent::Start();

	if (!CreateVertexBuffer())
	{
		throw std::exception();
	}

	if (!CreateIndexBuffer())
	{
		throw std::exception();
	}
}

void CMesh::Tick()
{
	OComponent::Tick();

	auto DirectX11 = SApplication::GetDirectX11();
	{
		DirectX11->SetDepthStencilState(ODirectX11::ERenderModeType::Model);
		DirectX11->SetRasterizerState(ODirectX11::ERenderModeType::Model);
		DirectX11->SetRenderTargets(ODirectX11::ERenderModeType::Model);
		DirectX11->SetViewport(ODirectX11::ERenderModeType::Model);
	}

	Render();
}

void CMesh::End()
{
	OComponent::End();
}

void CMesh::AddVertex(const FVertex& InFirst, const FVertex& InSecond, const FVertex& InThird)
{
	Vertices.emplace_back(InFirst);
	Vertices.emplace_back(InSecond);
	Vertices.emplace_back(InThird);
}

void CMesh::AddIndex(UINT InIndex)
{
	Indices.emplace_back(InIndex);
}

bool CMesh::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC			VertexBufferDesc;
	D3D11_SUBRESOURCE_DATA		VertexData;
	HRESULT						Result;

	// Initialize vertex buffer desc.
	{
		ZeroMemory(&VertexBufferDesc, sizeof(VertexBufferDesc));

		VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(FVertex) * Vertices.size());
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = 0;
		VertexBufferDesc.MiscFlags = 0;
		VertexBufferDesc.StructureByteStride = 0;
	}

	// Initialize vertex data.
	{
		ZeroMemory(&VertexData, sizeof(VertexData));

		VertexData.pSysMem = Vertices.data();
		VertexData.SysMemPitch = 0;
		VertexData.SysMemSlicePitch = 0;
	}

	// Create vertex buffer.
	ID3D11Device& Device = SApplication::GetDirectX11()->GetDevice();
	{
		Result = Device.CreateBuffer(&VertexBufferDesc, &VertexData, VertexBuffer.GetAddressOf());
		if (FAILED(Result))
		{
			SConsole::LogError(L"VertexCreateBuffer() is failed.");
			return false;
		}
	}

	return true;
}

bool CMesh::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC			IndexBufferDesc;
	D3D11_SUBRESOURCE_DATA		IndexData;
	HRESULT						Result;

	// Initialize index buffer desc.
	{
		ZeroMemory(&IndexBufferDesc, sizeof(IndexBufferDesc));

		IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(UINT) * Indices.size());
		IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags = 0;
		IndexBufferDesc.MiscFlags = 0;
		IndexBufferDesc.StructureByteStride = 0;
	}

	// Initialize index data.
	{
		ZeroMemory(&IndexData, sizeof(IndexData));

		IndexData.pSysMem = Indices.data();
		IndexData.SysMemPitch = 0;
		IndexData.SysMemSlicePitch = 0;
	}

	// Create index buffer.
	ID3D11Device& Device = SApplication::GetDirectX11()->GetDevice();
	{
		Result = Device.CreateBuffer(&IndexBufferDesc, &IndexData, IndexBuffer.GetAddressOf());
		if (FAILED(Result))
		{
			SConsole::LogError(L"CreateBuffer() is failed.");
			return false;
		}
	}

	return true;
}

void CMesh::Render()
{
	UINT				 Stride;
	UINT				 Offset;

	Stride = sizeof(FVertex);
	Offset = 0;
	ID3D11DeviceContext& DeviceContext = SApplication::GetDirectX11()->GetDeviceContext();
	{
		// Set the vertex buffer to active in the input assembler so it can be rendered.
		DeviceContext.IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &Stride, &Offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		DeviceContext.IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		DeviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}