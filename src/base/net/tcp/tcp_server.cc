#include "base/net/tcp/tcp_server.h"

#include "base/log/logging.h"

namespace base {

namespace net {

const std::size_t kSocketBuffSize = 4 * 1024;

TCPServer::TCPServer(const int port, const bool is_ipv6)
    : io_work_(io_service_),
      acceptor_(io_service_,
                asio::ip::tcp::endpoint(
                    is_ipv6 ? asio::ip::tcp::v6() : asio::ip::tcp::v4(),
                    port)),
      socket_(new asio::ip::tcp::socket(io_service_)),
      port_(port) {}

void TCPServer::StartAccept(const TCPSocketAcceptHandler& accept_handler) {
  if (!accept_handler) {
    NOTREACHED() << "Accpet handler can not be nullptr";
    return;
  }

  if (!thread_.IsRunning()) {
    thread_.Start();

    thread_.PostTask([this]() { io_service_.run(); });

    std::shared_ptr<asio::ip::tcp::socket> socket =
        std::make_shared<asio::ip::tcp::socket>(io_service_);

    acceptor_.async_accept(
        *socket_,
        std::bind(accept_handler, std::placeholders::_1, socket));
  }
}

void TCPServer::ReadAsync(std::shared_ptr<asio::ip::tcp::socket> socket,
                          const TCPSocketReadHandler& read_handler) {
  if (!socket) {
    NOTREACHED() << "Socket is nullptr";
    return;
  }

  std::shared_ptr<TCPSocketPair> socket_pair =
      std::make_shared<TCPSocketPair>();
  socket_pair->socket = socket;
  socket_pair->buff.reset(new char[kSocketBuffSize], std::default_delete<char[]>());
  memset(socket_pair->buff.get(), )

  socket->async_read_some(
      asio::buffer(socket_pair->buff.get(), kSocketBuffSize),
      std::bind(read_handler, std::placeholders::_1, std::placeholders::_2, socket_pair));
}

void TCPServer::WriteAsync(std::shared_ptr<asio::ip::tcp::socket> socket,
                           const std::string& message,
                           const TCPSocketWriteHandler& write_handler) {
  if (!socket) {
    NOTREACHED() << "Socket is nullptr";
    return;
  }

  std::shared_ptr<TCPSocketPair> socket_pair =
      std::make_shared<TCPSocketPair>();
  socket_pair->socket = socket;
  socket_pair->buff.reset(new char[message.size()], std::default_delete<char[]>());

  socket->async_write_some(
      asio::buffer(socket_pair->buff.get(), message.size()),
      std::bind(write_handler, std::placeholders::_1, std::placeholders::_2,
                socket_pair));
}

}  // namespace net
}  // namespace base
