#ifndef SRC_BASE_NET_TCP_TCP_SERVER_H_
#define SRC_BASE_NET_TCP_TCP_SERVER_H_

#include <functional>
#include <memory>

#include "base/base_export.h"
#include "base/callback.h"
#include "base/macros.h"
#include "base/net/tcp/tcp_common.h"
#include "base/third_party/asio/asio/include/asio.hpp"
#include "base/thread/thread.h"

namespace base {

namespace net {

class TCPServer {
 public:
  TCPServer(const int port, const bool is_ipv6 = false);
  ~TCPServer() = default;

  void StartAccept(const TCPSocketAcceptHandler& accept_handler);

  void ReadAsync(std::shared_ptr<asio::ip::tcp::socket> socket,
                 const TCPSocketReadHandler& read_handler);   

  void WriteAsync(std::shared_ptr<asio::ip::tcp::socket> socket,
                  const std::string& message,
                  const TCPSocketWriteHandler& write_handler);

 private:
  asio::io_service io_service_;
  asio::io_service::work io_work_;

  asio::ip::tcp::acceptor acceptor_;
  std::shared_ptr<asio::ip::tcp::socket> socket_;

  int port_;
  Thread thread_;
};

}  // namespace net

}  // namespace base

#endif  // SRC_BASE_NET_TCP_TCP_SERVER_H_