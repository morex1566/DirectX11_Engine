#pragma once

#include <filesystem>
#include <string_view>
#include <sstream>
#include <chrono>
#include <winuser.h>

class Utls
{
public:
	Utls(const Utls&)						= delete;
	Utls& operator=(const Utls&)			= delete;
	Utls(Utls&&) noexcept					= delete;
	Utls& operator=(Utls&&) noexcept		= delete;
	~Utls()  						        = default;


public:
    static std::string ToString(const std::wstring& inWString);
    static std::wstring ToWString(const std::string& inString);
    static float ToRadian(float inDegree);
    static float ToDegree(float inRadian);
    static int GetGap(int inValue1, int inValue2);
    static std::string GetFileName(const std::string& inPath);
    static std::string GetTime();
    static bool StringEndsWith(const std::string& inSource, const std::string& inEnd);


private:
    Utls()                                  = default;
};