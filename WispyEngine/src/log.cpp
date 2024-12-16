#include "log.h"

#include <Windows.h>

namespace wispy {

time_t Log::now_ = time(NULL);
tm Log::local_time_ = { 0 };

Log::Log() : output_filename_(), output_stream_() {
  now_ = time(NULL);
  localtime_s(&local_time_, &now_);

  CreateDirectory(L"logs", NULL);

  output_filename_ = "logs/wispy_log_";
  output_filename_ += std::to_string(((local_time_.tm_year + 1900) * 100 + local_time_.tm_mon + 1) * 100 + local_time_.tm_mday);
  output_filename_ += ".log";

  output_stream_.open(output_filename_);
  output_stream_ << "WispyEngine : 1.0.0.0\n";
  output_stream_.close();
}

Log::~Log() {
  std::remove(output_filename_.c_str());
}

void Log::Append(const std::string &msg) {
  now_ = time(NULL);
  localtime_s(&local_time_, &now_);

  output_stream_.open(output_filename_, std::ios_base::app);

  output_stream_ << '(';
  if (local_time_.tm_hour < 10) output_stream_ << '0';
  output_stream_ << (local_time_.tm_hour) << ':';
  if (local_time_.tm_min < 10) output_stream_ << '0';
  output_stream_ << (local_time_.tm_min) << ':';
  if (local_time_.tm_sec < 10) output_stream_ << '0';
  output_stream_ << (local_time_.tm_sec) << ')';

  output_stream_ << ' ' << msg << '\n';

  output_stream_.close();
}

}
