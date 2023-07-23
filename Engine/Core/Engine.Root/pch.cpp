#include "pch.h"

int Config::GET_ENGINE_SETTING_WINDOW_WIDTH()
{
	// Read json file.
	std::ifstream jsonIFile;
	jsonIFile.open(FILE_PATH("config.json"), std::ios_base::binary);
	Json::Value root;
	jsonIFile >> root;

	jsonIFile.close();

	return root["Engine"]["Setting"]["Window"]["Width"].asInt();
}

int Config::SET_ENGINE_SETTING_WINDOW_WIDTH(int value)
{
	// Read json file.
	std::ifstream jsonIFile;
	jsonIFile.open(FILE_PATH("config.json"), std::ios_base::binary);
	Json::Value root;
	jsonIFile >> root;

	// Change json value.
	{
		root["Engine"]["Setting"]["Window"]["Width"] = value;
	}

	// Write json file.
	Json::StyledWriter writer;
	std::string jsonString = writer.write(root);
	std::ofstream jsonOFile;
	jsonOFile.open(FILE_PATH("config.json"), std::ios_base::binary);

	jsonOFile << jsonString;

	jsonIFile.close();
	jsonOFile.close();

	return root["Engine"]["Setting"]["Window"]["Width"].asInt();
}

int Config::GET_ENGINE_SETTING_WINDOW_HEIGHT()
{
	// Read json file.
	std::ifstream jsonIFile;
	jsonIFile.open(FILE_PATH("config.json"), std::ios_base::binary);
	Json::Value root;
	jsonIFile >> root;

	jsonIFile.close();

	return root["Engine"]["Setting"]["Window"]["Height"].asInt();
}

int Config::SET_ENGINE_SETTING_WINDOW_HEIGHT(int value)
{
	// Read json file.
	std::ifstream jsonIFile;
	jsonIFile.open(FILE_PATH("config.json"), std::ios_base::binary);
	Json::Value root;
	jsonIFile >> root;

	// Change json value.
	{
		root["Engine"]["Setting"]["Window"]["Height"] = value;
	}

	// Write json file.
	Json::StyledWriter writer;
	std::string jsonString = writer.write(root);
	std::ofstream jsonOFile;
	jsonOFile.open(FILE_PATH("config.json"), std::ios_base::binary);

	jsonOFile << jsonString;

	jsonIFile.close();
	jsonOFile.close();

	return root["Engine"]["Setting"]["Window"]["Height"].asInt();
}

int Config::GET_ENGINE_SETTING_WINDOW_VSYNC()
{
	// Read json file.
	std::ifstream jsonIFile;
	jsonIFile.open(FILE_PATH("config.json"), std::ios_base::binary);
	Json::Value root;
	jsonIFile >> root;

	jsonIFile.close();

	return root["Engine"]["Setting"]["Window"]["Vsync"].asInt();
}

int Config::GET_ENGINE_SETTING_WINDOW_FULLSCREEN()
{
	// Read json file.
	std::ifstream jsonIFile;
	jsonIFile.open(FILE_PATH("config.json"), std::ios_base::binary);
	Json::Value root;
	jsonIFile >> root;

	jsonIFile.close();

	return root["Engine"]["Setting"]["Window"]["FullScreen"].asInt();
}

float Config::GET_ENGINE_SETTING_WINDOW_SCREENDEPTH()
{
	// Read json file.
	std::ifstream jsonIFile;
	jsonIFile.open(FILE_PATH("config.json"), std::ios_base::binary);
	Json::Value root;
	jsonIFile >> root;

	jsonIFile.close();

	return root["Engine"]["Setting"]["Window"]["ScreenDepth"].asFloat();
}

float Config::GET_ENGINE_SETTING_WINDOW_SCREENNEAR()
{
	// Read json file.
	std::ifstream jsonIFile;
	jsonIFile.open(FILE_PATH("config.json"), std::ios_base::binary);
	Json::Value root;
	jsonIFile >> root;

	jsonIFile.close();

	return root["Engine"]["Setting"]["Window"]["ScreenNear"].asFloat();
}
