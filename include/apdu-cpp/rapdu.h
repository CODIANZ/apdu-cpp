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
    static status_word create(const tlv& tlv, const data_chunk& src) {
      do{
        if(!tlv) break;
        if(!src) break;
        if(src.size() < (tlv.size() + 2)) break;
        return status_word(src.get_range(tlv.size(), 2));
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
  };

private:
  const tlv         m_tlv;
  const status_word m_status_word;

  rapdu() {}
  template<typename TLV, typename STATUS_WORD> rapdu(TLV&& tlv, STATUS_WORD&& sw) :
    m_tlv(std::forward<TLV>(tlv)),
    m_status_word(std::forward<STATUS_WORD>(sw))
  {}

protected:

public:
  static rapdu create(const data_chunk& src) {
    if(!src) return rapdu();
    const auto tlv = tlv::create(src);
    const auto sw = status_word::create(tlv, src);
    return rapdu(tlv, sw);
  }
  
  bool is_valid() const {
    return m_tlv && m_status_word;
  }

  operator bool() const {
    return is_valid();
  }

  std::size_t size() const {
    return m_tlv.size() + m_status_word.size();
  }

  const tlv& get_tlv() const {
    return m_tlv;
  }

  const status_word& get_status_word() const {
    return m_status_word;
  }
};

} /** namespace apdu_cpp */
#endif /** !defined(__apdu_cpp_h_rapdu__) */