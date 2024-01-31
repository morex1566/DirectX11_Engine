#include "PCH.h"
#include "CAnimator.h"
#include "CModel.h"

CAnimator::CAnimator(OGameObject* InOwner)
	: OComponent(InOwner)
{
}

CAnimator::~CAnimator()
{
}

void CAnimator::Init()
{
	OComponent::Init();
}

void CAnimator::Shutdown()
{
	OComponent::Shutdown();
}

void CAnimator::Start()
{
	OComponent::Start();

}

void CAnimator::Tick()
{
	OComponent::Tick();

}

void CAnimator::End()
{
	OComponent::End();

}

void CAnimator::LoadModel(CModel* InModel)
{
	Model = InModel;
}
