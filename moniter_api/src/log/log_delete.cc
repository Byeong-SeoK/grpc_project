#include "../../include/moniter_client.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <typeinfo>
#include <unistd.h>

#include "sys/sysinfo.h"
#include "sys/types.h"
#include "sys/statvfs.h"
#include "sys/stat.h"

void MoniterClient::log_delete_method(const std::string filename)
{
    std::string dirPath = "/mnt/c/Users/INNO-C-535/grpc_project/log_data/";
    std::string delete_file_path = dirPath + filename + ".txt";
    delete_file_path.erase(std::remove_if(delete_file_path.begin(), delete_file_path.end(), isspace), delete_file_path.end());

    bool result = unlink(delete_file_path.c_str()); // 0이면 올바르게 삭제, -1이면 error 발생

    if (!result)
    {
        std::cout << "delete" << std::endl;
    }
    else
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
    }

    return;
}