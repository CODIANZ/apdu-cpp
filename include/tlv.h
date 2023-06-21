#if !defined(__apdu_cpp_h_tlv__)
#define __apdu_cpp_h_tlv__

#include "data_chunk.h"
#include "data_chunk_holder.h"
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

namespace apdu_cpp {

class tlv {
friend class rapdu;
public:
  class tag : public data_chunk_holder {
  friend class tlv;
  private:
    tag() {}
    tag(data_chunk data) : data_chunk_holder(data) {}

    static tag create(const data_chunk& src) {
      do{
        if(!src) break;
        auto it = src.cbegin();
        if(it == src.cend()) break;
        if((*it & 0x1F) != 0x1F) {
          return tag(src.get_range(0, 1));
        }
        while(true){
          it++;
          if(it == src.cend()) break;
          if((*it & 0x80) == 0) {
            return tag(src.get_range(0, it - src.cbegin() + 1));
          }
        }
      } while(false);
      return tag();
    }

  public:
    enum class class_type {
      invalid = 0xFF,
      universal = 0x00,
      application = 0x40,
      context_specific = 0x80,
      private_use = 0xC0
    };

    static tag create(const std::vector<data_chunk::value_type>& src){
      return create(data_chunk::create(src.data(), src.size()));
    }

    static tag from_hex_string(const std::string& src){
      return create(data_chunk::from_hex_string(src));
    }

    class_type get_class() const {
      if(!is_valid()) return class_type::invalid;
      return static_cast<class_type>(data().raw_data_ptr()[0] & 0xC0);
    }
    
    bool is_constructed() const {
      if(!is_valid()) return false;
      return (data().raw_data_ptr()[0] & 0x20) != 0;
    }

    bool is_primitive() const {
      if(!is_valid()) return false;
      return !is_constructed();
    }

    bool operator == (const data_chunk& rhs) const {
      if(!is_valid()) return false;
      if(!rhs) return false;
      return std::equal(data().cbegin(), data().cend(), rhs.cbegin(), rhs.cend());
    }
  };

  struct tag_hash {
    std::size_t operator()(const tag& tag) const {
      return std::hash<std::string>()(tag.data().to_hex_string());
    }
  };

  class length : public data_chunk_holder {
  friend class tlv;
  private:
    uint64_t m_value_length;
    length() : m_value_length(0) {}
    length(data_chunk data, uint64_t value_length) :
      data_chunk_holder(data),
      m_value_length(value_length) {}

    static length create(const tag& tag, const data_chunk& src){
      do{
        if(!tag) break;
        if(!src) break;
        return create(src.get_range(tag.size(), src.size() - tag.size()));
      } while(false);
      return length();
    }

    static length create(const data_chunk& src) {
      do{
        if(!src) break;
        auto it = src.cbegin();
        if(it == src.cend()) break;
        if(*it & 0x80) {
          const auto _length = *it & 0x7F;
          if(_length == 0) break;
          if(_length > 8) break;
          it++;
          if(it == src.cend()) break;
          uint64_t value = 0;
          for(int i = 0; i < _length; i++) {
            value <<= 8;
            value |= *it;
            it++;
            if(it == src.cend()) break;
          }
          return length(src.get_range(0, it - src.cbegin()), value);
        }
        else {
          return length(src.get_range(0, 1), *it);
        }
      } while(false);
      return length();
    }
  protected:
  public:
    uint64_t get_length() const { return m_value_length; }
  };

  class value : public data_chunk_holder {
  friend class tlv;
  private:
    value() {}
    value(data_chunk data) : data_chunk_holder(data) {}
    static value create(const tag& tag, const length& length, const data_chunk& src) {
      do{
        if(!tag) break;
        if(!length) break;
        if(!src) break;
        if(src.size() < (tag.size() + length.size() + length.get_length())) break;
        return value(src.get_range(tag.size() + length.size(), length.get_length()));
      } while(false);
      return value();
    }
  protected:
  public:
    uint8_t as_uint8() const throw(std::runtime_error) {
      if(!is_valid()) throw std::runtime_error("invalid value");
      if(size() != 1) throw std::runtime_error("invalid length");
      return data().raw_data_ptr()[0];
    }

    uint16_t as_uint16() const throw(std::runtime_error) {
      if(!is_valid()) throw std::runtime_error("invalid value");
      if(size() != 2) throw std::runtime_error("invalid length");
      return (data().raw_data_ptr()[0] << 8) | data().raw_data_ptr()[1];
    }

    uint32_t as_uint32() const throw(std::runtime_error) {
      if(!is_valid()) throw std::runtime_error("invalid value");
      if(size() != 4) throw std::runtime_error("invalid length");
      return (data().raw_data_ptr()[0] << 24) | (data().raw_data_ptr()[1] << 16) | (data().raw_data_ptr()[2] << 8) | data().raw_data_ptr()[3];
    }

    uint64_t as_uint64() const throw(std::runtime_error) {
      if(!is_valid()) throw std::runtime_error("invalid value");
      if(size() != 8) throw std::runtime_error("invalid length");
      return (static_cast<uint64_t>(data().raw_data_ptr()[0]) << 56) | (static_cast<uint64_t>(data().raw_data_ptr()[1]) << 48) | (static_cast<uint64_t>(data().raw_data_ptr()[2]) << 40) | (static_cast<uint64_t>(data().raw_data_ptr()[3]) << 32) | (static_cast<uint64_t>(data().raw_data_ptr()[4]) << 24) | (static_cast<uint64_t>(data().raw_data_ptr()[5]) << 16) | (static_cast<uint64_t>(data().raw_data_ptr()[6]) << 8) | static_cast<uint64_t>(data().raw_data_ptr()[7]);
    }
  };

private:
  const tag         m_tag;
  const length      m_length;
  const value       m_value;

  std::vector<tlv>  m_children;

  tlv() {}
  template<typename TAG, typename LENGTH, typename VALUE> tlv(TAG&& t, LENGTH&& l, VALUE&& v) :
    m_tag(std::forward<TAG>(t)),
    m_length(std::forward<LENGTH>(l)),
    m_value(std::forward<VALUE>(v))
  {
    if(m_tag.is_constructed()){
      auto it = m_value.data().cbegin();
      while(it != m_value.data().cend()){
        const auto child = tlv::create(m_value.data().get_range(it - m_value.data().cbegin(), m_value.data().cend() - it));
        if(!child) break;
        m_children.push_back(child);
        it += child.size();
      }
    }
  }

protected:

public:
  static tlv create(const data_chunk& src) {
    if(!src) return tlv();
    const auto tag = tag::create(src);
    const auto length = length::create(tag, src);
    const auto  value = value::create(tag, length, src);
    return tlv(tag, length, value);
  }
  
  bool is_valid() const {
    return m_tag && m_length && m_value;
  }

  operator bool() const {
    return is_valid();
  }

  std::size_t size() const {
    return m_tag.size() + m_length.size() + m_value.size();
  }

  const tag& get_tag() const {
    return m_tag;
  }

  const length& get_length() const {
    return m_length;
  }

  const value& get_value() const {
    return m_value;
  }

  const std::vector<tlv>& get_children() const {
    return m_children;
  }

  std::vector<tlv> find_children(const tag& tag) const {
    std::vector<tlv> result;
    for(const auto& child : m_children){
      if(child.get_tag() == tag){
        result.push_back(child);
      }
    }
    return result;
  }
};

} /** namespace apdu_cpp */
#endif /** !defined(__apdu_cpp_h_tlv__) */