#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <openssl/md5.h>

namespace mp = boost::multiprecision;
namespace r = boost::random;

typedef boost::multiprecision::cpp_int upint;
typedef boost::multiprecision::int1024_t pint;
typedef boost::multiprecision::uint256_t qint;
typedef boost::multiprecision::int256_t sqint;
typedef r::independent_bits_engine<r::mt19937, 1024, mp::int1024_t> generator1024_type;
typedef r::independent_bits_engine<r::mt19937, 256, mp::int256_t> generator256_type;

struct ElgamalKeys
{
    long int p, g, c1, c2, d1, d2, k, r;
};

struct RSA_Open_key
{
    long long int n, e;
};


typedef long long int RSA_Secret_key;

struct RSA_Signed_msg {
    std::vector<char> data; // Данные сообщения
    std::vector<long long> r; // Исходные байты хэша
    std::vector<long long> s; // Подписанные значения
};


struct Elgamal_Open_key
{
    long long int y, p, g;
};

struct Elgamal_Secret_key {
    long x; // секретный ключ
};

struct Elgamal_Signed_msg {
    std::vector<char> msg;
    long long s;
    long r;
};

// struct Elgamal_Signed_msg
// {
//     std::vector<char> data;
//     long long int s, r;
// };

struct Gost_Open_keys
{
    pint q, p, y, a;
};

struct Gost_Signed_msg
{
    std::vector<char> msg;
    pint r, s;
};