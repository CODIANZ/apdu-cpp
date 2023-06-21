#include <data_chunk.h>
#include <tlv_descriptor.h>
#include <tlv.h>
#include <rapdu.h>
#include <iostream>

using namespace apdu_cpp;

void test_data_chunk()
{
  const std::string str = "Hello, World!";
  auto chunk = data_chunk::create(str.c_str(), str.size());
  std::cout << "to_hex_string: " << chunk.to_hex_string() << std::endl;
  std::cout << "to_string: " << chunk.to_string() << std::endl;

  auto chunk2 = data_chunk::from_hex_string(chunk.to_hex_string());
  std::cout << "to_hex_string: " << chunk2.to_hex_string() << std::endl;
  std::cout << "to_string: " << chunk2.to_string() << std::endl;
}

void test_rapdu()
{
  auto chunk = data_chunk::from_hex_string(
    "6F2B800205A0810205A082013983020000841074656D702D6B65796C6F61642E637274850100860100870200009000"
  );
  auto rapdu = rapdu::create(chunk);
  std::cout << "status word: " << rapdu.get_status_word().data().to_hex_string() << std::endl;
  auto _tlv = rapdu.get_tlv();
  {
    std::cout << tlv_descriptor::to_string(_tlv) << std::endl;
  }
}

int main()
{
  test_data_chunk();
  test_rapdu();
  return 0;
}