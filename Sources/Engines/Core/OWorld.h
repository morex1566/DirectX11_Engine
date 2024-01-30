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
	T*								TGetGameObject();
	template <class T, class = IsGameObject<T>>
	std::vector<T*>&				TGetGameObjects();
	template <class T, class = IsGameObject<T>>
	void							TAttachGameObject(T* InTarget);
	template <class T, class = IsGameObject<T>>
	void							TDetachGameObject(T* InTarget);	
	FORCEINLINE GameObjectHashMap&	GetGameObjects() { return GameObjects; }


private:
	GameObjectHashMap GameObjects;


};

template<class T, class>
inline T* OWorld::TGetGameObject()
{
	std::string type = typeid(T).name();

	auto GameObjectsIt = GameObjects.find(type);
	if (GameObjectsIt != GameObjects.end())
	{
		return GameObjects[type][0];
	}

	SConsole::LogWarning(ToWString(type) + L" is not exist in world.", __FILE__, __LINE__);
	return nullptr;
}

template<class T, class>
inline std::vector<T*>& OWorld::TGetGameObjects()
{
	std::string type = typeid(T).name();

	auto GameObjectsIt = GameObjects.find(type);
	if (GameObjectsIt != GameObjects.end())
	{
		return GameObjects[type];
	}

	SConsole::LogWarning(ToWString(type) + L" is not exist in world.", __FILE__, __LINE__);
	return std::vector<T*>();
}

template<class T, class>
inline void OWorld::TAttachGameObject(T* InTarget)
{
	// nullptr이면 함수에러
	if (!InTarget)
	{
		SConsole::LogError(L"TAttachGameObject(), param, 'InTarget' is nullptr.", __FILE__, __LINE__);
		return;
	}

	// 인스턴싱된 월드를 등록
	InTarget->SetWorld(this);

	// 해쉬맵에 등록되어있는 GameObject인지 찾습니다.
	// 등록되어있다면 기존 Type에 바인딩된 vector에, 아니라면 새로운 Type에 vector 바인딩 후 객체 삽입.
	std::string type = typeid(T).name();
	auto GameObjectsIt = GameObjects.find(type);
	if (GameObjectsIt == GameObjects.end())
	{
		GameObjects[type] = std::vector<OGameObject*>();
		GameObjects[type].push_back(InTarget);
	}
	else
	{
		GameObjectsIt->second.push_back(InTarget);
	}
}

template<class T, class>
inline void OWorld::TDetachGameObject(T* InTarget)
{
	// nullptr이면 함수에러
	if (!InTarget)
	{
		SConsole::LogError(L"TAttachGameObject(), param, 'InTarget' is nullptr.", __FILE__, __LINE__);
		return;
	}

	// 월드목록에서 이 객체를 제거
	InTarget->SetWorld(nullptr);

	// 해쉬맵에 등록되어있는 GameObject인지 탐색
	// 객체의 주소값을 비교하여 삭제할 GameObject를 탐색
	std::string type = typeid(T).name();
	auto GameObjectsIt = GameObjects.find(type);
	if (GameObjectsIt != GameObjects.end())
	{
		auto& GameObjectsOfType = GameObjectsIt->second;
		auto GameObjectsOfTypeIt = std::find(GameObjectsOfType.begin(),
											 GameObjectsOfType.end(),
											 InTarget);

		// 동일한 객체의 주소값이 있다면 삭제
		if (GameObjectsOfTypeIt != GameObjectsOfType.end())
		{
			(*GameObjectsOfTypeIt)->Shutdown();
			delete (*GameObjectsOfTypeIt);
			GameObjectsOfType.erase(GameObjectsOfTypeIt);

			// GameObjectsOfType의 사이즈가 0이면 해쉬를 삭제
			if (GameObjectsOfType.empty())
			{
				GameObjects.erase(GameObjectsIt);
			}
		}	
	}
}