#if !defined(__apdu_cpp_h_data_chunk__)
#define __apdu_cpp_h_data_chunk__

#include <memory>
#include <vector>
#include <string>
#include <sstream>

namespace apdu_cpp {

class data_chunk {
public:
  using value_type = uint8_t;
  using iterator = value_type*;
  using const_iterator = const value_type*;
  using this_type = data_chunk;

private:
  using data_type = std::vector<const value_type>;
  using data_sp = std::shared_ptr<const data_type>;

  const data_sp     m_data;
  const std::size_t m_offset;
  const std::size_t m_length;

  data_chunk(data_sp src, std::size_t offset, std::size_t length) :
    m_data(src),
    m_offset(offset),
    m_length(length)
  {}

  data_chunk() : m_offset(0), m_length(0), m_data() {}

protected:

public:
  static data_chunk invalid() {
    return data_chunk();
  }

  static data_chunk create(const void* src, std::size_t size) {
    auto _src = reinterpret_cast<const value_type*>(src); 
    data_sp data = std::make_shared<data_type>(_src, _src + size);
    return data_chunk(data, 0, size);
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
    return data_chunk::create(data.data(), data.size());
  }

  ~data_chunk() {}

  bool is_valid() const { return m_data.get() != nullptr; }
  operator bool() const { return is_valid(); }

  std::size_t size() const { return m_length; }
  const value_type* raw_data_ptr() const { return m_data->data() + m_offset; }

  const_iterator cbegin() const { return m_data->data() + m_offset; }
  const_iterator cend() const { return m_data->data() + m_offset + m_length; }
  const_iterator begin() const { return cbegin(); }
  const_iterator end() const { return cend(); }

  data_chunk get_range(std::size_t offset, std::size_t length) const {
    if(offset + length > m_length) return data_chunk::invalid();
    return data_chunk(m_data, m_offset + offset, length);
  }

  data_chunk get_range(std::size_t offset) const {
    if(offset > m_length) return data_chunk::invalid();
    return data_chunk(m_data, m_offset + offset, m_length - offset);
  }

  std::string to_hex_string() const {
    if(!is_valid()) return std::string();
    constexpr const char tbl[] = "0123456789ABCDEF";
    std::stringstream ss;
    for (auto it = cbegin(); it != cend(); ++it) {
      ss << tbl[(*it >> 4) & 0x0f] << tbl[*it & 0x0f];
    }
    return ss.str();
  }

  std::string to_string() const {
    if(!is_valid()) return std::string();
    std::string str;
    for (auto it = cbegin(); it != cend(); ++it) {
      str.push_back(*it);
    }
    return str;
  }
};

} /** namespace apdu_cpp */
#endif /** !defined(__apdu_cpp_h_data_chunk__) */