//
// Created by Ye-zixiao on 2021/4/7.
//

#ifndef LIBFM_NET_CALLBACK_H_
#define LIBFM_NET_CALLBACK_H_

#include <functional>
#include <memory>

#include "libfm/fmutil/TimeStamp.h"

namespace fm {

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

namespace net {

class Buffer;
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

using TimerCallback = std::function<void()>;

// 下面几个都属于普通的EventCallback范畴
using ConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
using CloseCallback = std::function<void(const TcpConnectionPtr &)>;
using WriteCompleteCallback = std::function<void(const TcpConnectionPtr &)>;
using HighWaterMarkCallback = std::function<void(const TcpConnectionPtr &, size_t)>;

// MessageCallback比较特殊，是用户注册最常用的回调函数
using MessageCallback = std::function<void(const TcpConnectionPtr &,
                                           Buffer *, time::TimeStamp)>;

void defaultConnectionCallback(const TcpConnectionPtr &conn);
void defaultMessageCallback(const TcpConnectionPtr &conn,
                            Buffer *buffer, time::TimeStamp receivedTime);

} // namespace net
} // namespace fm

#endif //LIBFM_NET_CALLBACK_H_
