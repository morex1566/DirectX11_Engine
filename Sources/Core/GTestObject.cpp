#include "PCH.h"
#include "GTestObject.h"
#include "OConsole.h"
#include "ODirectX11.h"

GTestObject::GTestObject(ODirectX11& InDirectX11): VertexBuffer(nullptr), IndexBuffer(nullptr),
                                                         VertexCount(0), IndexCount(0),
                                                         Color(XMFLOAT4(0, 1, 0, 1)), BL(XMFLOAT3(-1.0f, -1.0f, 0.0f)), TM(XMFLOAT3(0.0f, 1.0f, 0.0f)),
                                                         BR(XMFLOAT3(1.0f, -1.0f, 0.0f))
{
	DirectX11 = &InDirectX11;
}

GTestObject::~GTestObject()
{
}

Object::EHandleResultType GTestObject::Initialize()
{
	OGameObject::Initialize();

	FVertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// Set the number of vertices in the vertex array.
	VertexCount = 3;

	// Set the number of indices in the index array.
	IndexCount = 3;

	// Create the vertex array.
	vertices = new FVertex[VertexCount];
	if (!vertices)
	{
		OConsole::LogError(L"new FVertexType[VertexCount] is failed.");
		return EHandleResultType::Failed;
	}

	// Create the index array.
	indices = new unsigned long[IndexCount];
	if (!indices)
	{
		OConsole::LogError(L"new unsigned long[IndexCount] is failed.");
		return EHandleResultType::Failed;
	}

	// Load the vertex array with data.
	vertices[0].Position = BL;  // Bottom left.
	vertices[0].Color = Color;

	vertices[1].Position = TM;  // Top middle.
	vertices[1].Color = Color;

	vertices[2].Position = BR;  // Bottom right.
	vertices[2].Color = Color;

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(FVertex) * VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = DirectX11->GetDevice().CreateBuffer(&vertexBufferDesc, &vertexData, &VertexBuffer);
	if (FAILED(result))
	{
		OConsole::LogError(L"CreateBuffer() is failed.");
		return EHandleResultType::Failed;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = DirectX11->GetDevice().CreateBuffer(&indexBufferDesc, &indexData, &IndexBuffer);
	if (FAILED(result))
	{
		OConsole::LogError(L"CreateBuffer() is failed.");
		return EHandleResultType::Failed;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return EHandleResultType::Success;
}

void GTestObject::Release()
{
}

void GTestObject::Start()
{
}

void GTestObject::Tick()
{
}

void GTestObject::End()
{
}

void GTestObject::Render()
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(FVertex);
	offset = 0;

	ID3D11DeviceContext& DeviceContext = DirectX11->GetDeviceContext();
	{
		// Set the vertex buffer to active in the input assembler so it can be rendered.
		DeviceContext.IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		DeviceContext.IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		DeviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}

UINT GTestObject::GetVertexCount() const
{
	return VertexCount;
}

UINT GTestObject::GetIndexCount() const
{
	return IndexCount;
}
