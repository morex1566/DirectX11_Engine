#pragma once
class Object
{	
public:
	enum class EHandleResultType
	{
		Failed,
		Success,
	};

	Object()									= default;
	Object(const Object&)						= default;
	Object& operator=(const Object&)			= default;
	Object(Object&&) noexcept					= default;
	Object& operator=(Object&&) noexcept		= default;
	virtual ~Object()							= default;

	virtual EHandleResultType					Initialize() = 0;
	virtual void								Release() = 0;
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

protected:


private:
	
};