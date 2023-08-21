#pragma once

#include "Component.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

class Light;

class Model : public Component
{
public:
	Model()							= default;
	Model(const Model&)				= default;
	~Model() override				= default;

	void Initialize(ID3D11Device* device_, ID3D11DeviceContext* deviceContext_, HWND hWnd_,
					const std::string& meshFilePath_, const std::string& textureFilePath_, const std::string& vsFilePath_, const std::string& psFilePath_);

	void Render(const XMMATRIX& worldMatrix_, const XMMATRIX& viewMatrix_, const XMMATRIX& projectionMatrix_, Light* light_);
	void ClearMemory();
	void ClearMeshes();
	void ClearTexture();
	void ClearShader();

	std::vector<Mesh>	GetMeshes();
	Texture*			GetTexture();
	Shader*				GetShader();

	void LoadMesh(const std::string& filePath_);
	void LoadTexture(const std::string& filePath_);
	void LoadShader(const std::string& vsFilePath_, const std::string& psFilePath_);

private:
	void processNode(const aiNode* node, const aiScene* scene);
	Mesh processMesh(const aiMesh* mesh, const aiNode* node);

private:
	ID3D11Device*					_device;
	ID3D11DeviceContext*			_deviceContext;
	HWND							_hWnd;

	int								_vertexCount;
	int								_indexCount;

	std::vector<Mesh>				_meshes;
	std::unique_ptr<Texture>		_texture;
	std::unique_ptr<Shader>			_shader;
};