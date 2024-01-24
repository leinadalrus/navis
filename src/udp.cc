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
#include <deque>
#include <memory>
#include <mutex>
#include <typeinfo>
#include <vector>

#include "../include/pale_noel.h"

int main()
{
  using boost::asio::ip::udp;

  try
  {
    boost::asio::io_context io_context;
    udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));

    std::array<char, 1> ch_buf;
    udp::endpoint remote_e2e;

    for (auto &ch : ch_buf)
    {
      socket.receive_from(boost::asio::buffer(ch_buf), remote_e2e);
      std::printf("Channel Buffer Message - Character:\t%c", ch);
      std::string buffer = &ch;

      boost::system::error_code ignored_error;
      socket.send_to(boost::asio::buffer(buffer), remote_e2e, 0, ignored_error);
    }
  }
  catch (std::exception &exception)
  {
    std::cerr << exception.what() << std::endl;
  }
}
