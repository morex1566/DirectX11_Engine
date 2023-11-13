#include "PCH.h"
#include "SConsole.h"
#include "SReflection.h"

std::map<std::type_info, std::vector<void*>>	SReflection::ClassInfos;


SReflection& SReflection::GetInstance()
{
	static SReflection Instance;
	return Instance;
}

void SReflection::Initialize()
{
	for (const auto& Entry : std::filesystem::directory_iterator(GET_CORE_SOURCE_DIR))
	{
		if (Entry.is_regular_file())
		{
			std::string FilePath = Entry.path().generic_string();
			if (StringEndsWith(FilePath, ".h") || StringEndsWith(FilePath, ".hpp"))
			{
				std::ifstream File(FilePath);
				if (File)
				{
					std::string Line;
					while (std::getline(File, Line))
					{
						SConsole::Log(Line);
					}
				}
			}
		}
	}
}