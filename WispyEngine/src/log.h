#pragma once

#include <fstream>
#include <string>

namespace wispy {

class Log {
public:
  Log();
  ~Log();

  void Append(const std::string &msg);

private:
  static time_t now_;
  static tm local_time_;

  std::string output_filename_;
  std::ofstream output_stream_;
};

}
