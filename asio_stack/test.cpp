#include <iostream>
#include "tace_net.h"

enum class MsgTypes : uint32_t
{
    Request,
    Post
};

int main(){
    tace::net::message<MsgTypes> msg;

    msg.header.id = MsgTypes::Request;

    int a = 1;
    bool b = true;
    float c = 3.14159f;

    msg << a << b << c;

    a = 99;
    b = true;
    c = 0.01f;
    
    msg >> c >> b >> a;

    std::cout << a << b << c << std::endl;

    return 0;
}