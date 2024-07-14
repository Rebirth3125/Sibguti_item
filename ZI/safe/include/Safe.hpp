#pragma once

#include <vector>
#include <utility>
#include <cmath>
#include <array>
#include <string>
#include <future>
#include <iostream>

namespace Safe
{
    using long_num = long long;
    namespace Math
    {
        template <class T>
        T mod(T a, T b)
        {
            return a % b;
        }
        long_num get_digit(long_num number, long_num n)
        {
            return (number >> n) & 1;
        }

        long_num pow_mod(long_num a, long_num x, long_num p)
        {
            long_num max_pow = 0;
            while ((1l << ++max_pow) < x)
            {
            }
            long_num next_a = 1;
            for (long_num i = max_pow - 1; i >= 0; --i)
            {
                // next_a = (next_a * next_a) % p;
                next_a = mod(next_a * next_a, p);
                long_num digit = get_digit(x, i);
                if (digit == 1)
                {
                    // next_a = (next_a * a) % p;
                    next_a = mod(next_a * a, p);
                }
            }
            return mod(next_a, p);
        }
        std::vector<long_num> fast_log(long_num a, long_num p, long_num b)
        {
            long_num m = std::ceil(std::pow(p, 0.5));
            long_num k = std::ceil(std::pow(p, 0.5));
            std::vector<long_num> X;
            std::vector<std::pair<long_num, long_num>> A;
            std::vector<std::pair<long_num, long_num>> B;
            A.reserve(k + 1);
            for (long_num i = 1; i <= k; ++i)
            {
                A.emplace_back(pow_mod(a, i * m, p), i);
            }
            B.reserve(m + 1);
            for (long long j = 0; j < m; ++j)
            {
                B.emplace_back((b * pow_mod(a, j, p)) % p, j);
            }
            for (auto [a, i] : A)
            {
                for (auto [b, j] : B)
                {
                    if (a == b)
                    {
                        X.push_back(i * m - j);
                    }
                }
            }
            return X;
        }
        std::array<long_num, 3> gcd(long_num a, long_num b)
        {
            std::array<long_num, 3> U = {a, 1, 0};
            std::array<long_num, 3> V = {b, 0, 1};
            std::array<long_num, 3> T;
            while (V[0] != 0)
            {
                long_num q = U[0] / V[0];
                T = {U[0] % V[0], U[1] - q * V[1], U[2] - q * V[2]};
                U = V;
                V = T;
            }
            return U;
        }
        long_num inverse(long_num a, long_num p)
        {
            auto x = gcd(a, p)[1];
            return x > 0 ? x : (x + p);
        }
        long_num neg_mod(long_num a, long_num p)
        {
            long_num tmp = a;
            if (tmp < 0)
            {
                tmp *= -1;
            }
            long_num dev = 0;
            if (tmp < p)
            {
                dev = p - tmp;
                return dev;
            }
            else
            {
                long_num i = 1;
                while (dev < tmp)
                {
                    dev = i * p;
                    i++;
                }
                return dev - tmp;
            }
        }
        template <class T>
        bool is_prime(T number)
        {
            if (number <= 1)
            {
                return false;
            }
            T b = (T)std::pow(number, 0.5);
            for (T i = 2; i <= b; ++i)
            {
                if ((number % i) == 0)
                {
                    return false;
                }
            }
            return true;
        }
        long_num get_num(long_num from, long_num to)
        {
            long_num r1 = static_cast<long_num>(std::rand());
            long_num r2 = static_cast<long_num>(std::rand());
            long_num r = (r1 << (sizeof(int) * 8)) + r2;
            return (r % (to - from + 1)) + from;
        }
        long_num get_prime(long_num from, long_num to)
        {
            long_num number;
            do
            {
                number = get_num(from, to);
            } while (!is_prime(number));
            return number;
        }
        long_num get_prime(long_num range = 0)
        {
            long_num number = 0;
            if (range == 0)
            {
                do
                {
                    number = std::rand();
                } while (!is_prime(number));
            }
            else
            {
                do
                {
                    number = std::rand() % range;
                } while (!is_prime(number));
            }

            return number;
        }
        long_num get_mutually_prime(long_num p, long_num range = 0)
        {
            while (true)
            {
                long_num a = get_prime(range);
                if (gcd(a, p)[0] == 1)
                {
                    return a;
                }
            }
        }
        long_num f_euler(long_num a)
        {
            if (is_prime(a))
            {
                return a - 1;
            }
            long_num y = 0;
            for (long_num i = 0; i < a; ++i)
            {
                if (gcd(a, i)[0] == 1)
                {
                    y += 1;
                }
            }
            return y;
        }
        long_num get_primitive_root(long_num p)
        {
            std::vector<long_num> fact;
            long_num phi = p - 1, n = phi;
            for (long_num i = 2; i * i <= n; ++i)
                if (n % i == 0)
                {
                    fact.push_back(i);
                    while (n % i == 0)
                        n /= i;
                }
            if (n > 1)
                fact.push_back(n);

            for (long_num res = 2; res <= p; ++res)
            {
                bool ok = true;
                for (size_t i = 0; i < fact.size() && ok; ++i)
                    ok &= pow_mod(res, phi / fact[i], p) != 1;
                if (ok)
                    return res;
            }
            return -1;
        }
    };
    namespace Encryption
    {
        class Diffi
        {
        private:
            long_num q;
            long_num p;
            long_num g;

        public:
            Diffi()
            {
                do
                {
                    q = Math::get_prime();
                    p = 2 * q + 1;
                } while (!Math::is_prime(p));
                do
                {
                    g = std::rand() % (p - 1);
                } while (Math::pow_mod(g, q, p) == 1);
            }
            long_num get_close_key()
            {
                return std::rand() % p;
            }
            long_num get_open_key(long_num X)
            {
                return Math::pow_mod(g, X, p);
            }
            long_num get_common_key(long_num Y, long_num X)
            {
                return Math::pow_mod(Y, X, p);
            }
        };
        class Shamir
        {
        private:
            long_num p;
            long_num a;
            long_num a_;
            std::promise<std::vector<long_num>> buff;

        public:
            Shamir(long_num P)
            {
                p = P;
                a = Math::get_mutually_prime(p - 1, p - 1);
                a_ = Math::inverse(a, p - 1);
            }
            long_num get_open_key()
            {
                return p;
            }
            std::vector<long_num> encode(const std::vector<long_num> &message)
            {
                std::vector<long_num> vec;
                vec.reserve(message.size());
                for (auto l : message)
                {
                    vec.push_back(Math::pow_mod(l, a, p));
                }
                return vec;
            }
            void encode(std::istream &is, std::ostream &os)
            {
                while (!is.eof())
                {
                    long_num c;
                    is.read(reinterpret_cast<char *>(&c), sizeof(c));
                    if (is.eof())
                    {
                        break;
                    }
                    long_num o = Math::pow_mod(c, a, p);
                    os.write(reinterpret_cast<char *>(&o), sizeof(o));
                }
            }
            std::vector<long_num> decode(const std::vector<long_num> &M)
            {
                std::vector<long_num> vec;
                vec.reserve(M.size());
                for (auto l : M)
                {
                    vec.push_back(Math::pow_mod(l, a_, p));
                }
                return vec;
            }
            void decode(std::istream &is, std::ostream &os)
            {
                while (!is.eof())
                {
                    long_num c;
                    is.read(reinterpret_cast<char *>(&c), sizeof(c));
                    if (is.eof())
                    {
                        break;
                    }
                    long_num o = Math::pow_mod(c, a_, p);
                    os.write(reinterpret_cast<char *>(&o), sizeof(o));
                }
            }
            static std::string convert(const std::vector<long_num> &vec)
            {
                std::string message;
                for (auto l : vec)
                {
                    message.push_back(static_cast<char>(l));
                }
                return message;
            }
            static std::vector<long_num> convert(const std::string &message)
            {
                std::vector<long_num> vec;
                vec.reserve(message.length() + 1);
                for (auto c : message)
                {
                    vec.push_back(static_cast<long_num>(c));
                }
                return vec;
            }
            void operator<<(std::vector<long_num> vec)
            {
                buff.set_value(vec);
            }
            void operator>>(std::vector<long_num> &vec)
            {
                vec = buff.get_future().get();
                buff = std::promise<std::vector<long_num>>();
            }
        };
        class LGamal
        {
        private:
            long_num p;
            long_num g;
            long_num y;
            long_num x;

        public:
            LGamal(long_num prime)
            {
                p = prime;
                g = Math::get_primitive_root(p);
                x = Math::get_num(1 + 1, p - 1 - 1);
                y = Math::pow_mod(g, x, p);
            }
            LGamal(long_num p, long_num g)
            {
                x = Math::get_num(1 + 1, p - 1 - 1);
                y = Math::pow_mod(g, x, p);
            }
            std::tuple<long_num, long_num, long_num> get_open_key()
            {
                return {p, g, y};
            }
            std::vector<long_num> encode(const std::vector<long_num> &message, std::tuple<long_num, long_num, long_num> open_key)
            {
                std::vector<long_num> vec;
                vec.reserve(message.size() * 2);
                auto [p, g, y] = open_key;
                for (auto m : message)
                {
                    long_num k;
                    k = Math::get_mutually_prime(p - 1, p - 1);
                    long_num a = Math::pow_mod(g, k, p);
                    long_num b = (m * Math::pow_mod(y, k, p)) % p;
                    vec.push_back(a);
                    vec.push_back(b);
                }
                return vec;
            }
            void encode(std::istream &is, std::ostream &os, std::tuple<long_num, long_num, long_num> open_key)
            {
                auto [p, g, y] = open_key;
                while (!is.eof())
                {
                    long_num m;
                    char c;
                    is.read(reinterpret_cast<char *>(&c), sizeof(char));
                    m = static_cast<long_num>(c);
                    if (is.eof())
                    {
                        break;
                    }
                    long_num k;
                    k = Math::get_mutually_prime(p - 1, p - 1);
                    long_num a = Math::pow_mod(g, k, p);
                    long_num b = (m * Math::pow_mod(y, k, p)) % p;
                    os.write(reinterpret_cast<char *>(&a), sizeof(a));
                    os.write(reinterpret_cast<char *>(&b), sizeof(b));
                }
            }
            void decode(std::istream &is, std::ostream &os)
            {
                while (!is.eof())
                {
                    long_num a;
                    long_num b;
                    is.read(reinterpret_cast<char *>(&a), sizeof(a));
                    is.read(reinterpret_cast<char *>(&b), sizeof(b));
                    if (is.eof())
                    {
                        break;
                    }
                    long_num m = (b * Math::pow_mod(a, p - 1 - x, p)) % p;
                    // os << static_cast<char>(m);
                    os.write(reinterpret_cast<char *>(&m), sizeof(char));
                }
            }
            std::vector<long_num> decode(const std::vector<long_num> &vec)
            {
                std::vector<long_num> message;
                message.reserve(vec.size() / 2);
                for (std::size_t i = 0; i < vec.size(); i += 2)
                {
                    long_num a = vec[i];
                    long_num b = vec[i + 1];
                    long_num m = (b * Math::pow_mod(a, p - 1 - x, p)) % p;
                    message.push_back(m);
                }
                return message;
            }
            static std::vector<long_num> convert(const std::string &message)
            {
                std::vector<long_num> vec;
                vec.reserve(message.length() + 1);
                for (auto c : message)
                {
                    vec.push_back(static_cast<long_num>(c));
                }
                return vec;
            }
            static std::string convert(const std::vector<long_num> &vec)
            {
                std::string message;
                for (auto l : vec)
                {
                    message.push_back(static_cast<char>(l));
                }
                return message;
            }
        };

        class Vernam
        {
        public:
            static std::vector<char> generate(long_num l)
            {
                std::vector<char> key;
                key.reserve(l);
                for (long_num i = 0; i < l; ++i)
                {
                    key.push_back(Math::get_num(0, 255));
                }
                return key;
            }
            static void generate(std::ostream &os, long_num l)
            {
                for (long_num i = 0; i < l; ++i)
                {
                    auto n = Math::get_num(0, 255);
                    os.write(reinterpret_cast<char *>(&n), 1);
                }
            }
            static std::vector<char> encode(const std::string &message, const std::vector<char> &key)
            {
                std::vector<char> encoded;
                encoded.reserve(message.length());
                for (long_num i = 0; i < message.length(); ++i)
                {
                    encoded.push_back(message[i] ^ key[i]);
                }
                return encoded;
            }
            static void encode(std::istream &is, std::ostream &os, std::istream &key)
            {
                while (!is.eof())
                {
                    char m;
                    char k;
                    is.read(&m, 1);
                    key.read(&k, 1);
                    if (is.eof())
                    {
                        break;
                    }
                    char e = (m ^ k);
                    os.write(&e, 1);
                }
            }
            static std::string decode(const std::vector<char> &encoded, const std::vector<char> &key)
            {
                std::string message;
                message.reserve(encoded.size());
                for (long_num i = 0; i < encoded.size(); ++i)
                {
                    message.push_back(encoded[i] ^ key[i]);
                }
                return message;
            }
            static void decode(std::istream &is, std::ostream &os, std::istream &key)
            {
                while (!is.eof())
                {
                    char m;
                    char k;
                    is.read(&m, 1);
                    key.read(&k, 1);
                    if (is.eof())
                    {
                        break;
                    }
                    char e = (m ^ k);
                    os.write(&e, 1);
                }
            }
        };

        class RSA
        {
        private:
            long_num n;
            long_num e;
            long_num d;

        public:
            RSA()
            {
                long_num p = Math::get_prime(100000);
                // p = 3557;
                long_num q = Math::get_prime(100000);
                // q = 2579;
                n = p * q;
                long_num phi = (p - 1) * (q - 1);
                e = Math::get_mutually_prime(phi, phi);
                d = Math::inverse(e, phi);
            }
            std::pair<long_num, long_num> get_open_key()
            {
                return {e, n};
            }
            long_num get_close_key() {
                return d;
            }
            std::vector<long_num> encode(const std::string &message, std::pair<long_num, long_num> open_key)
            {
                std::vector<long_num> vec;
                vec.reserve(message.length());
                auto [e, n] = open_key;
                for (auto m : message)
                {
                    vec.push_back(Math::pow_mod(m, e, n));
                }
                return vec;
            }
            void encode(std::istream &is, std::ostream &os, std::pair<long_num, long_num> open_key)
            {
                auto [e, n] = open_key;
                while (true)
                {
                    char c;
                    is.read(&c, 1);
                    if (is.eof())
                    {
                        return;
                    }
                    long_num m = c;
                    auto v = Math::pow_mod(m, e, n);
                    os.write(reinterpret_cast<char *>(&v), sizeof(v));
                }
            }
            void decode(std::istream &is, std::ostream &os)
            {
                while (true)
                {
                    long_num c;
                    is.read(reinterpret_cast<char *>(&c), sizeof(c));
                    if (is.eof())
                    {
                        return;
                    }
                    auto v = Math::pow_mod(c, d, n);
                    char m = v;
                    os.write(reinterpret_cast<char *>(&m), sizeof(m));
                }
            }
            std::string decode(std::vector<long_num> vec)
            {
                std::string message;
                message.reserve(vec.size());
                for (auto c : vec)
                {
                    message.push_back(static_cast<char>(Math::pow_mod(c, d, n)));
                }
                return message;
            }
        };

    };
};
