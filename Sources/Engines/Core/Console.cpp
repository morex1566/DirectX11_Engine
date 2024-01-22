#pragma warning (disable : 6385)

#include "PCH.h"
#include "System.h"

bool		Console::isConsoleEnabled;

Console::Console()
{
	isConsoleEnabled = Config::GetConfig()["Console"]["IsConsoleEnabled"];
}

void __stdcall Console::MsgHandler(HWND HWnd, UINT Msg, WPARAM WParam, LPARAM LParam)
{
	if (!isConsoleEnabled)
	{
		return;
	}

}

void Console::Log(const std::wstring& Log)
{
	if (!isConsoleEnabled)
	{
		return;
	}

	// 흰색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 7);
	}

	std::wcout << L"[" << Utls::ToWString(Utls::GetTime()) << L"]"
		<< L"[Log] " << Log << std::endl;
}

void Console::LogSuccess(const std::wstring& Log)
{
	if (!isConsoleEnabled)
	{
		return;
	}

	// 흰색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 10);
	}

	std::wcout << L"[" << Utls::ToWString(Utls::GetTime()) << L"]"
		<< L"[LogSuccess] " << Log << std::endl;
}

void Console::LogWarning(const std::wstring& Log, const std::string& inFIleName, int inLine)
{
	if (!isConsoleEnabled)
	{
		return;
	}

	// 주황색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 6);
	}

	std::wcout << L"[" << Utls::ToWString(Utls::GetTime()) << L"]"
		<< L"[LogWarning] " << Log << std::endl;

	logCallstack();
}

void Console::LogError(const std::wstring& Log, const std::string& inFIleName, int inLine)
{
	if (!isConsoleEnabled)
	{
		return;
	}

	// 붉은색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
	}

	std::wcout << L"[" << Utls::ToWString(Utls::GetTime()) << L"]"
		<< L"[LogError]" << 
		L"[" << Utls::ToWString(inFIleName) << L"]" <<
		L"[" << Utls::ToWString(std::to_string(inLine)) << L"] " <<
		Log 
		<< std::endl;

	logCallstack();
}

void Console::Init()
{
	if (!isConsoleEnabled)
	{
		return;
	}

	FILE* Stream;
	errno_t		Error;

	// 콘솔창 생성
	{
		AllocConsole();

		hWindow = GetConsoleWindow();
		if (hWindow == nullptr)
		{
			throw std::exception();
		}

		Error = freopen_s(&Stream, "CONOUT$", "w", stdout);
		if (Error)
		{
			throw std::exception();
		}
	}

	// 함수명을 불러들임
	SymInitialize(GetCurrentProcess(), nullptr, TRUE);
}

void Console::Shutdown()
{
	if (!isConsoleEnabled)
	{
		return;
	}

	DestroyWindow(hWindow);
}

bool Console::isInternalFunction(const std::string& functionName)
{
	// 내부 함수를 필터링하기 위한 패턴이나 키워드 추가
	if (functionName.find("invoke_main") != std::string::npos ||
		functionName.find("__scrt_common_main") != std::string::npos ||
		functionName.find("WinMainCRTStartup") != std::string::npos ||
		functionName.find("BaseThreadInitThunk") != std::string::npos ||
		functionName.find("RtlUserThreadStart") != std::string::npos)
	{
		return true; // 내부 함수로 판단
	}

	return false; // 내부 함수 아님
}

void Console::logCallstack()
{
	const DWORD maxStackFrames = 63;
	void* stackFrames[maxStackFrames];
	USHORT frames = CaptureStackBackTrace(0, maxStackFrames, stackFrames, nullptr);

	std::cout << "Callstack is..." << std::endl;

	// 여기서 i >= 2는 logCallStack(), LogWarning()을 콜스택에서 제외한 것입니다. 
	for (USHORT i = frames - 1; i >= 2; i--)
	{
		// 각 함수에 대한 주소를 함수 이름으로 변환하여 출력
		ULONG64 displacement;
		char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(char)];
		SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(buffer);
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = MAX_SYM_NAME;

		// 주소값을 통해 함수명을 로드
		if (SymFromAddr(GetCurrentProcess(), reinterpret_cast<DWORD64>(stackFrames[i]), &displacement, symbol))
		{
			std::string functionName(symbol->Name);

			// main함수 이전의 내부 이벤트함수는 제외시킴
			if (isInternalFunction(functionName))
			{
				continue;
			}

			// 마지막이면
			int j = i;
			if (--j < 2)
			{
				std::cout << "  " << functionName << "()" << " <------ problem occurred." << std::endl;
			}
			else
			{
				std::cout << "  " << functionName << "()" << std::endl;
			}
		}
		else
		{
			std::cout << "  [Unknown Function]\n";
		}
	}
}