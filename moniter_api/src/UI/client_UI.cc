#include "../../include/client_UI.h"
#include <vector>

void Client_UI::show_menu()
{
    std::cout << "========= Choose monitoring service that you want =========" << std::endl;
    std::cout << "1. Memory usage monitoring service" << std::endl;
    std::cout << "2. CPU usage monitoring service" << std::endl;
    std::cout << "3. Disk usage monitoring service" << std::endl;
    std::cout << "4. Process monitoring service" << std::endl;
    std::cout << "5. Simple echo type log monitoring service" << std::endl;
    std::cout << "0. Exit the monitoring" << std::endl;
}

void Client_UI::set_request(int num)
{
    if (num == 1)
    {
        this->vecRequest.push_back("Current usage of virtual memory volume: ");      // 현재 사용 중인 가상 메모리 용량
        this->vecRequest.push_back("Current usage of physical memeory volume: ");    // 현재 사용 중인 물리적 메모리 용량
        this->vecRequest.push_back("Current available of virtual memory volume: ");  // 현재 가용한 가상 메모리 용량
        this->vecRequest.push_back("Current available of physical memory volume: "); // 현재 가용한 물리적 메모리 용량
    }
    else if (num == 2)
    {
        this->vecRequest.push_back("Current usage of CPU volume: ");
    }
    else if (num == 3)
    {
        this->vecRequest.push_back("Current total volume of disk: ");     // Current disk volume size that mount on root directory (linux)
        this->vecRequest.push_back("Current usage volume of disk: ");     // Current used disk volume size that mount on root directory (linux)
        this->vecRequest.push_back("Current available volume of disk: "); // Current available disk volume size that mount on root directory (linux)
    }
    else if (num == 4)
    {
        this->vecRequest.push_back("Current process PID: ");        // Current process's PID
        this->vecRequest.push_back("Current parent process PID: "); // Current process's paraent process's PID
        this->vecRequest.push_back("All current process info: ");   // Total current process
    }
}

std::vector<std::string> Client_UI::get_request()
{
    return this->vecRequest;
}

void Client_UI::clear_request_vector()
{
    this->vecRequest.clear();
}