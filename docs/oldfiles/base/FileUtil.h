//
// Created by Ye-zixiao on 2021/4/7.
//

#ifndef LIBFM_BASE_FILEUTIL_H_
#define LIBFM_BASE_FILEUTIL_H_

#include <cstdio>
#include <string>
#include "NonCopyable.h"
#include "Timestamp.h"
#include "SystemClock.h"

namespace fm {

class AppendFile : private NonCopyable {
 public:
  explicit AppendFile(const std::string &filename);
  ~AppendFile();

  void append(const char *logline, size_t len);

  void flush();

  off_t writenBytes() const { return writen_bytes_; }

 private:
  size_t write(const char *logline, size_t len);

  FILE *fp_;
  off_t writen_bytes_;
  char buffer_[64 * 1024];
};

// 只推荐读取较小的文件！
class ReadSmallFile : private NonCopyable {
 public:
  explicit ReadSmallFile(const std::string &filename);
  ~ReadSmallFile();

  size_t fileSize() const { return file_size_; }

  const time::Timestamp &mtime() const { return mtime_; }
  const time::Timestamp &ctime() const { return ctime_; }

  int readToString(std::string *content, size_t maxContentSize);

 public:
  static constexpr size_t k128KB = 1024 * 128;
  static constexpr size_t k512KB = 1024 * 512;
  static constexpr size_t k1MB = 1024 * 1024;
  static constexpr size_t k64MB = 1024 * 1024 * 64;

  static int readFile(const std::string &filename,
                      std::string *content,
                      size_t maxContentSize,
                      size_t *fileSize = nullptr,
                      time::Timestamp *mtime = nullptr,
                      time::Timestamp *ctime = nullptr);

 private:
  int fd_;
  int err_;
  size_t file_size_;
  time::Timestamp mtime_;
  time::Timestamp ctime_;
};

} // namespace fm

#endif //LIBFM_BASE_FILEUTIL_H_
