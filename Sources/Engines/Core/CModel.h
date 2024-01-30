#pragma once
#include "OComponent.h"

#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

class CMesh;
class CTexture;
class CLitShader;

class CModel : public OComponent
{
public:
	CModel(OGameObject* InOwner);

	CModel()										= default;
	CModel(const CModel&)							= default;
	CModel& operator=(const CModel&)				= default;
	CModel(CModel&&) noexcept						= default;
	CModel& operator=(CModel&&) noexcept			= default;
	~CModel() override;


public:
	virtual void		Init() override;
	virtual void		Shutdown() override;
	virtual void		Start() override;
	virtual void		Tick() override;
	virtual void		End() override;


public:
	void LoadModel(const std::wstring& InFilePath);
	void LoadTexture(const std::wstring& InFilePath);
	void LoadShader(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath);


private:
	void ProcessNode(const aiNode* InNode, const aiScene* InScene);
	CMesh* ProcessMesh(const aiMesh* InMesh, const aiNode* InNode);


private:
	std::vector<CMesh*>			Meshes;
	//std::vector<CTexture*>    Textures;
	//std::vector<CLitShader*>	Shaders;
	CTexture*					Texture;
	CLitShader*					Shader;


};

