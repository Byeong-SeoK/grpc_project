#include "../../include/client_UI.h"
#include <iostream>
#include <vector>

void Client_UI::show_menu()
{
    std::cout << "\033[0;37m";
    std::cout << "========= Choose monitoring service that you want =========" << std::endl;
    std::cout << "1. Memory usage monitoring service" << std::endl;
    std::cout << "2. CPU usage monitoring service" << std::endl;
    std::cout << "3. Disk usage monitoring service" << std::endl;
    std::cout << "4. Process monitoring service" << std::endl;
    std::cout << "5. Log inquiring service" << std::endl;
    std::cout << "6. Log delete service" << std::endl;
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
        this->vecRequest.push_back("Current process PID: ");         // Current process's PID
        this->vecRequest.push_back("Current parent process PID: ");  // Current process's paraent process's PID
        this->vecRequest.push_back("All current process info: ");    // Total current process
        this->vecRequest.push_back("Selected PID process info: \n"); // Process information that user selected
    }
    else if (num == 5)
    {
        std::string LogDate;
        std::string comType;
        std::string LogType;

        std::cout << "Input log record date that you want to inquire: ";
        std::cin >> LogDate;
        std::cout << "Input computer type where the log is record: ";
        std::cin >> comType;
        std::cout << "Input log type that you want to inquire: ";
        std::cin >> LogType;
        std::cout << std::endl;

        this->vecRequest.push_back(LogDate);
        this->vecRequest.push_back(comType);
        this->vecRequest.push_back(LogType);
    }
    else if (num == 6)
    {
        std::string filename;
        std::cout << "Input filename: ";
        std::cin >> filename;

        this->vecRequest.push_back(filename);
    }
}

std::vector<std::string> Client_UI::get_request()
{
    return this->vecRequest; // proto의 request message 여러 개를 한번에 받는 vector
}

void Client_UI::clear_request_vector()
{
    this->vecRequest.clear(); // 기존에 받았던 proto의 request message에 대해 reply를 받아서 이를 해제한다.
}