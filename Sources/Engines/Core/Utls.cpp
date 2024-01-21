#include "PCH.h"
#include "Utls.h"

std::string Utls::ToString(const std::wstring& inWString)
{
    char multiByteBuffer[256] = "";

    WideCharToMultiByte(CP_ACP, 0, inWString.c_str(), -1, multiByteBuffer, sizeof(multiByteBuffer), nullptr, nullptr);

    return std::string(multiByteBuffer);
}

std::wstring Utls::ToWString(const std::string& inString)
{
    wchar_t wideByteBuffer[256] = L"";

    MultiByteToWideChar(CP_ACP, 0, inString.c_str(), -1, wideByteBuffer, sizeof(wideByteBuffer) / sizeof(wideByteBuffer[0]));

    return std::wstring(wideByteBuffer);
}

float Utls::ToRadian(float inDegree)
{
    return inDegree * 0.0174532925f;
}

float Utls::ToDegree(float inRadian)
{
    return inRadian * (180.0f / 3.14159265f);
}

int Utls::GetGap(int inValue1, int inValue2)
{
    if (inValue1 > inValue2)
    {
        return inValue1 - inValue2;
    }
    else
    {
        return inValue2 - inValue1;
    }
}

std::string Utls::GetFileName(const std::string& inPath)
{
    // Create the file path string.
    std::filesystem::path path = inPath;

    // Extract the file name.
    std::string fileName = path.filename().string();

    // Erase the file extension.
    size_t dotPosition = fileName.find_last_of('.');
    if (dotPosition != std::string::npos) {
        fileName = fileName.substr(0, dotPosition);
    }

    return fileName;
}

std::string Utls::GetTime()
{
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

bool Utls::StringEndsWith(const std::string& inSource, const std::string& inEnd)
{
    if (inSource.size() < inEnd.size()) {
        return false;
    }
    std::string_view StringView = inSource;
    std::string_view SuffixView = inEnd;

    return StringView.substr(StringView.size() - SuffixView.size()) == SuffixView;
}
