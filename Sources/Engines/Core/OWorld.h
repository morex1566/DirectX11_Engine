#pragma once
#include "Object.h"
#include "OGameObject.h"

class GCamera;
class OWindow;
class ODirectX11;

class OWorld : public Object
{
public:
	// T가 GameObject를 상속받는지 확인
	template <class T> using IsGameObject = std::enable_if_t<std::is_base_of<OGameObject, T>::value>;

	// GameObject를 저장하는 해쉬맵 Key = typeid(OGameObject).name(), Value = OGameObject's vector.
	using GameObjectHashMap = std::map<std::string, std::vector<OGameObject*>>;


public:
	OWorld();
	OWorld(const OWorld&)									= default;
	OWorld& operator=(const OWorld&)						= default;
	OWorld(OWorld&&) noexcept								= default;
	OWorld& operator=(OWorld&&) noexcept					= default;
	~OWorld() override;


public:
	void	Init() override;
	void	Shutdown() override;
	void	Start() override;
	void	Tick() override;
	void	End() override;


public:
	template <class T, class = IsGameObject<T>>
	T*				TGetGameObject();
	template <class T, class = IsGameObject<T>>
	std::vector<T*> TGetGameObjects();
	template <class T, class = IsGameObject<T>>
	void			TAttachGameObject(T* InTarget);
	template <class T, class = IsGameObject<T>>
	void			TDetachGameObject(T* InTarget);
	

	template <typename T, typename... Args>
	T&														TCreateGameObject(Args&&... Arguments);
	const std::vector<std::shared_ptr<OGameObject>>&		GetGameObjects_Deprecated() const { return GameObjects_Deprecated; }
	
	FORCEINLINE GameObjectHashMap* GetGameObjects() { return &GameObjects; }

private:
	std::vector<std::shared_ptr<OGameObject>>	GameObjects_Deprecated;
	GameObjectHashMap GameObjects;


};

template<class T, class>
inline T* OWorld::TGetGameObject()
{
	

	return nullptr;
}

template<class T, class>
inline std::vector<T*> OWorld::TGetGameObjects()
{
	return std::vector<T*>();
}

template<class T, class>
inline void OWorld::TAttachGameObject(T* InTarget)
{
	// nullptr이면 함수에러
	if (!InTarget)
	{
		SConsole::LogError("TAttachGameObject(), param, 'InTarget' is nullptr.");
		return;
	}

	InTarget->SetWorld(this);
}

template<class T, class>
inline void OWorld::TDetachGameObject(T* InTarget)
{
	// nullptr이면 함수에러
	if (!InTarget)
	{
		SConsole::LogError("TAttachGameObject(), param, 'InTarget' is nullptr.");
		return;
	}

	InTarget->SetWorld(nullptr);
}

template <typename T, typename... Args>
T& OWorld::TCreateGameObject(Args&&... Arguments)
{
	// Create and attach.
	std::shared_ptr<T> TGameObject = std::make_shared<T>(std::move(Arguments)...);
	GameObjects_Deprecated.push_back(TGameObject);

	// name as default.
	bool IsDone = false;
	int Index = 0;
	while (!IsDone)
	{
		bool Found = false;

		// Find same name.
		// When is found, raise the index.
		for (const auto& GameObject : GameObjects_Deprecated)
		{
			if (GameObject->Name == ToWString(GetTypeToString<T>() + std::to_string(Index)))
			{
				Found = true;
				Index++;
				break;
			}
		}

		// Not found, set current index to name.
		if (!Found)
		{
			std::static_pointer_cast<T>(TGameObject)->Name = ToWString(GetTypeToString<T>() + std::to_string(Index));
			IsDone = true;
		}
	}

	return *TGameObject;
}
