#pragma once
#include "Object.h"

class OGameObject;

class OComponent : public Object, public IEnable
{
public:
	OComponent(const OGameObject* InOwner);
	OComponent()										= default;
	OComponent(const OComponent&)						= default;
	OComponent& operator=(const OComponent&)			= default;
	OComponent(OComponent&&) noexcept					= default;
	OComponent& operator=(OComponent&&) noexcept		= default;
	~OComponent() override								= default;


public:
	void				Init() override {}
	void				Shutdown() override {}
	void				Start() override {}
	void				Tick() override {}
	void				End() override {}


protected:
	const OGameObject*	Owner;


};