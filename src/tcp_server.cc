//
// created by David on the: 2023-12-22
//

#if _WIN32
#include <C:\dev\vcpkg\packages\boost-asio_x86-windows\include\boost\asio.hpp>
#endif

#if __linux__
#include </usr/include/boost/asio.hpp>
#endif

// STL: General
#include <cstdio>
#include <cstdlib>
#include <iostream>

// STL: Memory Management
#include <condition_variable>
#include <ctime>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <typeinfo>
#include <vector>

class TCPConnector : public std::enable_shared_from_this<TCPConnector>
{
  boost::asio::ip::tcp::socket _socket;
  std::string _message;

  TCPConnector(boost::asio::io_context& io_context) : _socket(io_context)
  {
  }

public:
  typedef boost::shared_ptr<TCPConnector> pointer;

  static void handle()
  {
    boost::asio::placeholders::error;
    boost::asio::placeholders::bytes_transferred;
  }

  static pointer create(boost::asio::io_context& io_context)
  {
    return pointer(new TCPConnector(io_context));
  }

  boost::asio::ip::tcp::socket& socket()
  {
    return this->_socket;
  }

  void start()
  {
    this->_message = "";

    using namespace std::placeholders;

    boost::asio::async_write(
        this->_socket, boost::asio::buffer(this->_message),
        std::bind(&TCPConnector::handle));
  }
};

class TCPServer
{
  boost::asio::io_context& _io_context;
  boost::asio::ip::tcp::acceptor _acceptor;
  TCPConnector *_connector{};

  void handle
      (
          TCPConnector::pointer vector,
          const boost::system::error_code& error
      )
  {
    this->_connector;
    boost::asio::placeholders::error;
  }

  void start()
  {
    this->_acceptor.async_accept(this->_connector->socket(),
        std::bind(&TCPConnector::handle));
  }

public:
  TCPServer(boost::asio::io_context& io_context) :
      _io_context(io_context),
      _acceptor
          (
              io_context,
              boost::asio::ip::tcp::endpoint
                  (
                      boost::asio::ip::tcp::v4(),
                      13
                  )
          )
  {
    start();
  }
};

int main()
{
  try
  {
    boost::asio::io_context io_context;
    TCPServer serving_tcp(io_context);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}
