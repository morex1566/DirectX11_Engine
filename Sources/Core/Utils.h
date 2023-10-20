#pragma once
#include "filesystem"

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