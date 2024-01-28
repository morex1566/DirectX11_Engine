#pragma once
class Object
{	
public:
	Object();
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


public:
	uint8										CheckIsEnabled() const;
	std::wstring								GetName() const;
	void										SetIsEnabled(uint8 InOnOff);
	void										SetName(const std::wstring& InName);
	FORCEINLINE void							SetName(const char* InName) { Name = ToWString(InName); }


protected:
	uint8										bIsEnabled;
	std::wstring								Name;


};