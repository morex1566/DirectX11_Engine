#include "PCH.h"
#include "WTools.h"

WTools::WTools(const OWorld& InWorld)
	: OWidget()
{
	World = &InWorld;
}

WTools::~WTools()
{
}

Object::EHandleResultType WTools::Initialize()
{
	OWidget::Initialize();

	return EHandleResultType::Success;
}

void WTools::Release()
{
	OWidget::Release();
}

void WTools::Start()
{
	OWidget::Start();
}

void WTools::Tick()
{
	OWidget::Tick();
}

void WTools::End()
{
	OWidget::End();
}

void WTools::Render()
{
	// Create your GUI here
	ImGui::Begin("Tools");

	ImGui::End();
}