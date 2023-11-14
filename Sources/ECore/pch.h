#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <memory>
#include <cstdint>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <d3d11.h>
#include <iostream>
#include <fstream>
#include <string_view>
#include <sstream>
#include <filesystem>
#include <directxmath.h>
#include <wrl/client.h>

using namespace DirectX;
using namespace Microsoft::WRL;
using int8					= int8_t;
using uint8					= uint8_t;
using int16					= int16_t;
using uint16				= uint16_t;
using int32					= int32_t;
using uint32				= uint32_t;
using int64					= int64_t;
using uint64				= uint64_t;

#include "ETypes.h"
#include "EMacros.h"

static std::string ToString(const std::wstring& wstring_)
{
    char multiByteBuffer[256] = "";

    WideCharToMultiByte(CP_ACP, 0, wstring_.c_str(), -1, multiByteBuffer, sizeof(multiByteBuffer), nullptr, nullptr);

    return std::string(multiByteBuffer);
}

static std::wstring ToWString(const std::string& string_)
{
    wchar_t wideByteBuffer[256] = L"";

    MultiByteToWideChar(CP_ACP, 0, string_.c_str(), -1, wideByteBuffer, sizeof(wideByteBuffer) / sizeof(wideByteBuffer[0]));

    return std::wstring(wideByteBuffer);
}

static std::string GetFileNameToString(const std::string& path_)
{
    // Create the file path string.
    std::filesystem::path path = path_;

    // Extract the file name.
    std::string fileName = path.filename().string();

    // Erase the file extension.
    size_t dotPosition = fileName.find_last_of('.');
    if (dotPosition != std::string::npos) {
        fileName = fileName.substr(0, dotPosition);
    }

    return fileName;
}

static std::wstring GetFileNameToWString(const std::wstring& path_)
{
    // Create the file path string.
    std::filesystem::path path = path_;

    // Extract the file name.
    std::wstring fileName = path.filename().wstring();

    // Erase the file extension.
    size_t dotPosition = fileName.find_last_of(L'.');
    if (dotPosition != std::wstring::npos) {
        fileName = fileName.substr(0, dotPosition);
    }

    return fileName;
}

static std::string GetCurrentTimeAsString() {

    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    struct std::tm timeinfo;
    errno_t errorCode = localtime_s(&timeinfo, &time);

    if (errorCode == 0)
    {
        int hours = timeinfo.tm_hour;
        int minutes = timeinfo.tm_min;
        int seconds = timeinfo.tm_sec;

        std::stringstream timeString;
        timeString << std::setfill('0') << std::setw(2) << hours << ":"
            << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << seconds;

        return timeString.str();
    }

    return "00:00:00";
}

static std::wstring GetCurrentTimeAsWString() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    struct std::tm timeinfo;
    errno_t errorCode = localtime_s(&timeinfo, &time);

    if (errorCode == 0)
    {
        int hours = timeinfo.tm_hour;
        int minutes = timeinfo.tm_min;
        int seconds = timeinfo.tm_sec;

        std::wstringstream timeString;
        timeString << std::setfill(L'0') << std::setw(2) << hours << L":"
            << std::setfill(L'0') << std::setw(2) << minutes << L":"
            << std::setfill(L'0') << std::setw(2) << seconds;

        return timeString.str();
    }

    return L"00:00:00";
}

static bool StringEndsWith(const std::string& InSource, const std::string& InEnd) {
    if (InSource.size() < InEnd.size()) {
        return false;
    }
    std::string_view StringView = InSource;
    std::string_view SuffixView = InEnd;

    return StringView.substr(StringView.size() - SuffixView.size()) == SuffixView;
}

static float ToRadian(float degree_)
{
    return degree_ * 0.0174532925f;
}

static float ToDegree(float radian_)
{
    return radian_ * (180.0f / 3.14159265f);
}

static int GetGap(int Value1, int Value2)
{
    if (Value1 > Value2)
    {
        return Value1 - Value2;
    }
    else
    {
        return Value2 - Value1;
    }
}

static uint32 GetSystemWidth()
{
    return GetSystemMetrics(SM_CXSCREEN);
}

static uint32 GetSystemHeight()
{
    return GetSystemMetrics(SM_CYSCREEN);
}