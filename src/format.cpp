#include <string>

#include "format.h"

using std::string;

string Format::Format(int time) {
  string timeAsString = std::to_string(time);
  return string(2 - timeAsString.length(), '0') + timeAsString;
}

string Format::ElapsedTime(long seconds) {
  int hour = 0;
  int min = 0;
  int sec = 0;

  hour = seconds / 3600;
  seconds = seconds % 3600;
  min = seconds / 60;
  seconds = seconds % 60;
  sec = seconds;

  return Format(hour) + ':' + Format(min) + ':' + Format(sec);
}