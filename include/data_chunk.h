#if !defined(__apdu_cpp_h_data_chunk__)
#define __apdu_cpp_h_data_chunk__

#include <memory>
#include <vector>

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

protected:

public:
  static data_chunk from(value_type* src, std::size_t size) {
    data_sp data = std::make_shared<data_type>(src, src + size);
    return data_chunk(data, 0, size);
  }

  static data_chunk from(data_sp src, std::size_t offset, std::size_t length) {
    return data_chunk(src, offset, length);
  }

  static data_chunk from(const data_chunk& chunk, std::size_t offset, std::size_t length) {
    return data_chunk(chunk.m_data, offset, length);
  }

  ~data_chunk() {}

  std::size_t size() const { return m_length; }
  const value_type* data() const { return m_data->data() + m_offset; }

  const_iterator cbegin() const { return m_data->data() + m_offset; }
  const_iterator cend() const { return m_data->data() + m_offset + m_length; }
  const_iterator begin() const { return cbegin(); }
  const_iterator end() const { return cend(); }
};

} /** namespace apdu_cpp */
#endif /** !defined(__apdu_cpp_h_data_chunk__) */