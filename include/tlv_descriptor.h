#if !defined(__apdu_cpp_h_tlv_descriptor__)
#define __apdu_cpp_h_tlv_descriptor__

#include "tlv.h"

namespace apdu_cpp {

class tlv_descriptor {
public:
  static std::string to_string(const tlv& tlv) {
    std::stringstream ss;
    to_stream(tlv, ss);
    return ss.str();
  }

  static void to_stream(const tlv& tlv, std::ostream& os, std::size_t indent = 0) {
    if(!tlv.is_valid()) {
      os << "invalid tlv" << std::endl;
      return;
    }

    auto class_type_to_string = [](tlv::tag::class_type class_type) -> std::string {
      switch(class_type) {
      case tlv::tag::class_type::invalid:
        return "invalid";
      case tlv::tag::class_type::universal:
        return "universal";
      case tlv::tag::class_type::application:
        return "application";
      case tlv::tag::class_type::context_specific:
        return "context_specific";
      case tlv::tag::class_type::private_use:
        return "private_use";
      }
    };

    auto _i = [indent]() -> std::string {
      std::string ret;
      for(std::size_t i = 0; i < indent; i++) {
        ret += " ";
      }
      return ret;
    }();

    os
      << _i << "tag: " << tlv.get_tag().data().to_hex_string() << std::endl
      << _i << " - class_type: " << class_type_to_string(tlv.get_tag().get_class()) << std::endl
      << _i << " - is_constructed: " << (tlv.get_tag().is_constructed() ? "true" : "false") << std::endl
      << _i << " - is_primitive: " << (tlv.get_tag().is_primitive() ? "true" : "false") << std::endl
      << _i << "length:" << tlv.get_length().data().to_hex_string()
            << "(" << tlv.get_length().get_length() << ")" << std::endl
      << _i << "value: " << tlv.get_value().data().to_hex_string() << std::endl;

    if(tlv.get_children().empty()) return;

     os << _i << "children: " << std::endl;    
    for(auto i = 0; i < tlv.get_children().size(); i++) {
      os << _i << "  [" << i << "]" << std::endl;
      to_stream(tlv.get_children()[i], os, indent + 4);
    }
  }
};

} /** namespace apdu_cpp */
#endif /** !defined(__apdu_cpp_h_tlv_descriptor__) */