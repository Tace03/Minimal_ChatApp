#define ASIO_STANDALONE
#define _WIN32_WINNT 0x0A00 // Target Windows 10 API

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include <iostream>
#include <vector>
#include <thread>

// Initialize a common buffer to read
std::vector<char> vBuffer(1024);

void GrabSomeData(asio::ip::tcp::socket& socket){
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
        [&](std::error_code ec, std::size_t length)
        {
            if(!ec){
                std::cout << "\n\nRead " << length << " bytes\n\n";

                for(int i = 0 ; i < length ; i++){
                    std::cout << vBuffer[i];
                }

                GrabSomeData(socket);
            } else{
                std::cout << "Error : " << ec.message() << std::endl;
            }
        }
    );
}

int main() {
    try {
        asio::error_code ec;
        asio::io_context io_context;

        asio::io_context::work idleWork(io_context);

        std::thread thrContext = std::thread([&]() { io_context.run(); } );

        asio::ip::tcp::resolver resolver(io_context);
        
        // Resolve the hostname with error handling
        auto endpoints = resolver.resolve("51.38.81.49", "http", ec);
        
        if (ec) {
            std::cerr << "Resolver error: " << ec.message() << std::endl;
            return 1;
        }

        // Create and connect the socket with error handling
        asio::ip::tcp::socket socket(io_context);
        asio::connect(socket, endpoints, ec);
        if (ec) {
            std::cerr << "Connection error: " << ec.message() << std::endl;
            return 1;
        }

        if(socket.is_open()){

            GrabSomeData(socket);

            std::string sRequest = 
                "GET /index.html HTTP/1.1\r\n"
                "Host: david-barr.co.uk\r\n"
                "Connection: close\r\n\r\n";
            
            socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(20000ms);

        }

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
