#include "PCH.h"
#include "OApplication.h"
#include "OWidget.h"


OWidget::OWidget()
{
}

OWidget::~OWidget()
{
}

Object::EHandleResultType OWidget::Initialize()
{
	Object::Initialize();

	return EHandleResultType::Success;
}

void OWidget::Release()
{
	Object::Release();
}

void OWidget::Start()
{
	Object::Start();
}

void OWidget::Tick()
{
	Object::Tick();

	Render();
} 

void OWidget::End()
{
	Object::End();
}