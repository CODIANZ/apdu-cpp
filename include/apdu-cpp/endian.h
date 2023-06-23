#if !defined(__apdu_cpp_h_endian__)
#define __apdu_cpp_h_endian__
namespace apdu_cpp {

enum class endian {
  big, little, unknown
};

#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
constexpr endian system_endian = endian::big;
#elif defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
constexpr endian system_endian = endian::little;
#else
constexpr endian system_endian = endian::unknown;
#endif

#if __cplusplus >= 201703L
#define APDU_CPP_CONSTEXPR constexpr
#else
#define APDU_CPP_CONSTEXPR
#endif

} /** namespace apdu_cpp */
#endif /** !defined(__apdu_cpp_h_endian__) */