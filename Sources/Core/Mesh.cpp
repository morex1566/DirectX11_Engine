#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_, const vector<VertexType>& vertices_, const vector<DWORD>& indices_)
	: _device(device_), _deviceContext(deviceContext_), _vertexBuffer(nullptr), _indexBuffer(nullptr)
{
	bool result;

	result = Initialize(device_, deviceContext_, vertices_, indices_);
	if (!result)
	{
		throw;
	}
}

bool Mesh::Initialize(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_, const vector<VertexType>& vertices_,
	const vector<DWORD>& indices_)
{
	bool result;

	if (!createVertexBuffer(vertices_))
	{
		return false;
	}

	if (!createIndexBuffer(indices_))
	{
		return false;
	}

	return true;
}

void Mesh::SendBufferToAssembly()
{
	uint offset;
	uint stride;

	offset = 0;
	stride = sizeof(VertexType);

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	_deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	_deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ID3D11Buffer* Mesh::createVertexBuffer(const vector<VertexType>& vertices)
{
	// Safety
	if (_vertexBuffer != nullptr) { _vertexBuffer->Release(); }

	this->_stride = std::make_shared<UINT>(sizeof(vertices.data()));

	// Set up the description of the static vertex buffer.
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	{
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(VertexType) * vertices.size());
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
	}

	// Set up subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	{
		vertexData.pSysMem = vertices.data();
	}

	// Create the vertex buffer.
	HRESULT result = _device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}

	return _vertexBuffer;
}

ID3D11Buffer* Mesh::createIndexBuffer(const vector<DWORD>& indices)
{
	// Safety
	if (_indexBuffer != nullptr) { _indexBuffer->Release(); }

	this->_bufferSize = indices.size();

	// Set up the description of the static index buffer.
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	{
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(DWORD) * indices.size());
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
	}

	// Set up subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	{
		indexData.pSysMem = indices.data();
	}

	// Create the index buffer.
	HRESULT result = _device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result))
	{
		return nullptr;
	}

	return _indexBuffer;
}
