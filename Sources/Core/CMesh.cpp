#include "PCH.h"
#include "CMesh.h"
#include "OConsole.h"
#include "ODirectX11.h"

CMesh::CMesh(const OGameObject& InOwner, const ODirectX11& InDirectX11)
	: OComponent(InOwner)
{
	DirectX11 = &InDirectX11;
}

CMesh::~CMesh()
{
}

Object::EHandleResultType CMesh::Initialize()
{
	OComponent::Initialize();

	return EHandleResultType::Success;
}

void CMesh::Release()
{
	OComponent::Release();
}

void CMesh::Start()
{
	OComponent::Start();

	if (CreateVertexBuffer() != EHandleResultType::Success)
	{
		throw std::exception();
	}

	if (CreateIndexBuffer() != EHandleResultType::Success)
	{
		throw std::exception();
	}
}

void CMesh::Tick()
{
	OComponent::Tick();

	DirectX11->SetDepthStencilState(ODirectX11::ERenderModeType::Model);
	DirectX11->SetRasterizerState(ODirectX11::ERenderModeType::Model);
	DirectX11->SetRenderTargets(ODirectX11::ERenderModeType::Model);
	DirectX11->SetViewport(ODirectX11::ERenderModeType::Model);

	Render();
}

void CMesh::End()
{
	OComponent::End();
}

void CMesh::AddVertex(const FVertex& InVertex)
{
	Vertices.emplace_back(InVertex);
}

void CMesh::AddIndex(UINT InIndex)
{
	Indices.emplace_back(InIndex);
}

UINT CMesh::GetVertexCount() const
{
	return  static_cast<UINT>(Vertices.size());
}

UINT CMesh::GetIndexCount() const
{
	return static_cast<UINT>(Indices.size());
}

Object::EHandleResultType CMesh::CreateVertexBuffer()
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
	Result = DirectX11->GetDevice().CreateBuffer(&VertexBufferDesc, &VertexData, VertexBuffer.GetAddressOf());
	if (FAILED(Result))
	{
		OConsole::LogError(L"VertexCreateBuffer() is failed.");
		return EHandleResultType::Failed;
	}

	return EHandleResultType::Success;
}

Object::EHandleResultType CMesh::CreateIndexBuffer()
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
	Result = DirectX11->GetDevice().CreateBuffer(&IndexBufferDesc, &IndexData, IndexBuffer.GetAddressOf());
	if (FAILED(Result))
	{
		OConsole::LogError(L"CreateBuffer() is failed.");
		return EHandleResultType::Failed;
	}

	return EHandleResultType::Success;
}

void CMesh::Render()
{
	UINT				 Stride = sizeof(FVertex);
	UINT				 Offset = 0;
	ID3D11DeviceContext& DeviceContext = DirectX11->GetDeviceContext();
	{
		// Set the vertex buffer to active in the input assembler so it can be rendered.
		DeviceContext.IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &Stride, &Offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		DeviceContext.IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		DeviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}
