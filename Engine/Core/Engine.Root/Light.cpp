#include "pch.h"
#include "Light.h"

void Light::SetDiffuseColor(float r_, float g_, float b_, float a_)
{
	_diffuseColor = XMFLOAT4(r_, g_, b_, a_);
}

void Light::SetAmbientColor(float r_, float g_, float b_, float a_)
{
	_ambientColor = XMFLOAT4(r_, g_, b_, a_);
}

void Light::SetSpecularColor(float r_, float g_, float b_, float a_)
{
	_specularColor = XMFLOAT4(r_, g_, b_, a_);
}

void Light::SetSpecularIntensity(float intensity_)
{
	_specularIntensity = intensity_;
}

XMFLOAT4 Light::GetDiffuseColor()
{
	return _diffuseColor;
}

XMFLOAT4 Light::GetAmbientColor()
{
	return _ambientColor;
}

XMFLOAT4 Light::GetSpecularColor()
{
	return _specularColor;
}

float Light::GetSpecularIntensity()
{
	return _specularIntensity;
}