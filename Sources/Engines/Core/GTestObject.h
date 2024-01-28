#pragma once
#include "OGameObject.h"

class GCamera;
class ODirectX11;
class OWindow;

class GTestObject : public OGameObject
{
public:
	GTestObject();
	GTestObject(const GTestObject&)							= default;
	GTestObject& operator=(const GTestObject&)				= default;
	GTestObject(GTestObject&&) noexcept						= default;
	GTestObject& operator=(GTestObject&&) noexcept			= default;
	~GTestObject() override;


public:
	void		Init() override;
	void		Shutdown() override;
	void		Start() override;
	void		Tick() override;
	void		End() override;


};

