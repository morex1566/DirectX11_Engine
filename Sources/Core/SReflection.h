#pragma once
class SReflection
{
public:
	SReflection(const SReflection&)					= delete;
	SReflection& operator=(const SReflection&)		= delete;
	SReflection(SReflection&&) noexcept				= delete;
	SReflection& operator=(SReflection&&) noexcept	= delete;
	~SReflection()									= default;

	static SReflection&								GetInstance();

	void											Initialize();

private:
	SReflection()									= default;

};