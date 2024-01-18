#pragma once

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