#pragma once
#include "Component.h"

class Mesh : public Component, public Management
{
public:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	Mesh(const GameObject* owner_);
	Mesh(const Mesh&)						= delete;
	Mesh& operator=(const Mesh&)			= delete;
	Mesh(Mesh&&) noexcept					= delete;
	Mesh& operator=(Mesh&&) noexcept		= delete;
	~Mesh() override;

	bool Initialize();
	void AddVertex(Vertex vertex_);
	void AddIndex(unsigned int order_);
	bool CreateBuffer(ID3D11Device* device_);

private:
	std::vector<Vertex>					_vertices;
	std::vector<unsigned int>			_indices;
	ComPtr<ID3D11Buffer>				_vertexBuffer;
	ComPtr<ID3D11Buffer>				_indexBuffer;

	D3D11_BUFFER_DESC					_vertexBufferDesc;
	D3D11_BUFFER_DESC					_indexBufferDesc;
	D3D11_SUBRESOURCE_DATA				_vertexData;
	D3D11_SUBRESOURCE_DATA				_indexData;
};

