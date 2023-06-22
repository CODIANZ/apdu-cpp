#if !defined(__apdu_cpp_h_rapdu__)
#define __apdu_cpp_h_rapdu__

#include "data_chunk.h"
#include "data_chunk_holder.h"
#include "tlv.h"
#include <unordered_map>

namespace apdu_cpp {

class rapdu {
public:
  class status_word : public data_chunk_holder {
  friend class rapdu;
  private:
    status_word() {}
    status_word(data_chunk data) : data_chunk_holder(data) {}
    static status_word create(const data_chunk& src) {
      do{
        if(!src) break;
        if(src.size() < 2) break;
        return status_word(src);
      } while(false);
      return status_word();
    }
  protected:
  public:
    uint16_t sw1sw2() const throw(std::runtime_error) {
      if(!is_valid()) throw std::runtime_error("invalid value");
      return (data().raw_data_ptr()[0] << 8) | data().raw_data_ptr()[1];
    }

    uint8_t sw1() const throw(std::runtime_error) {
      if(!is_valid()) throw std::runtime_error("invalid value");
      return data().raw_data_ptr()[0];
    }

    uint8_t sw2() const throw(std::runtime_error) {
      if(!is_valid()) throw std::runtime_error("invalid value");
      return data().raw_data_ptr()[1];
    }

    std::string to_hex_string() const {
      if(!is_valid()) return std::string();
      return data().to_hex_string();
    }

    bool is_valid() const {
      return data().size() == 2;
    }

    operator bool() const {
      return is_valid();
    }

    bool is_ok() const {
      return sw1() == 0x90 && sw2() == 0x00;
    }
  };

private:
  const data_chunk  m_data;
  const status_word m_status_word;

  rapdu() : m_data(data_chunk::invalid()) {}
  template<typename DATA, typename STATUS_WORD> rapdu(DATA&& data, STATUS_WORD&& sw) :
    m_data(std::forward<DATA>(data)),
    m_status_word(std::forward<STATUS_WORD>(sw))
  {}

protected:

public:
  static rapdu create(const data_chunk& src) {
    if(!src) return rapdu();
    if(src.size() < 2) return rapdu();

    const auto data = src.get_range(0, src.size() - 2);
    const auto sw = src.get_range(src.size() - 2, 2);
    return rapdu(data, sw);
  }

  bool is_valid() const {
    return m_status_word;
  }

  operator bool() const {
    return is_valid();
  }

  const status_word& status_word() const {
    return m_status_word;
  }

  const data_chunk& data() const {
    return m_data;
  }

  tlv get_tlv() const {
    return tlv::create(m_data);
  }
};

} /** namespace apdu_cpp */
#endif /** !defined(__apdu_cpp_h_rapdu__) */