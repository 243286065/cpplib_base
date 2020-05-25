#ifndef SRC_BASE_NET_TCP_TCP_COMMON_H_
#define SRC_BASE_NET_TCP_TCP_COMMON_H_

#include <functional>

#include "base/third_party/asio/asio/include/asio.hpp"

namespace base {

namespace net {

struct TCPSocketPair {
  std::shared_ptr<asio::ip::tcp::socket> socket;
  std::shared_ptr<char> buff = nullptr;
};

typedef std::function<void(const asio::error_code& error_code,
                           std::shared_ptr<asio::ip::tcp::socket> socket)>
    TCPSocketAcceptHandler;

typedef std::function<void(const asio::error_code& error_code,
                           std::size_t bytes_transferred,
                           std::shared_ptr<TCPSocketPair> socket)>
    TCPSocketReadHandler;

typedef std::function<void(const asio::error_code& error_code,
                           std::size_t bytes_transferred,
                           std::shared_ptr<TCPSocketPair> socket)>
    TCPSocketWriteHandler;

}  // namespace net

}  // namespace base

#endif  // SRC_BASE_NET_TCP_TCP_COMMON_H_