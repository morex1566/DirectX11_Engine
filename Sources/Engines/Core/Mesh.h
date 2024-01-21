#pragma once

#include "Component.h"

class Mesh : public Component
{
public:
	Mesh();
	Mesh(const Mesh&)						= default;
	Mesh& operator=(const Mesh&)			= default;
	Mesh(Mesh&&) noexcept					= default;
	Mesh& operator=(Mesh&&) noexcept		= default;
	~Mesh() override  						= default;


public:
	void Shutdown();
	void Render();
	void LoadMesh();
	void ReleaseMesh();


private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	unsigned int vertexCount;
	unsigned int indexCount;

};

