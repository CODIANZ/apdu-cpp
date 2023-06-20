#if !defined(__apdu_cpp_h_hex_converter__)
#define __apdu_cpp_h_hex_converter__

#include <memory>
#include <vector>
#include <sstream>

#include "data_chunk.h"

namespace apdu_cpp {

class data_chunk_converter {
public:
  static std::string to_hex_string(const data_chunk& data) {
    constexpr const char tbl[] = "0123456789abcdef";
    std::stringstream ss;
    for (auto it = data.cbegin(); it != data.cend(); ++it) {
      ss << tbl[(*it >> 4) & 0x0f] << tbl[*it & 0x0f];
    }
    return ss.str();
  }

  static std::string to_string(const data_chunk& data) {
    std::string str;
    for (auto it = data.cbegin(); it != data.cend(); ++it) {
      str.push_back(*it);
    }
    return str;
  }

  static data_chunk from_hex_string(const std::string& hex) {
    std::vector<uint8_t> data;
    for (auto it = hex.cbegin(); it != hex.cend(); ++it) {
      uint8_t v = 0;
      if ('0' <= *it && *it <= '9') {
        v = *it - '0';
      } else if ('a' <= *it && *it <= 'f') {
        v = *it - 'a' + 10;
      } else if ('A' <= *it && *it <= 'F') {
        v = *it - 'A' + 10;
      } else {
        throw std::runtime_error("invalid hex string");
      }
      if (it + 1 == hex.cend()) {
        throw std::runtime_error("invalid hex string");
      }
      ++it;
      uint8_t v2 = 0;
      if ('0' <= *it && *it <= '9') {
        v2 = *it - '0';
      } else if ('a' <= *it && *it <= 'f') {
        v2 = *it - 'a' + 10;
      } else if ('A' <= *it && *it <= 'F') {
        v2 = *it - 'A' + 10;
      } else {
        throw std::runtime_error("invalid hex string");
      }
      data.push_back((v << 4) | v2);
    }
    return data_chunk::from(data.data(), data.size());
  }
};

} /** namespace apdu_cpp */
#endif /** !defined(__apdu_cpp_h_hex_converter__) */