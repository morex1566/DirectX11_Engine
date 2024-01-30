#pragma warning (disable : 6385)

#include "PCH.h"
#include "SApplication.h"
#include "SConsole.h"

uint8		SConsole::bIsConsoleEnabled;


SConsole& SConsole::GetInstance()
{
	static SConsole Instance;
	return Instance;
}

void SConsole::Init()
{
	FILE*		Stream;
	errno_t		Error;

	AllocConsole();

	HWnd = GetConsoleWindow();
	if (HWnd == nullptr)
	{
		throw std::exception();
	}

	Error = freopen_s(&Stream, "CONOUT$", "w", stdout);
	if (Error)
	{
		throw std::exception();
	}
}

void SConsole::Shutdown()
{
	DestroyWindow(HWnd);
}

void SConsole::Log(const std::wstring& Log)
{
	// 흰색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 7);
	}

	std::wcout << Log << std::endl;
}

void SConsole::Log(const std::string& Log)
{
	// 흰색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 7);
	}

	std::cout << Log << std::endl;
}

void SConsole::LogWarning(const std::wstring& Log, const std::string& inFIleName, int inLine)
{
	// 주황색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 6);
	}

	std::wcout << L"[" << GetCurrentTimeAsWString() << L"]" << 
				L"[LogWarning] " << 
				L"[" << ToWString(inFIleName) << L"]" <<
				L"[" << ToWString(std::to_string(inLine)) << L"] " <<
				Log << 
				std::endl;
}

void SConsole::LogWarning(const std::string& Log, const std::string& inFIleName, int inLine)
{
	// 주황색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 6);
	}

	std::cout << L"[" << GetCurrentTimeAsString() << L"]" <<
				 L"[LogWarning] " <<
				 L"[" << (inFIleName) << L"]" <<
				 L"[" << (std::to_string(inLine)) << L"] " <<
				 Log << 
				 std::endl;
}

void SConsole::LogError(const std::wstring& Log, const std::string& inFIleName, int inLine)
{
	// 붉은색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
	}

	std::wcout << L"[" << GetCurrentTimeAsWString() << L"]" << 
				  L"[LogError] " << 
				  L"[" << ToWString(inFIleName) << L"]" <<
				  L"[" << ToWString(std::to_string(inLine)) << L"] " <<
				  Log << 
				  std::endl;

	LogCallStack();

	throw std::runtime_error(ToString(Log));
}


void SConsole::LogError(const std::string& Log, const std::string& inFIleName, int inLine)
{
	// 붉은색으로 설정
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
	}

	std::cout << L"[" << GetCurrentTimeAsString() << L"]" <<
				 L"[LogError] " <<
				 L"[" << (inFIleName) << L"]" <<
				 L"[" << (std::to_string(inLine)) << L"] " <<
				 Log << 
				 std::endl;

	LogCallStack();

	throw std::runtime_error(Log);
}

const HWND& SConsole::GetHWnd() const
{
	return HWnd;
}

bool SConsole::IsInternalFunction(const std::string& functionName)
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

void SConsole::LogCallStack()
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
			if (IsInternalFunction(functionName))
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