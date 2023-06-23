# Simple APDU handling library

## Usage

1. Add this repository's `include` directory to the header search path.
1. Add `#include <apdu_cpp/apdu_cpp.h>` in your file.
1. The library namespace is `apdu_cpp`. Add `using namespace adpu_cpp;` if necessary.


## Sample

```cpp
#include <apdu_cpp/apdu_cpp.h>
using namespace apdu_cpp;

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
int main() {
  auto chunk = data_chunk::from_hex_string(
    "6F2B800205A0810205A0820139830200008410616263642D313233343536372E637274850100860100870200009000"
  );
  auto rapdu = rapdu::create(chunk);
  std::cout << "status word: " << rapdu.status_word().data().to_hex_string() << std::endl;
  auto _tlv = rapdu.get_tlv();
  std::cout << tlv_descriptor::to_string(_tlv) << std::endl;
}
```

```log
status word: 9000
tag: 6F
 - class_type: application
 - is_constructed: true
 - is_primitive: false
length:2B(43)
value: 800205A0810205A0820139830200008410616263642D313233343536372E63727485010086010087020000
children: 
  [0]
    tag: 80
     - class_type: context_specific
     - is_constructed: false
     - is_primitive: true
    length:02(2)
    value: 05A0
  [1]
    tag: 81
     - class_type: context_specific
     - is_constructed: false
     - is_primitive: true
    length:02(2)
    value: 05A0
  [2]
    tag: 82
     - class_type: context_specific
     - is_constructed: false
     - is_primitive: true
    length:01(1)
    value: 39
  [3]
    tag: 83
     - class_type: context_specific
     - is_constructed: false
     - is_primitive: true
    length:02(2)
    value: 0000
  [4]
    tag: 84
     - class_type: context_specific
     - is_constructed: false
     - is_primitive: true
    length:10(16)
    value: 616263642D313233343536372E637274
  [5]
    tag: 85
     - class_type: context_specific
     - is_constructed: false
     - is_primitive: true
    length:01(1)
    value: 00
  [6]
    tag: 86
     - class_type: context_specific
     - is_constructed: false
     - is_primitive: true
    length:01(1)
    value: 00
  [7]
    tag: 87
     - class_type: context_specific
     - is_constructed: false
     - is_primitive: true
    length:02(2)
    value: 0000
```