#include <apdu-cpp/apdu-cpp.h>
#include <iostream>

using namespace apdu_cpp;

template <typename FUNC> bool is_exception_thrown(FUNC func)
{
  try {
    func();
  } catch(...) {
    return true;
  }
  return false;
}

void test_endian()
{
  if(system_endian == endian::big){
    std::cout << "system_endian = endian::big" << std::endl;
  } else if(system_endian == endian::little) {
    std::cout << "system_endian = endian::little" << std::endl;
  } else {
    std::cout << "system_endian = endian::unknown" << std::endl;
  }

  const uint16_t d = 0x1234;
  const uint8_t* p = reinterpret_cast<const uint8_t*>(&d);
  if(p[0] == 0x12){
    std::cout << "actual endian: big" << std::endl;
  }
  else  {
    std::cout << "actual endian: big" << std::endl;
  }
}

void test_data_chunk()
{
  const std::string hello_world = "Hello, World!";
  const std::string hello_akiba = "Hello, Akiba!";

  auto chunk0 = data_chunk::invalid();
  auto chunk1 = data_chunk::create(hello_world.c_str(), hello_world.size());
  auto chunk2 = data_chunk::from_hex_string(chunk1.to_hex_string());
  auto chunk3 = data_chunk::create(hello_akiba.c_str(), hello_akiba.size());

  assert(chunk0.is_valid() == false);
  assert(chunk0.is_empty() == true);
  assert(chunk0.size() == 0);
  assert(chunk0.to_string() == "");
  assert(chunk0.to_hex_string() == "");
  assert(chunk0 == chunk0);
  assert(chunk0 != chunk1);
  assert(is_exception_thrown([&chunk0](){ chunk0.raw_data_ptr(); }));
  assert(is_exception_thrown([&chunk0](){ chunk0.cbegin(); }));
  assert(is_exception_thrown([&chunk0](){ chunk0.cend(); }));
  assert(is_exception_thrown([&chunk0](){ chunk0.begin(); }));
  assert(is_exception_thrown([&chunk0](){ chunk0.end(); }));

  assert(chunk0.get_range(0, 5).to_string() == "");
  assert(chunk0.get_range(7, 5).to_string() == "");
  assert(chunk0.get_range(7).to_string() == "");
  assert(chunk0.get_range(0, 0).to_string() == "");
  assert(chunk0.get_range(0, 0) == chunk0); /** ranged invalid is invalid */

  assert(chunk1.is_valid() == true);
  assert(chunk1.is_empty() == false);
  assert(chunk1.size() == hello_world.size());
  assert(chunk1.to_string() == hello_world);
  assert(chunk1.to_hex_string() == "48656C6C6F2C20576F726C6421");
  assert(chunk1 != chunk0);
  assert(chunk1 == chunk1);
  assert(chunk1 != chunk3);
  assert(!is_exception_thrown([&chunk1](){ chunk1.raw_data_ptr(); }));
  assert(!is_exception_thrown([&chunk1](){ chunk1.cbegin(); }));
  assert(!is_exception_thrown([&chunk1](){ chunk1.cend(); }));
  assert(!is_exception_thrown([&chunk1](){ chunk1.begin(); }));
  assert(!is_exception_thrown([&chunk1](){ chunk1.end(); }));

  assert(chunk1.get_range(0, 5).to_string() == "Hello");
  assert(chunk1.get_range(7, 5).to_string() == "World");
  assert(chunk1.get_range(7).to_string() == "World!");
  assert(chunk1.get_range(0, 0).to_string() == "");
  assert(chunk1.get_range(0, 0).size() == 0);
  assert(chunk1.get_range(0, 0).to_hex_string() == "");
  assert(chunk1.get_range(0, 0) != chunk0); /** empty is not equal invalid */
  assert(chunk1.get_range(5000, 1000) == chunk0); /** out of range is invalid */
  assert(chunk1.get_range(123) == chunk0); /** out of range is invalid */

  assert(chunk1 == chunk2);

  assert(chunk3 != chunk1);
}

/*
  6F2B800205A0810205A0820139830200008410616263642D313233343536372E637274850100860100870200009000
    6F 2B
      80 02 05A0
      81 02 05A0
      82 01 39
      83 02 0000
      84 10 616263642D313233343536372E637274 abcd-1234567.crt
      85 01 00
      86 01 00
      87 02 0000
    9000
*/

void test_tlv()
{
  auto chunk = data_chunk::from_hex_string(
    "6F2B800205A0810205A0820139830200008410616263642D313233343536372E637274850100860100870200009000"
  );
  auto tlv = tlv::create(chunk);
  assert(tlv.is_valid() == true);
  assert(tlv.size() == chunk.size() - 2);
  assert(tlv.get_tag() == tlv::tag::create({0x6F}));
  assert(tlv.get_tag() != tlv::tag::create({0x80}));
  assert(tlv.get_value() == chunk.get_range(2, chunk.size() - 4));

  auto children = tlv.get_children();
  assert(children.size() == 8);
  assert(children[0].get_tag() == tlv::tag::create({0x80}));
  assert(children[1].get_tag() == tlv::tag::create({0x81}));
  assert(children[2].get_tag() == tlv::tag::create({0x82}));
  assert(children[3].get_tag() == tlv::tag::create({0x83}));
  assert(children[4].get_tag() == tlv::tag::create({0x84}));
  assert(children[5].get_tag() == tlv::tag::create({0x85}));
  assert(children[6].get_tag() == tlv::tag::create({0x86}));
  assert(children[7].get_tag() == tlv::tag::create({0x87}));

  assert(children[0].get_length().get_value_length() == 2);
  assert(children[1].get_length().get_value_length() == 2);
  assert(children[2].get_length().get_value_length() == 1);
  assert(children[3].get_length().get_value_length() == 2);
  assert(children[4].get_length().get_value_length() == 16);
  assert(children[5].get_length().get_value_length() == 1);
  assert(children[6].get_length().get_value_length() == 1);
  assert(children[7].get_length().get_value_length() == 2);

  assert(children[0].get_length().get_value_length() == children[0].get_value().size());
  assert(children[1].get_length().get_value_length() == children[1].get_value().size());
  assert(children[2].get_length().get_value_length() == children[2].get_value().size());
  assert(children[3].get_length().get_value_length() == children[3].get_value().size());
  assert(children[4].get_length().get_value_length() == children[4].get_value().size());
  assert(children[5].get_length().get_value_length() == children[5].get_value().size());
  assert(children[6].get_length().get_value_length() == children[6].get_value().size());
  assert(children[7].get_length().get_value_length() == children[7].get_value().size());

  assert(children[0].get_value().to_hex_string() == "05A0");
  assert(children[1].get_value().to_hex_string() == "05A0");
  assert(children[2].get_value().to_hex_string() == "39");
  assert(children[3].get_value().to_hex_string() == "0000");
  assert(children[4].get_value().to_hex_string() == "616263642D313233343536372E637274");
  assert(children[5].get_value().to_hex_string() == "00");
  assert(children[6].get_value().to_hex_string() == "00");
  assert(children[7].get_value().to_hex_string() == "0000");

  assert(children[4].get_value().to_string() == "abcd-1234567.crt");

  assert(is_exception_thrown([&children]{ children[0].get_value().to_uint8(); }));
  assert(children[0].get_value().to_uint16() == 0x05A0);
  assert(is_exception_thrown([&children]{ children[0].get_value().to_uint32(); }));
  assert(is_exception_thrown([&children]{ children[0].get_value().to_uint64(); }));
  assert(children[0].get_value().to_uint() == 0x05A0);

  auto child_84 = tlv.find_first_child(tlv::tag::create({0x84}));
  assert(child_84.get_tag() == tlv::tag::create({0x84}));
  assert(child_84.get_value().to_hex_string() == "616263642D313233343536372E637274");
  assert(child_84.get_value().to_string() == "abcd-1234567.crt");

  auto child_82 = tlv.find_first_child(tlv::tag::create({0x82}));
  assert(child_82.get_tag() == tlv::tag::create({0x82}));
  assert(child_82.get_value().to_hex_string() == "39");
  assert(child_82.get_value().to_uint8() == 0x39);
}


void test_rapdu()
{
  auto chunk = data_chunk::from_hex_string(
    "6F2B800205A0810205A0820139830200008410616263642D313233343536372E637274850100860100870200009000"
  );
  auto rapdu = rapdu::create(chunk);
  assert(rapdu.is_valid() == true);
  assert(rapdu.data().size() == chunk.size() - 2);
  assert(rapdu.status_word().data().to_hex_string() == "9000");
  assert(rapdu.status_word().is_ok() == true);
  assert(rapdu.get_tlv().get_tag() == tlv::tag::create({0x6F}));

  std::cout << "status word: " << rapdu.status_word().data().to_hex_string() << std::endl;
  auto _tlv = rapdu.get_tlv();
  {
    std::cout << tlv_descriptor::to_string(_tlv) << std::endl;
  }
}

int main()
{
  test_endian();
  test_data_chunk();
  test_tlv();
  test_rapdu();
  return 0;
}