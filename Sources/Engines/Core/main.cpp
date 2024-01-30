#include "PCH.h"
#include "SApplication.h"
#include "SConsole.h"
#include "SReflection.h"


int WINAPI WinMain(HINSTANCE HInstance, HINSTANCE HPrevInstance, PSTR Scmdline, int Cmdshow)
{

	// Console::LogError의 CallStack을 찾기위해 호출
	BOOL r = SymInitialize(GetCurrentProcess(), nullptr, TRUE);
	if (!r)
	{
		return 0;
	}


	// DirectXTex의 Texture를 만들기위해 호출
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		return 0;
	}


	// 콘솔 생성
	// 디버그 매세지 등, 여기에
	SConsole& Console = SConsole::GetInstance();
	{
		Console.Init();
	}


	// 리플렉션
	SReflection& Reflection = SReflection::GetInstance();
	{
		Reflection.Init();
	}


	// 어플리케이션
	// 게임 컨텐츠, 윈도우 프로그램과 관련된 내용은 여기에
	SApplication& Application = SApplication::GetInstance();
	{
		Application.Init(HInstance);
	}


	// 어플리케이션 메인 플로우
	{
		Application.Start();

		while (SApplication::CheckIsPlaying())
		{
			MSG Msg;
			while(::PeekMessage(&Msg, nullptr, 0U, 0U, PM_REMOVE))
			{
				::TranslateMessage(&Msg);
				::DispatchMessage(&Msg);
			}

			Application.Tick();
			Application.Draw();
		}

		Application.End();
	}


	// 인스턴스의 맴버들 메모리 해제
	{
		Application.Shutdown();
		Console.Shutdown();
	}

	return 0;
}