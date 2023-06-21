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
  operator bool() const { return is_valid(); }

  std::size_t size() const { return m_data.size(); }
  data_chunk data() const { return m_data; }
};

} /** namespace apdu_cpp */
#endif /** !defined(__apdu_cpp_h_data_chunk_holder__) */