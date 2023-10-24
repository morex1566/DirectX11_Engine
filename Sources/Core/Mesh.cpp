#include "PCH.h"
#include "Mesh.h"

Mesh::Mesh(const GameObject* owner_)
	: Component(owner_)
{
}

Mesh::~Mesh()
{
}

bool Mesh::Initialize()
{
	// Setup vertex buffer desc.
	ZeroMemory(&_vertexBufferDesc, sizeof(_vertexBufferDesc));
	{
		_vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		_vertexBufferDesc.ByteWidth = _vertices.size() * sizeof(Vertex);
		_vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		_vertexBufferDesc.CPUAccessFlags = 0;
		_vertexBufferDesc.MiscFlags = 0;
		_vertexBufferDesc.StructureByteStride = 0;

		_vertexData.pSysMem = &_vertices;
		_vertexData.SysMemPitch = 0;
		_vertexData.SysMemSlicePitch = 0;
	}

	// Setup index buffer desc.
	ZeroMemory(&_indexBufferDesc, sizeof(_indexBufferDesc));
	{
		_indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		_indexBufferDesc.ByteWidth = _indices.size() * sizeof(unsigned int);
		_indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		_indexBufferDesc.CPUAccessFlags = 0;
		_indexBufferDesc.MiscFlags = 0;
		_indexBufferDesc.StructureByteStride = 0;

		_indexData.pSysMem = &_indices;
		_indexData.SysMemPitch = 0;
		_indexData.SysMemSlicePitch = 0;
	}

	return true;
};

void Mesh::AddVertex(Vertex vertex_)
{
	_vertices.emplace_back(vertex_);
};

void Mesh::AddIndex(unsigned int order_)
{
	_indices.emplace_back(order_);
};

bool Mesh::CreateBuffer(ID3D11Device* device_)
{
	HRESULT result;

	if (_vertexBuffer) { _vertexBuffer->Release(); }
	if (_indexBuffer) { _indexBuffer->Release(); }

	// Create vertex buffer.
	result = device_->CreateBuffer(&_vertexBufferDesc, &_vertexData, _vertexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		Console::LogWarning("CreateVertexBuffer() is failed.");
		return false;
	}

	// Create index buffer.
	result = device_->CreateBuffer(&_indexBufferDesc, &_indexData, _indexBuffer.GetAddressOf());
	if (FAILED(result))
	{
		Console::LogWarning("CreateIndexBuffer() is failed.");
		return false;
	}

	return true;
};