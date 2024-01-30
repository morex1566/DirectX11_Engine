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

	template <typename T>
	static bool												CheckIsRegistered();

	template <typename T>
	static void												RegisterClassType();

	void													Init();

private:
	SReflection()											= default;

	static std::map<size_t, std::vector<void*>>				ClassInfos;
};

template <typename T>
std::vector<void*>& SReflection::GetClassProperties()
{
	return ClassInfos[typeid(T).hash_code()];
}

template<typename T>
inline bool SReflection::CheckIsRegistered()
{
	return ClassInfos.find(typeid(T).hash_code()) != ClassInfos.end() ? true : false;
}

template<typename T>
inline void SReflection::RegisterClassType()
{
	ClassInfos.insert(std::make_pair<size_t, std::vector<void*>>(typeid(T).hash_code(), std::vector<void*>()));

}