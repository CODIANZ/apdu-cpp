#if !defined(__apdu_cpp_h_data_chunk_holder__)
#define __apdu_cpp_h_data_chunk_holder__

#include "data_chunk.h"

namespace apdu_cpp {

class data_chunk_holder {
private:
  const data_chunk  m_data;

protected:
  data_chunk_holder() : m_data(data_chunk::invalid()) {}
  data_chunk_holder(data_chunk data) : m_data(data) {}

public:
  virtual ~data_chunk_holder() {}

  bool is_valid() const { return m_data.is_valid(); }
  bool is_empty() const { return m_data.is_empty(); }

  std::size_t size() const { return m_data.size(); }
  const data_chunk& data() const { return m_data; }

  const data_chunk::value_type& operator [] (std::size_t index) const throw(std::runtime_error) {
    return m_data[index];
  }

  bool operator == (const data_chunk& rhs) const {
    return m_data == rhs;
  }

  bool operator != (const data_chunk& rhs) const {
    return !(*this == rhs);
  }

  bool operator == (const data_chunk_holder& rhs) const {
    return m_data == rhs.m_data;
  }

  bool operator != (const data_chunk_holder& rhs) const {
    return !(*this == rhs);
  }

  std::string to_hex_string() const {
    return m_data.to_hex_string();
  }

  std::string to_string() const {
    return m_data.to_string();
  }

  uint8_t to_uint8() const throw(std::runtime_error) {
    return m_data.to_uint8();
  }

  uint16_t to_uint16() const throw(std::runtime_error) {
    return m_data.to_uint16();
  }

  uint32_t to_uint32() const throw(std::runtime_error) {
    return m_data.to_uint32();
  }

  uint64_t to_uint64() const throw(std::runtime_error) {
    return m_data.to_uint64();
  }

  uint64_t to_uint() const throw(std::runtime_error) {
    return m_data.to_uint();
  }
};

} /** namespace apdu_cpp */
#endif /** !defined(__apdu_cpp_h_data_chunk_holder__) */