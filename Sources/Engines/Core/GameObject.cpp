#include "PCH.h"
#include "GameObject.h"

GameObject::GameObject()
	: IEnable(), ITag(), IName()
{
}

void GameObject::Start()
{
}

void GameObject::Update()
{
}

void GameObject::Shutdown()
{
    // components 정리
    {
        for (Component* component : components) {
            delete component;
        }

        components.clear();
    }
}

// 컴포넌트를 부착합니다. 'attachedComponent'에는 동적할당된 Component만 넣어주세요!
Component* GameObject::AttachComponent(Component* attachedComponent)
{
    // 주어진 attachedComponent가 NULL이면 함수를 종료합니다.
    if (!attachedComponent)
    {
        Console::LogWarning(L"param, 'attachedComponent' is nullptr.", __FILE__, __LINE__);
        return nullptr;
    }

	components.push_back(attachedComponent);

    return components.back();
}

// 주소값이 동일한 Component를 제거합니다.
bool GameObject::DetachComponent(Component* detachedComponent)
{
    // 주어진 detachedComponent가 NULL이면 함수를 종료합니다.
    if (!detachedComponent)
    {
        Console::LogWarning(L"param, 'detachedComponent' is nullptr.", __FILE__, __LINE__);
        return false;
    }

    auto it = std::find(components.begin(), components.end(), detachedComponent);

    // 만약 찾았다면 제거하고 true를 반환합니다.
    if (it != components.end())
    {
        components.erase(it);
        return true;
    }

    return false;
}
