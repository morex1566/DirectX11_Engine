#pragma once
class Object
{
public:
	Object();
	Object(const Object&)						= default;
	Object& operator=(const Object&)			= default;
	Object(Object&&) noexcept					= default;
	Object& operator=(Object&&) noexcept		= default;
	~Object();

	bool				CheckIsEnabled() const;
	bool				CheckIsActivated() const;

	void				SetActive(bool toggle_);

protected:
	bool		_isEnabled;
	bool		_isActivated;

};

