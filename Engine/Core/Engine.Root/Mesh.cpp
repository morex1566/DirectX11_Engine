#include "pch.h"
#include "Mesh.h"
#include "D3DManager.h"

#pragma warning(disable: 4267)

Mesh::Mesh()
	: Resource(), _vertexBuffer(nullptr), _indexBuffer(nullptr), _vertexCount(0), _indexCount(0)
{
}

Mesh::~Mesh()
{
	Mesh::Destroy();
}

void Mesh::Render()
{
	uint stride;
	uint offset;

	stride = sizeof(VertexType);
	offset = 0;

	ID3D11DeviceContext* deviceContext = D3DManager::Get().GetDeviceContext();
	{
		// Set the vertex buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);

		// Set the index buffer to active in the input assembler so it can be rendered.
		deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	return;
}

void Mesh::Destroy()
{
	_isDestroyed = true;
	_isActivated = false;
}

void Mesh::ReadVertices(const aiMesh* mesh_, const aiScene* scene_)
{
	std::vector<VertexType>			vertices;
	HRESULT							result;

	for (uint i = 0; i < mesh_->mNumVertices; i++)
	{
		VertexType vertex;

		vertex.position.x = mesh_->mVertices[i].x;
		vertex.position.y = mesh_->mVertices[i].x;
		vertex.position.z = mesh_->mVertices[i].x;

		if (mesh_->mTextureCoords[0])
		{
			vertex.texture.x = static_cast<float>(mesh_->mTextureCoords[0][i].x);
			vertex.texture.y = static_cast<float>(mesh_->mTextureCoords[0][i].y);
		}

		vertices.push_back(vertex);
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	{
		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(VertexType) * mesh_->mNumVertices;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;
	}

	D3D11_SUBRESOURCE_DATA vertexData;
	{
		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = &vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;
	}

	result = D3DManager::Get().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if(FAILED(result))
	{
		LogManager::LogWarning("Fail to read vertices.");
		return;
	}
}

void Mesh::ReadIndices(const aiMesh* mesh_, const aiScene* scene_)
{
	std::vector<DWORD>		indices;
	HRESULT					result;

	for (uint i = 0; i < mesh_->mNumFaces; i++)
	{
		const aiFace face = mesh_->mFaces[i];

		for (uint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	{
		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(DWORD) * indices.size();
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;
	}

	D3D11_SUBRESOURCE_DATA  indexData;
	{
		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = &indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;
	}

	result = D3DManager::Get().GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if(FAILED(result))
	{
		LogManager::LogWarning("Fail to read indices.");
		return;
	}
}

int32 Mesh::GetIndexCount()
{
	return _indexCount;
}
