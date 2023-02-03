#include "../../include/save_log.h"
#include <glog/logging.h>
#include <gflags/gflags.h>

void SaveLog::save_level_Log(std::string log, int level)
{
    if (level == 0)
    {
        std::string DirLog = "../../../../file/"; // 별다른 값을 주지 않고 default로 냅두면 src/cmake/build에 저장된다.
        std::string strLogFileName = DirLog + "INFO_log.txt";

        FILE *fp = fopen(strLogFileName.c_str(), "a");
        if (fp == NULL)
        {
            puts("fail to open file pointer");
            return;
        }
    }
    else if (level == 1)
    {
        std::string DirLog = "../../../../file/"; // 별다른 값을 주지 않고 default로 냅두면 src/cmake/build에 저장된다.
        std::string strLogFileName = DirLog + "WARNING_log.txt";

        FILE *fp = fopen(strLogFileName.c_str(), "a");
        if (fp == NULL)
        {
            puts("fail to open file pointer");
            return;
        }
    }
    else if (level == 2)
    {
        std::string DirLog = "../../../../file/"; // 별다른 값을 주지 않고 default로 냅두면 src/cmake/build에 저장된다.
        std::string strLogFileName = DirLog + "ERROR_log.txt";

        FILE *fp = fopen(strLogFileName.c_str(), "a");
        if (fp == NULL)
        {
            puts("fail to open file pointer");
            return;
        }
    }
    else if (level == 3)
    {
        std::string DirLog = "../../../../file/"; // 별다른 값을 주지 않고 default로 냅두면 src/cmake/build에 저장된다.
        std::string strLogFileName = DirLog + "FATAL_log.txt";

        FILE *fp = fopen(strLogFileName.c_str(), "a");
        if (fp == NULL)
        {
            puts("fail to open file pointer");
            return;
        }
    }
    else
    {
        std::cout << "Wrong log level" << std::endl;
        return;
    }

    if (fp != NULL)
    {
        fclose(fp);
    }

    return;
}