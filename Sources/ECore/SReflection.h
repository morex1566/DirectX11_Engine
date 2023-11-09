#pragma once
class SReflection
{
public:
	SReflection(const SReflection&)							= delete;
	SReflection& operator=(const SReflection&)				= delete;
	SReflection(SReflection&&) noexcept						= delete;
	SReflection& operator=(SReflection&&) noexcept			= delete;
	~SReflection()											= default;

	static SReflection&										GetInstance();

	template <typename T>
	static std::vector<void*>&								GetClassProperties();


	void													Initialize();

private:
	SReflection()											= default;

	static std::map<std::type_info, std::vector<void*>>		ClassInfos;
};

template <typename T>
std::vector<void*>& SReflection::GetClassProperties()
{
	return ClassInfos[typeid(T)];
}
