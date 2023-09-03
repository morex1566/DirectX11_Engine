#pragma once

class Mesh
{
public:
	Mesh(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_, const vector<VertexType>& vertices_, const vector<DWORD>& indices_);
	Mesh(const Mesh&)		= default;
	~Mesh()					= default;

	bool Initialize(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_, const vector<VertexType>& vertices_, const vector<DWORD>& indices_);
	void SendBufferToAssembly();

private:
	ID3D11Buffer* createVertexBuffer(const vector<VertexType>& vertices);
	ID3D11Buffer* createIndexBuffer(const vector<DWORD>& indices);

private:
	ID3D11Device*				_device;
	ID3D11DeviceContext*		_deviceContext;
	ID3D11Buffer*				_vertexBuffer;
	ID3D11Buffer*				_indexBuffer;
	UINT						_bufferSize;
};

