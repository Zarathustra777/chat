#include <iostream>
#include <vector>
#include <chrono>
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <thread>

void GrabSomeData(boost::asio::ip::tcp::socket& socket);
std::vector<char> vBuffer (20*1024);

int main() {
    boost::system::error_code ec;
    boost::asio::io_context context;
    boost::asio::io_context::work idleWork(context);
    std::thread thrContext  = std::thread([&](){context.run();});
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address("51.38.81.49", ec), 80);
    boost::asio::ip::tcp::socket socket(context);

    socket.connect(endpoint,ec);

    if(!ec){
        std::cout<<"Connected!"<<std::endl;
    }
    else{
        std::cout<<"Failed to connect to address:\n" << ec.message()<<std::endl;
    }
    if(socket.is_open()){
        GrabSomeData(socket);
        std::string sRequest;
        sRequest = "GET /index.html HTTP/1.1\r\n"
                   "Host: david-bar.co.uk\r\n"
                   "Connection: closer\r\n\r\n";

        socket.write_some(boost::asio::buffer(sRequest.data(),sRequest.size()), ec);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(20000ms);
    }
    return 0;
}

void GrabSomeData(boost::asio::ip::tcp::socket& socket) {
    socket.async_read_some(boost::asio::buffer(vBuffer.data(),vBuffer.size()),
                           [&](std::error_code ec, std::size_t sz)
                               {
                                    if(!ec)
                                    {
                                        std::cout<<"\n\nRead " << sz << "bytes\n\n";
                                        for(int i =0; i <sz; ++i)
                                            std::cout<<vBuffer[i];

                                        GrabSomeData(socket);
                                    }
                               }
                           );
}
