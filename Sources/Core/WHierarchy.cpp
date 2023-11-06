#include "PCH.h"
#include "WHierarchy.h"

WHierarchy::WHierarchy()
	: OWidget()
{
}

WHierarchy::~WHierarchy()
{
}

Object::EHandleResultType WHierarchy::Initialize()
{
	OWidget::Initialize();

	return EHandleResultType::Success;
}

void WHierarchy::Release()
{
	OWidget::Release();
}

void WHierarchy::Start()
{
	OWidget::Start();
}

void WHierarchy::Tick()
{
	OWidget::Tick();
}

void WHierarchy::End()
{
	OWidget::End();
}

void WHierarchy::Render()
{
	// Create your GUI here
	ImGui::Begin("Hierarchy");


	ImGui::End();
}
