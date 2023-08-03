#pragma once

#include "GameObject.h"

class Light : public GameObject
{
public:
	virtual ~Light() override = default;

	void SetDiffuseColor(float r_ = 0.0f, float g_ = 0.0f, float b_ = 0.0f, float a_ = 0.0f);
	void SetAmbientColor(float r_ = 0.0f, float g_ = 0.0f, float b_ = 0.0f, float a_ = 0.0f);
	void SetSpecularColor(float r_ = 0.0f, float g_ = 0.0f, float b_ = 0.0f, float a_ = 0.0f);
	void SetSpecularIntensity(float intensity_);

	XMFLOAT4	GetDiffuseColor();
	XMFLOAT4	GetAmbientColor();
	XMFLOAT4	GetSpecularColor();
	float		GetSpecularIntensity();

protected:
	Light() = default;

protected:
	XMFLOAT4			_diffuseColor;
	XMFLOAT4			_ambientColor;
	XMFLOAT4			_specularColor;
	float				_specularIntensity;
};