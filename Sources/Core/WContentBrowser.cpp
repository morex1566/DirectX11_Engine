#include "PCH.h"
#include "WContentBrowser.h"

WContentBrowser::WContentBrowser(const OWorld& InWorld)
{
}

WContentBrowser::~WContentBrowser()
{
}

Object::EHandleResultType WContentBrowser::Initialize()
{
	OWidget::Initialize();

	return EHandleResultType::Success;
}

void WContentBrowser::Release()
{
	OWidget::Release();
}

void WContentBrowser::Start()
{
	OWidget::Start();
}

void WContentBrowser::Tick()
{
	OWidget::Tick();
}

void WContentBrowser::End()
{
	OWidget::End();
}

void WContentBrowser::Render()
{
	// Create your GUI here
	ImGui::Begin("ContentBrowser");


	ImGui::End();
}
