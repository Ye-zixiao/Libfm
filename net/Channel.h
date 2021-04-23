//
// Created by Ye-zixiao on 2021/4/7.
//

#ifndef FAKEMUDUO_NET_CHANNEL_H_
#define FAKEMUDUO_NET_CHANNEL_H_

#include "../base/noncoapyable.h"
#include "../base/TimeStamp.h"

#include <functional>
#include <memory>

namespace fm {

namespace net {

class EventLoop;

class Channel : private noncopyable {
 public:
  using EventCallback = std::function<void()>;
  // 可读事件的回调函数比较特殊，需要向用户的回调函数传递一个接收时间信息
  using ReadEventCallback = std::function<void(TimeStamp)>;

  Channel(EventLoop *loop, int fd);

  ~Channel();

  void handleEvent(TimeStamp receiveTime);

  void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
  void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
  void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
  void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

  void enableReading() { events_ |= kReadEvent, update(); }
  void enableWriting() { events_ |= kWriteEvent, update(); }
  void disableReading() { events_ &= ~kReadEvent, update(); }
  void disableWriting() { events_ &= ~kWriteEvent, update(); }
  void disableAll() { events_ = kNoneEvent, update(); }
  bool isReading() const { return events_ & kReadEvent; }
  bool isWriting() const { return events_ & kWriteEvent; }

  std::string eventsToString() const { return eventsToString(fd_, events_); }
  std::string reventsToString() const { return eventsToString(fd_, revents_); }

  void set_index(int idx) { index_ = idx; }
  int index() const { return index_; }

  void set_revents(int revents) { revents_ = revents; }
  int events() const { return events_; }

  int fd() const { return fd_; }

  EventLoop *ownerLoop() { return loop_; }

  void tie(const std::shared_ptr<void> &);

  void remove();

  bool isNoneEvent() const { return events_ == kNoneEvent; }

 private:
  void update();

  void handleEventWithGurad(TimeStamp receiveTime);

  static std::string eventsToString(int fd, int ev);

  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;

 private:
  EventLoop *loop_;
  const int fd_;
  int events_;
  int revents_;
  int index_;
  bool logHup_;          // 连接挂掉的时候是否需要进行日志

  std::weak_ptr<void> tie_;
  bool tied_;            // 是否与某一个连接绑定
  bool eventHandling_;   // 是否正在处理事件
  bool addedToLoop_;     // 是否添加到事件循环中
  ReadEventCallback readCallback_;
  EventCallback writeCallback_;
  EventCallback closeCallback_;
  EventCallback errorCallback_;
};

} // namespace net

} // namespace fm

#endif //FAKEMUDUO_NET_CHANNEL_H_