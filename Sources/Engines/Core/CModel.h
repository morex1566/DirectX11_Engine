#pragma once
#include "OComponent.h"

class CMesh;
class CTexture;
class CLitShader;

class CModel : public OComponent
{
public:
	CModel(const OGameObject* InOwner);
	CModel(const CModel&)							= default;
	CModel& operator=(const CModel&)				= default;
	CModel(CModel&&) noexcept						= default;
	CModel& operator=(CModel&&) noexcept			= default;
	~CModel() override;


public:
	virtual void		Initialize() override;
	virtual void		Release() override;
	virtual void		Start() override;
	virtual void		Tick() override;
	virtual void		End() override;

public:
	void LoadModel(const std::wstring& InFilePath);
	void LoadTexture(const std::wstring& InFilePath);
	void LoadShader(const std::wstring& InVSFilePath, const std::wstring& InPSFilePath);
	void ReleaseMesh();
	void ReleaseTexture();
	void ReleaseShader();


private:
	std::vector<CMesh*>			Meshes;
	CTexture*					Texture;
	CLitShader*					Shader;


};

