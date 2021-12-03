// AC-Automation-Recognition.cpp: 定义应用程序的入口点。
//

#include "AC-Automation-Recognition.hpp"
#include <cstring>
#include <string>

AC_Automation AC;

int main(int argc, char* argv[]) {
    AC.readwords(std::string(argv[1], strlen(argv[1])), AC.ReadFileMode);
    //printf("****************");
    AC.query(std::string(argv[2], strlen(argv[2])), std::string(argv[3], strlen(argv[3])), AC.ReadFileMode);
    return 0;
}