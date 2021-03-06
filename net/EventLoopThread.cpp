//
// Created by Ye-zixiao on 2021/4/9.
//

#include "libfm/fmnet/EventLoopThread.h"
#include <cassert>
#include "libfm/fmutil/Log.h"
#include "libfm/fmnet/EventLoop.h"

using namespace fm;
using namespace fm::net;

EventLoopThread::EventLoopThread()
    : loop_(nullptr),
      exiting_(false),
      thread_(),
      mutex_(),
      cond_() {}

EventLoopThread::~EventLoopThread() {
  exiting_ = true;
  if (loop_) {
    loop_->quit();
    thread_->join();
  }
}

EventLoop *EventLoopThread::startLoop() {
  assert(!thread_);
  thread_ = std::make_unique<std::thread>([this] { threadFunc(); });

  EventLoop *loop = nullptr;
  {
    std::unique_lock<std::mutex> lock(mutex_);
    while (!loop_)
      cond_.wait(lock);
    loop = loop_;
  }
  return loop;
}

void EventLoopThread::threadFunc() {
  EventLoop loop;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    loop_ = &loop;
    cond_.notify_all();
  }

  loop.loop();
  std::lock_guard<std::mutex> lock(mutex_);
  loop_ = nullptr;
}