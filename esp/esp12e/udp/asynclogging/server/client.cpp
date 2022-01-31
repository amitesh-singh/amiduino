#include <asio.hpp>
#include <iostream>

class udpclient
{   
    asio::io_context &io_;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint endpoint;
    public:
    udpclient(asio::io_context &io, std::uint16_t port): io_(io), endpoint(asio::ip::udp::v4(), port),
        socket(io, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0))
    {

    }

    ~udpclient()
    {
        socket.close();
    }

    void send(const std::string &msg)
    {
        socket.send_to(asio::buffer(msg, msg.size()), endpoint);
    }
};

int main(int argc, char **argv)
{
    asio::io_context io;
    if (argc != 2)
      {
         std::cerr << argv[0] << " <msg>\n";
         return -1;
      }

    udpclient client(io, 15001);

    client.send(argv[1]);
    io.run();
    return 0;
}
