#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_connection.h"

namespace net{
    template<typename T>
    class clien_inerface{
    public:
        clien_inerface()
            : m_socket(m_context){
        }
        virtual ~clien_inerface(){
            Disconnect();
        }
    public:
        bool Connect(const std::string & host, const uint16_t port){
            try {

                boost::asio::ip::tcp::resolver resolver(m_context);
                boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host,std::to_string(port));

                m_connection = std::make_unique<connection<T>>();//TODO

                m_connection->ConnectToServer(endpoints);
                thrContext = std::thread([this](){m_context.run();});
            }
            catch (std::exception &ec){
                std::cerr<<"Client Exception: " << ec.what()<<'\n';
                return false;
            }
            return true;
        }
        void Disconnect(){
            if(m_connection) {
                m_connection->Disconect();
            }
            m_context.stop();
            if(thrContext.joinable()) {
                thrContext.join();
            }
            m_connection.release();
        }
        bool IsConnect(){
            if(m_connection) {
                return m_connection->IsConnect();
            }
            return false;
        }
        tsqueue<owned_message<T>> &  InComing(){
            return m_qMassageIn;
        }
    protected:
        boost::asio::io_context m_context;
        std::thread thrContext;
        boost::asio::ip::tcp::socket m_socket;
        std::unique_ptr<connection<T>> m_connection;
    private:
        tsqueue<owned_message<T>> m_qMassageIn;
    };
}
