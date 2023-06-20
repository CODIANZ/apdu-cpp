#include <data_chunk.h>
#include <data_chunk_converter.h>
#include <iostream>

int main()
{
  auto chunk = apdu_cpp::data_chunk::from((uint8_t*)"Hello, World!", 13);
  auto hex = apdu_cpp::data_chunk_converter::to_hex_string(chunk);
  auto str = apdu_cpp::data_chunk_converter::to_string(chunk);
  std::cout << hex << std::endl;
  std::cout << str << std::endl;
  auto chunk2 = apdu_cpp::data_chunk_converter::from_hex_string(hex);
  std::cout << apdu_cpp::data_chunk_converter::to_string(chunk2) << std::endl;
  return 0;
}