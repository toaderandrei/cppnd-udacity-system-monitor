//
// Created by toaderst on 08-03-20.
//

#ifndef MONITOR_PARSER_UTILS_H
#define MONITOR_PARSER_UTILS_H

#include <fstream>
#include <iostream>
#include <sstream>

namespace ParserUtils {

inline void CloseStream(std::ifstream *stream) {
  if (stream != NULL && stream->is_open()) {
    stream->close();
  }
}

template <typename T>
T GetValueByKey(const std::string &directory, const std::string &filename,
                const std::string &filter) {
  std::string key, line;
  T value;
  std::ifstream stream(directory + filename);
  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == filter) {
          return value;
        }
      }
    }
  }
  CloseStream(&stream);
  return value;
}

template <typename T>
T GetFirstValueByKey(const std::string &directory, const std::string &filename,
                     const size_t &total_characters = 40) {
  std::string key, line;
  T value;
  std::ifstream stream(directory + filename);
  if (stream.is_open()) {
    if (getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key.size() > total_characters) {
        key.resize(total_characters);
        key = key + "...";
      }
      return key;
    }
  }
  CloseStream(&stream);
  return value;
}

}  // namespace ParserUtils

#endif  // MONITOR_PARSER_UTILS_H
