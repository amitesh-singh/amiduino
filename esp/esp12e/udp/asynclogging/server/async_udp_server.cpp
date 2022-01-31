#include <cstdlib>
#include <iostream>
//#include <bind.hpp>
#include <asio.hpp>

using namespace std;

using asio::ip::udp;

class server
{
public:
  server(asio::io_context& io_context, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port))
  {
    socket_.async_receive_from(
        asio::buffer(data_, max_length), sender_endpoint_,
        std::bind(&server::handle_receive_from, this,
          std::placeholders::_1,
          std::placeholders::_2));
  }

  void handle_receive_from(const std::error_code& error,
      size_t bytes_recvd)
  {
    if (!error && bytes_recvd > 0)
    {
       std::cerr << data_;
/*      socket_.async_send_to(
          asio::buffer(data_, bytes_recvd), sender_endpoint_,
          std::bind(&server::handle_send_to, this,
            std::placeholders::_1,
            std::placeholders::_2));
            */
    }
    socket_.async_receive_from(
        asio::buffer(data_, max_length), sender_endpoint_,
        std::bind(&server::handle_receive_from, this,
                  std::placeholders::_1,
                  std::placeholders::_2));
  }

  void handle_send_to(const std::error_code& /*error*/,
                      size_t /*bytes_sent*/)
    {
       socket_.async_receive_from(
           asio::buffer(data_, max_length), sender_endpoint_,
           std::bind(&server::handle_receive_from, this,
                     std::placeholders::_1,
                     std::placeholders::_2));
    }

private:
  udp::socket socket_;
  udp::endpoint sender_endpoint_;
  enum { max_length = 1024 };
  char data_[max_length];
};

int main(int argc, char* argv[])
{
   try
     {
        if (argc != 2)
          {
             std::cerr << "Usage: async_udp_echo_server <port>\n";
             return 1;
          }

        asio::io_context io_context;

        std::cout << "Started logging\n";
        using namespace std; // For atoi.
        server s(io_context, atoi(argv[1]));

        io_context.run();
     }
   catch (std::exception& e)
     {
        std::cerr << "Exception: " << e.what() << "\n";
     }

   return 0;
}
