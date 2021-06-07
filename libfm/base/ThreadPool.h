//
// Created by Ye-zixiao on 2021/4/7.
//

#ifndef LIBFM_BASE_THREADPOOL_H_
#define LIBFM_BASE_THREADPOOL_H_

#include <condition_variable>
#include <functional>
#include <string>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>
#include <queue>

#include "libfm/base/noncoapyable.h"

namespace fm {

class ThreadPool : private noncopyable {
 public:
  using Task = std::function<void()>;
  using ThreadPtr = std::unique_ptr<std::thread>;

  ThreadPool(std::string str, size_t maxSize);
  ~ThreadPool();

  const std::string &name() const { return name_; }

  void setThreadNum(int numThreads);

  void start();
  void stop();
  void run(Task task);

 private:
  void runInThread(); // 工作线程所执行的例程
  Task take();
  bool isEmptyQueue();

 private:
  std::string name_;
  std::mutex mutex_;
  std::condition_variable notEmpty_;
  std::condition_variable notFull_;
  std::queue<Task> queue_;
  std::vector<ThreadPtr> threads_;
  bool running_; // 是否能够用原子操作替代？或者说有这个必要吗？
  size_t maxSize_;
};

} // namespace fm

#endif //LIBFM_BASE_THREADPOOL_H_