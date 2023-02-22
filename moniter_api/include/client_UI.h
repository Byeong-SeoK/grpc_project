#include <iostream>
#include <vector>

class Client_UI
{
public:
    Client_UI(){};                          // UI 객체 생성자
    void show_menu();                       // UI에서 서비스 메뉴를 보여주는 함수
    void set_request(int num);              // client request를 만드는 함수
    std::vector<std::string> get_request(); // 만든 client request를 받는 함수
    void clear_request_vector();

private:
    std::vector<std::string> vecRequest;
};