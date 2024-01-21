#pragma once

// 솔루션 파일을 기준으로 파일을 검색합니다. (확장자 포함시켜주세요!)
#define SOLUTION_GETFILE(FILE_NAME) std::string(SOLUTION_DIR) + FILE_NAME