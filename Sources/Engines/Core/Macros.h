#pragma once

// 솔루션 폴더에서 파일경로를 얻습니다. (확장자 포함시켜주세요!)
#define SOLUTION_FILE_PATH(FILE_NAME) Utls::ToWString(std::string(SOLUTION_DIR) + FILE_NAME)

// 리소스 폴더에서 파일경로를 얻습니다. (확장자 포함시켜주세요!)
#define RESOURCE_FILE_PATH(FILE_NAME) Utls::ToWString(std::string(SOLUTION_DIR) + std::string("Resources\\") + FILE_NAME)

// 쉐이더 폴더에서 파일경로를 얻습니다. (확장자 포함시켜주세요!)
#define SHADER_FILE_PATH(FILE_NAME) Utls::ToWString(std::string(SOLUTION_DIR) + std::string("Shaders\\") + FILE_NAME)