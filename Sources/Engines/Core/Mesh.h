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
	~Mesh() override  						= default;


public:
	void Start() override;
	void Update() override;
	void Shutdown() override;
	void Render();
	void Load(const std::wstring& Filename);
	void Release();
	FORCEINLINE unsigned int GetVertexCount() const { return vertexCount; }
	FORCEINLINE unsigned int GetIndexCount() const { return indexCount; }


private:
	void createBuffer();


private:
	// CAUTION : device를 수정하지 마세요!!!
	ID3D11Device*				device = nullptr;
	// CAUTION : deviceContext를 수정하지 마세요!!!
	ID3D11DeviceContext*		deviceContext = nullptr;
	D3D_PRIMITIVE_TOPOLOGY		primitiveType;

	ID3D11Buffer*				vertexBuffer = nullptr;
	ID3D11Buffer*				indexBuffer = nullptr;
	unsigned int				vertexCount;
	unsigned int				indexCount;
	VertexType*						vertices = nullptr;
	unsigned long*				indices = nullptr;


};

