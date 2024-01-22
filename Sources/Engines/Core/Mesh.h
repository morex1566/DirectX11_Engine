#pragma once

#include <istream>

#include "Component.h"

class Mesh : public Component
{
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, D3D_PRIMITIVE_TOPOLOGY primitiveType);
	Mesh(const Mesh&)						= default;
	Mesh& operator=(const Mesh&)			= default;
	Mesh(Mesh&&) noexcept					= default;
	Mesh& operator=(Mesh&&) noexcept		= default;
	virtual ~Mesh() override  				= default;


public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void Shutdown() override;
	void Render();
	void Load(const std::wstring& Filename);
	void Release();
	FORCEINLINE unsigned int GetVertexCount() const { return vertexCount; }
	FORCEINLINE unsigned int GetIndexCount() const { return indexCount; }


private:
	void createBuffer();


private:
	// CAUTION : device를 수정하지 마세요!!!
	ID3D11Device*				device;
	// CAUTION : deviceContext를 수정하지 마세요!!!
	ID3D11DeviceContext*		deviceContext;
	D3D_PRIMITIVE_TOPOLOGY		primitiveType;

	ID3D11Buffer*				vertexBuffer;
	ID3D11Buffer*				indexBuffer;
	unsigned int				vertexCount;
	unsigned int				indexCount;
	Vertex*						vertices;
	unsigned long*				indices;


};

