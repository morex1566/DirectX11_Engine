#pragma once
class Object
{	
public:
	enum class EHandleResultType
	{
		Failed,
		Success,
	};

	Object();
	Object(const Object&)						= default;
	Object& operator=(const Object&)			= default;
	Object(Object&&) noexcept					= default;
	Object& operator=(Object&&) noexcept		= default;
	virtual ~Object()							= default;

	virtual EHandleResultType					Initialize();
	virtual void								Release();
	/**
	 * \brief Called only once before entering the main loop.
	 */
	virtual void								Start();
	/**
	 * \brief Called once when the every frame.
	 */
	virtual void								Tick();
	/**
	 * \brief Called only once immediately after the main loop is over.
	 */
	virtual void								End();

	uint8										CheckIsEnabled() const;

	std::wstring								GetName() const;

	void										SetIsEnabled(uint8 InOnOff);
	void										SetName(const std::wstring& InName);
	FORCEINLINE void							SetName(const char* InName) { Name = ToWString(InName); }

protected:
	uint8										bIsEnabled;
	std::wstring								Name;
};