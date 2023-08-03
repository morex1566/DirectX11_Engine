#pragma once

#include <d3d11.h>
#include "Resource.h"

class Mesh : public Resource
{
public:
	Mesh();
	~Mesh() override;

	void Render();
	void Destroy() override;

	void ReadVertices(const aiMesh* mesh_, const aiScene* scene_);
	void ReadIndices(const aiMesh* mesh_, const aiScene* scene_);

	int32 GetIndexCount();

private:
	ComPtr<ID3D11Buffer>	_vertexBuffer;
	ComPtr<ID3D11Buffer>	_indexBuffer;
	int32					_vertexCount;
	int32					_indexCount;
};