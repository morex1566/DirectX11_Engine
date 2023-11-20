#pragma once

#define GET_SHADER_FILE_DIR(SHADER_FILE_NAME) std::string(SHADERS_FOLDER_DIR) + SHADER_FILE_NAME
#define GET_CORE_SOURCE_DIR (std::string(SOLUTION_DIR) + "Sources\\ECore\\")


#define ECLASS() /*REFLECTION_ECLASS*/
#define EPROPERTY() /*REFLECTION_EPROPERTY*/

#define GENERATED_ECLASS(TYPE_NAME)			     \
void foo(TYPE_NAME par, <TYPE_NAME>)             \
{                                                \
                                                 \
}											     \