#pragma once

#include "Safe.hpp"
#include <filesystem>
#include <fstream>
#include "MD5.h"
#include <newbigint.h>

namespace Safe
{
    namespace Signature
    {
        std::vector<char> read(std::ifstream &f)
        {
            std::vector<char> message;
            while (!f.eof())
            {
                char c;
                f.read(&c, 1);
                if (f.eof())
                {
                    break;
                }
                message.emplace_back(c);
            }
            return message;
        }
        std::vector<char> read(const std::filesystem::path path_to_file)
        {
            std::ifstream f(path_to_file.string(), std::ios_base::binary);
            return read(f);
        }

        class LGamal
        {
        private:
            long_num p;
            long_num g;
            long_num y;
            long_num x;

        public:
            LGamal(long_num p_)
            {
                p = p_;
                std::cout << "p = " << p << std::endl;
                g = Math::get_primitive_root(p);
                std::cout << "g = " << g << std::endl;
                x = Math::get_num(1 + 1, p - 1 - 1);
                std::cout << "x = " << x << std::endl;
                y = Math::pow_mod(g, x, p);
                std::cout << "y = " << y << std::endl;
            }
            LGamal(long_num p_, long_num g_)
            {
                p = p_;
                g = g_;
                x = Math::get_num(1 + 1, p - 1 - 1);
                y = Math::pow_mod(g, x, p);
            }
            std::tuple<long_num, long_num, long_num> get_open_key() const
            {
                return {p, g, y};
            }
            void subscribe_in(std::filesystem::path path_to_file)
            {
                std::filesystem::path subscribed_path{path_to_file.string() + ".sgn"};
                std::vector<char> message;
                std::ifstream f(path_to_file.string(), std::ios_base::binary);
                while (!f.eof())
                {
                    char c;
                    f.read(&c, 1);
                    if (f.eof())
                    {
                        break;
                    }
                    message.emplace_back(c);
                }
                MD5 tmp_hash(message);                               // digest
                std::string hash = tmp_hash.hexdigest();             // Получаем hash сообщения
                std::vector<long long> hash_vt = HashToVector(hash); // Перевод hash в целочисленный vector из 8 элеметов
                long long *hash_arr = HashToArray(hash);
                long_num k;
                k = Math::get_mutually_prime(p - 1, p - 1);
                std::ofstream out(subscribed_path.string(), std::ios_base::binary);
                long_num r = Math::pow_mod(g, k, p);
                for (int i = 0; i < 8; ++i)
                { // ошибка?
                    auto h = hash_arr[i];
                    long_num u = Math::pow_mod(h - x * r, 1, p - 1);
                    long_num k_1 = Math::inverse(k, p - 1);
                    long_num s = Math::pow_mod(k_1 * u, 1, p - 1);
                    out.write(reinterpret_cast<char *>(&r), sizeof(r));
                    out.write(reinterpret_cast<char *>(&s), sizeof(s));
                }
                out.write((message.data()), message.size());
            }
            void subscribe(std::filesystem::path path_to_file)
            {
                std::filesystem::path subscribed_path{path_to_file.string() + ".sig"};
                std::vector<char> message;
                std::ifstream f(path_to_file.string(), std::ios_base::binary);
                while (!f.eof())
                {
                    char c;
                    f.read(&c, 1);
                    if (f.eof())
                    {
                        break;
                    }
                    message.emplace_back(c);
                }
                MD5 tmp_hash(message);                               // digest
                std::string hash = tmp_hash.hexdigest();             // Получаем hash сообщения
                std::vector<long long> hash_vt = HashToVector(hash); // Перевод hash в целочисленный vector из 8 элеметов
                long long *hash_arr = HashToArray(hash);
                long_num k;
                k = Math::get_mutually_prime(p - 1, p - 1);
                std::ofstream out(subscribed_path.string(), std::ios_base::binary);
                long_num r = Math::pow_mod(g, k, p);
                for (int i = 0; i < 8; ++i)
                { // ошибка?
                    auto h = hash_arr[i];
                    long_num u = Math::pow_mod(h - x * r, 1, p - 1);
                    long_num k_1 = Math::inverse(k, p - 1);
                    long_num s = Math::pow_mod(k_1 * u, 1, p - 1);
                    out.write(reinterpret_cast<char *>(&r), sizeof(r));
                    out.write(reinterpret_cast<char *>(&s), sizeof(s));
                }
            }
            bool check(const LGamal &client, std::filesystem::path subscribed_path)
            {

                std::ifstream in(subscribed_path.string(), std::ios_base::binary);
                std::vector<std::pair<long_num, long_num>> rs;
                for (int i = 0; i < 8; ++i)
                { // ошибка?
                    long_num r;
                    long_num s;
                    in.read(reinterpret_cast<char *>(&r), sizeof(r));
                    in.read(reinterpret_cast<char *>(&s), sizeof(s));
                    rs.emplace_back(r, s);
                }

                std::vector<char> message;
                // std::ifstream f(path_to_file.string(), std::ios_base::binary);
                while (!in.eof())
                {
                    char c;
                    in.read(&c, 1);
                    if (in.eof())
                    {
                        break;
                    }
                    message.emplace_back(c);
                }
                MD5 tmp_hash(message);                               // digest
                std::string hash = tmp_hash.hexdigest();             // Получаем hash сообщения
                std::vector<long long> hash_vt = HashToVector(hash); // Перевод hash в целочисленный vector из 8 элеметов
                long long *hash_arr = HashToArray(hash);
                auto [p, g, y] = client.get_open_key();
                for (int i = 0; i < 8; ++i)
                {
                    auto h = hash_arr[i];
                    auto [r, s] = rs[i];
                    if (Math::mod((Math::pow_mod(y, r, p) * Math::pow_mod(r, s, p)), p) == Math::pow_mod(g, h, p))
                    {
                        std::cout << "y = " << y << "\tr = " << r << "\tp = " << p << "\t=" << Math::pow_mod(y, r, p) << std::endl;
                        std::cout << "r = " << r << "\ts = " << s << "\tp = " << p << "\t=" << Math::pow_mod(r, s, p) << std::endl;
                        std::cout << "g = " << g << "\th = " << h << "\tp = " << p << "\t=" << Math::pow_mod(g, h, p) << std::endl;
                        return false;
                    }
                }

                return true;
            }
            bool check(const LGamal &client, std::filesystem::path path_to_file, std::filesystem::path subscribed_path)
            {
                std::vector<char> message;
                std::ifstream f(path_to_file.string(), std::ios_base::binary);
                while (!f.eof())
                {
                    char c;
                    f.read(&c, 1);
                    if (f.eof())
                    {
                        break;
                    }
                    message.emplace_back(c);
                }
                MD5 tmp_hash(message);                               // digest
                std::string hash = tmp_hash.hexdigest();             // Получаем hash сообщения
                std::vector<long long> hash_vt = HashToVector(hash); // Перевод hash в целочисленный vector из 8 элеметов
                long long *hash_arr = HashToArray(hash);
                auto [p, g, y] = client.get_open_key();
                std::ifstream in(subscribed_path.string(), std::ios_base::binary);
                for (int i = 0; i < 8; ++i)
                { // ошибка?
                    auto h = hash_arr[i];
                    long_num r;
                    long_num s;
                    in.read(reinterpret_cast<char *>(&r), sizeof(r));
                    in.read(reinterpret_cast<char *>(&s), sizeof(s));
                    if (Math::mod((Math::pow_mod(y, r, p) * Math::pow_mod(r, s, p)), p) == Math::pow_mod(g, h, p))
                    {
                        std::cout << "y = " << y << "\tr = " << r << "\tp = " << p << "\t=" << Math::pow_mod(y, r, p) << std::endl;
                        std::cout << "r = " << r << "\ts = " << s << "\tp = " << p << "\t=" << Math::pow_mod(r, s, p) << std::endl;
                        std::cout << "g = " << g << "\th = " << h << "\tp = " << p << "\t=" << Math::pow_mod(g, h, p) << std::endl;
                        return false;
                    }
                }
                return true;
            }
        };

        class RSA
        {
        private:
            long_num n;
            long_num c;
            long_num d;

        public:
            RSA()
            {
                long_num p = Math::get_prime(10000);
                long_num q = Math::get_prime(10000);
                n = p * q;
                long_num phi = (p - 1) * (q - 1);
                c = Math::get_mutually_prime(phi, phi);
                d = Math::inverse(c, phi);
            }
            std::pair<long_num, long_num> get_open_key()
            {
                return {d, n};
            }
            void subscribe(std::filesystem::path path_to_file)
            {
                std::vector<char> message;
                std::ifstream f(path_to_file.string(), std::ios_base::binary);
                while (!f.eof())
                {
                    char c;
                    f.read(&c, 1);
                    if (f.eof())
                    {
                        break;
                    }
                    message.emplace_back(c);
                }
                MD5 tmp_hash(message);
                std::string hash = tmp_hash.hexdigest();
                std::vector<long long> hash_vt = HashToVector(hash);
                long long *hash_arr = HashToArray(hash);
                std::ofstream out(path_to_file.string() + ".sig", std::ios_base::binary);
                for (int i = 0; i < 8; ++i)
                {
                    long_num h = hash_arr[i];
                    long_num s = Math::pow_mod(h, c, n);
                    out.write(reinterpret_cast<char *>(&s), sizeof(s));
                }
            }
            void subscribe_in(std::filesystem::path path_to_file)
            {
                std::vector<char> message;
                std::ifstream f(path_to_file.string(), std::ios_base::binary);
                while (!f.eof())
                {
                    char c;
                    f.read(&c, 1);
                    if (f.eof())
                    {
                        break;
                    }
                    message.emplace_back(c);
                }
                MD5 tmp_hash(message);
                std::string hash = tmp_hash.hexdigest();
                std::vector<long long> hash_vt = HashToVector(hash);
                long long *hash_arr = HashToArray(hash);
                std::ofstream out(path_to_file.string() + ".sgn", std::ios_base::binary);
                for (int i = 0; i < 8; ++i)
                {
                    long_num h = hash_arr[i];
                    long_num s = Math::pow_mod(h, c, n);
                    out.write(reinterpret_cast<char *>(&s), sizeof(s));
                }
                out.write(message.data(), message.size());
            }
            bool check(RSA client, std::filesystem::path path_to_file, std::filesystem::path signature_path)
            {
                std::vector<char> message;
                std::ifstream f(path_to_file.string(), std::ios_base::binary);
                while (!f.eof())
                {
                    char c;
                    f.read(&c, 1);
                    if (f.eof())
                    {
                        break;
                    }
                    message.emplace_back(c);
                }
                MD5 tmp_hash(message);
                std::string hash = tmp_hash.hexdigest();
                std::vector<long long> hash_vt = HashToVector(hash);
                long long *hash_arr = HashToArray(hash);
                std::ifstream in(signature_path.string(), std::ios_base::binary);
                auto [d, n] = client.get_open_key();
                for (int i = 0; i < 8; ++i)
                {
                    long_num s;
                    in.read(reinterpret_cast<char *>(&s), sizeof(s));
                    long_num h = hash_arr[i];
                    if (h != (Math::pow_mod(s, d, n)))
                    {
                        return false;
                    }
                }
                return true;
            }
            bool check(RSA client, std::filesystem::path path_to_file)
            {
                std::vector<char> message;
                std::ifstream f(path_to_file.string(), std::ios_base::binary);
                auto [d, n] = client.get_open_key();
                std::vector<long_num> s_array;
                for (int i = 0; i < 8; ++i)
                {
                    long_num s;
                    f.read(reinterpret_cast<char *>(&s), sizeof(s));
                    s_array.emplace_back(s);
                }
                while (!f.eof())
                {
                    char c;
                    f.read(&c, 1);
                    if (f.eof())
                    {
                        break;
                    }
                    message.emplace_back(c);
                }
                MD5 tmp_hash(message);
                std::string hash = tmp_hash.hexdigest();
                std::vector<long long> hash_vt = HashToVector(hash);
                long long *hash_arr = HashToArray(hash);
                for (int i = 0; i < 8; ++i)
                {
                    long_num s = s_array[i];
                    long_num h = hash_arr[i];
                    if (h != (Math::pow_mod(s, d, n)))
                    {
                        return false;
                    }
                }
                return true;
            }
        };

        class GOST
        {
        private:
            long_num p, q, a;

        public:
            std::tuple<long_num, long_num, long_num> get_common_key()
            {
                return {p, q, a};
            }
            GOST()
            {

                do
                {
                    q = Math::get_prime(10, 1000);
                    // p = Math::get_prime(); // 1024 бит
                    long_num b = Math::get_num(2, 100000);
                    p = b * q + 1;
                    // q = (p - 1) / b;
                    // std::cout << p - 1 << " % " << q << " = " << Math::mod(p - 1, q) << std::endl;
                } while (!Math::is_prime(p));
                std::cout << "p q generated\n";
                std::cout << "p == " << p << std::endl;
                std::cout << "q == " << q << std::endl;
                long_num g;

                do
                {
                    g = Math::get_num(2, p); // случайное число > 1
                    a = Math::pow_mod(g, (p - 1) / q, p);
                    // std::cout << g << "^" << (p - 1) / q << " mod " << p << " = " << a << std::endl;
                } while (a <= 1);
                std::cout << (Math::pow_mod(a, q, p)) << std::endl;
                std::cout << "(p - 1) / q == " << ((p - 1) / q) << std::endl;
                std::cout << "g == " << g << std::endl;
                std::cout << "a == " << a << std::endl;
                std::cout << "g a generated\n";
            }
            long_num get_close_key()
            {
                long_num x = Math::get_num(1, q - 1); // секретный ключ 0 < x < q
                return x;
            }
            long_num get_open_key(long_num x)
            {
                long_num y = Math::pow_mod(a, x, p); // открытый ключ
                return y;
            }
            void write(const std::filesystem::path &path_to_file, const std::pair<long_num, std::vector<long_num>> &sign)
            {
                std::ofstream f(path_to_file.string());
                auto [r, s] = sign;
                f.write(reinterpret_cast<char *>(&r), sizeof(r));
                f.write(reinterpret_cast<char *>(s.data()), s.size() * sizeof(long_num));
            }
            std::pair<long_num, std::vector<long_num>> read(std::ifstream &f)
            {
                // std::ifstream f(path_to_file.string());
                long_num r;
                f.read(reinterpret_cast<char *>(&r), sizeof(r));
                std::vector<long_num> s;
                s.resize(8);
                f.read(reinterpret_cast<char *>(s.data()), s.size() * sizeof(long_num));
                return {r, s};
            }
            void write(const std::filesystem::path &path_to_file, const std::pair<long_num, std::vector<long_num>> &sign, const std::vector<char> &message)
            {
                std::ofstream f(path_to_file.string());
                auto [r, s] = sign;
                f.write(reinterpret_cast<char *>(&r), sizeof(r));
                f.write(reinterpret_cast<char *>(s.data()), s.size() * sizeof(long_num));
                f.write(message.data(), message.size());
            }
            std::pair<long_num, std::vector<long_num>> subscribe(const std::vector<char> &message, long_num x)
            {
                MD5 tmp_hash(message);
                std::string hash = tmp_hash.hexdigest();
                std::vector<long long> hash_vt = HashToVector(hash);
                long long *hash_arr = HashToArray(hash);
                long_num k;
                long_num r;
                std::vector<long_num> s;
                s.resize(8);
                bool is_good = true;
                do
                {
                    do
                    {
                        k = Math::get_num(0 + 1, q - 1); // случайное число 0 < k < q
                        r = Math::mod(Math::pow_mod(a, k, p), q);
                    } while (r == 0);
                    for (int i = 0; i < 8; ++i)
                    {
                        long_num h = hash_arr[i];
                        s[i] = Math::mod(k * h + x * r, q);
                        if (s[i] < 0)
                        {
                            std::cout << "a == " << a << std::endl;
                            std::cout << k << std::endl;
                            std::cout << h << std::endl;
                            std::cout << x << std::endl;
                            std::cout << "r == " << r << std::endl;
                        }
                        if (s[i] == 0)
                        {
                            is_good = false;
                            break;
                        }
                    }
                } while (!is_good);
                return {r, s};
            }
            bool check(const std::vector<char> &message, long_num y, std::pair<long_num, std::vector<long_num>> sign)
            {
                MD5 tmp_hash(message);
                std::string hash = tmp_hash.hexdigest();
                std::vector<long long> hash_vt = HashToVector(hash);
                long long *hash_arr = HashToArray(hash);
                auto [r, s] = sign;
                for (int i = 0; i < 8; ++i)
                {

                    long_num h = hash_arr[i];
                    //std::cout << "1" << std::endl;
                    long_num u1 = Math::mod(s[i] * Math::inverse(h, q), q);
                    //std::cout << "2" << std::endl;
                    long_num u2 = Math::mod((-r + q) * Math::inverse(h, q), q); // отрицательное?
                    long_num v = Math::mod(Math::mod(Math::pow_mod(a, u1, p) * Math::pow_mod(y, u2, p), p), q);
                    //std::cout << "3" << std::endl;
                    if (v != r)
                    {
                        std::cout << "h == " << h << std::endl;
                        std::cout << "s == " << s[i] << std::endl;
                        std::cout << "u1 == " << u1 << std::endl;
                        std::cout << "u2 == " << u2 << std::endl;
                        std::cout << "v == " << v << std::endl;
                        std::cout << "r == " << r << std::endl;
                        std::cout << "a == " << a << std::endl;
                        std::cout << "y == " << y << std::endl;
                        return false;
                    }
                }

                return true;
            }
        };

    } // namespace Signature

} // namespace Safe
