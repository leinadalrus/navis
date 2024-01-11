//
// created by David on the: 2023-12-22
//

#if _WIN32
#include <C:\dev\vcpkg\packages\boost-asio_x86-windows\include\boost\asio.hpp>
#endif

#if __linux__
#include <boost/asio.hpp>
#endif

// STL: General
#include <cstdio>
#include <cstdlib>
#include <iostream>

// STL: Memory Management
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <typeinfo>
#include <vector>

class TCPConnector : public boost::enable_shared_from_this<TCPConnector>
{
  boost::asio::ip::tcp::socket _socket;
  std::string _message;

  TCPConnector(boost::asio::io_context& io_context) : _socket(io_context)
  {
  }

  void handle()
  {
  }

public:
  typedef boost::shared_ptr<TCPConnector> pointer;

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

    boost::asio::async_write
        (
            this->_socket,
            boost::asio::buffer(this->_message),
            boost::bind
                (
                    &this->handle,
                    shared_from_this()
                )
        );
  }
};

class TCPServer
{
  boost::asio::io_context& _io_context;
  boost::asio::ip::tcp::acceptor _acceptor;

  void handle
      (
          TCPConnector::pointer connection,
          const boost::system::error_code& error
      )
  {
  }

  void start()
  {
    this->_acceptor.async_accept(connection->socket(),
        boost::bind(&this->handle, this, connection
    boost::asio::placeholders::error));
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
