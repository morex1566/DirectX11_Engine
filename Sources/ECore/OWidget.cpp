#include "PCH.h"
#include "SApplication.h"
#include "OWidget.h"


OWidget::OWidget()
	: Object()
{
}

OWidget::~OWidget()
{
}

void OWidget::Initialize()
{
	Object::Initialize();
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