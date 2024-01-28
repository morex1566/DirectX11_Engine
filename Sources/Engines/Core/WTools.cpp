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

void WTools::Init()
{
	OWidget::Init();
}

void WTools::Shutdown()
{
	OWidget::Shutdown();
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