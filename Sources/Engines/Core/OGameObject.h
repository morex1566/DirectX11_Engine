#pragma once
#include "CTransform.h"
#include "Object.h"
#include "OWorld.h"
#include "OComponent.h"


class OGameObject : public Object, public IName, public IEnable, public ITag
{
public:
	// T가 Component를 상속받는지 확인
	template <class T> using IsComponent = std::enable_if_t<std::is_base_of<OComponent, T>::value>;

	// Component를 저장하는 해쉬맵 Key = typeid(Component).name(), Value = Component's vector.
	using ComponentHashMap = std::map<std::string, std::vector<OComponent*>>;

	typedef OWorld::GameObjectHashMap GameObjectHashMap;

public:
	OGameObject();
	OGameObject(const OGameObject&)							= default;
	OGameObject& operator=(const OGameObject&)				= default;
	OGameObject(OGameObject&&) noexcept						= default;
	OGameObject& operator=(OGameObject&&) noexcept			= default;
	virtual ~OGameObject() override;


public:
	virtual void					Init() override;
	virtual void					Shutdown() override;
	virtual void					Start() override;
	virtual void					Tick() override;
	virtual void					End() override;


public:
	template <class T, class = IsComponent<T>>
	T* 								TGetComponent();
	template <class T, class = IsComponent<T>>
	std::vector<T*>					TGetComponents();
	template <class T, class = IsComponent<T>>
	void							TAttachComponent(T* InTarget);
	template <class T, class = IsComponent<T>>
	void							TDetachComponent(T* InTarget);
	FORCEINLINE ComponentHashMap&	GetComponents() { return Components; }
	FORCEINLINE GameObjectHashMap&	GetChildren() { return Children; }
	FORCEINLINE CTransform*			GetTransform() const { return Transform; }
	FORCEINLINE OWorld*				GetWorld() const { return World; }
	FORCEINLINE OGameObject*		GetParent() const { return Parent; }
	void							SetWorld(OWorld* InWorld);
	void							SetParent(OGameObject* InParent);


protected:
	CTransform*									Transform;
	OGameObject*								Parent;
	OWorld*										World;
	GameObjectHashMap							Children;
	ComponentHashMap							Components;


};

template<class T, class>
inline T* OGameObject::TGetComponent()
{
	std::string type = typeid(T).name();

	auto ComponentsIt = Components.find(type);
	if (ComponentsIt != Components.end())
	{
		// 캐스팅
		if (T* CastedComponent = dynamic_cast<T*>(Components[type][0]))
		{
			return CastedComponent;
		}
		else
		{
			SConsole::LogWarning(ToWString(type) + L" casting failed.", __FILE__, __LINE__);
			return nullptr;
		}
	}

	SConsole::LogWarning(ToWString(type) + L" is not exist in world.", __FILE__, __LINE__);
	return nullptr;
}

template<class T, class>
inline std::vector<T*> OGameObject::TGetComponents()
{
	std::string type = typeid(T).name();

	auto ComponentsIt = Components.find(type);
	if (ComponentsIt != Components.end())
	{
		// 캐스팅
		std::vector<T*> CastedComponents;
		for (OComponent* Component : Components[type])
		{
			if (T* CastedComponent = dynamic_cast<T*>(Component))
			{
				CastedComponents.push_back(CastedComponent);
			}
			else
			{
				SConsole::LogWarning(ToWString(type) + L" casting failed.", __FILE__, __LINE__);
				return std::vector<T*>();
			}
		}

		return CastedComponents;
	}

	SConsole::LogWarning(ToWString(type) + L" is not exist in world.", __FILE__, __LINE__);
	return std::vector<T*>();
}

template<class T, class>
inline void OGameObject::TAttachComponent(T* InTarget)
{
	// nullptr이면 함수에러
	if (!InTarget)
	{
		SConsole::LogError(L"TAttachGameObject(), param, 'InTarget' is nullptr.", __FILE__, __LINE__);
		return;
	}

	InTarget->SetOwner(this);

	// 해쉬맵에 등록되어있는 Component인지 찾습니다.
	// 등록되어있다면 기존 Type에 바인딩된 vector에, 아니라면 새로운 Type에 vector 바인딩 후 Component 삽입.
	std::string type = typeid(T).name();
	auto ComponentsIt = Components.find(type);
	if (ComponentsIt == Components.end())
	{
		Components[type] = std::vector<OComponent*>();
		Components[type].push_back(InTarget);
	}
	else
	{
		ComponentsIt->second.push_back(InTarget);
	}
}

template<class T, class>
inline void OGameObject::TDetachComponent(T* InTarget)
{
	// nullptr이면 함수에러
	if (!InTarget)
	{
		SConsole::LogError(L"TDetachComponent(), param, 'InTarget' is nullptr.", __FILE__, __LINE__);
		return;
	}

	InTarget->SetOwner(nullptr);

	// 해쉬맵에 등록되어있는 Component인지 탐색
	// 객체의 주소값을 비교하여 삭제할 Component를 탐색
	std::string type = typeid(T).name();
	auto ComponentsIt = Components.find(type);
	if (ComponentsIt != Components.end())
	{
		auto& ComponentsOfType = ComponentsIt->second;
		auto ComponentsOfTypeIt = std::find(ComponentsOfType.begin(),
											ComponentsOfType.end(),
											InTarget);

		// 동일한 객체의 주소값이 있다면 삭제
		if (ComponentsOfTypeIt != ComponentsOfType.end())
		{
			(*ComponentsOfTypeIt)->Shutdown();
			delete (*ComponentsOfTypeIt);
			ComponentsOfType.erase(ComponentsOfTypeIt);

			// ComponentsOfType의 사이즈가 0이면 해쉬를 삭제
			if (ComponentsOfType.empty())
			{
				Components.erase(ComponentsIt);
			}
		}	
	}
}
