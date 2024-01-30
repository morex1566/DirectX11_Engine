#pragma once
class Object
{	
public:
	Object()									= default;
	Object(const Object&)						= default;
	Object& operator=(const Object&)			= default;
	Object(Object&&) noexcept					= default;
	Object& operator=(Object&&) noexcept		= default;
	virtual ~Object()							= default;


public:
	virtual void								Init() {}
	virtual void								Shutdown() {}
	virtual void								Start() {}
	virtual void								Tick() {}
	virtual void								End() {}


};