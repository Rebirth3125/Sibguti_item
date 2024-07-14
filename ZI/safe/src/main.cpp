#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Safe.hpp>
#include <fstream>
#include <Signature.hpp>

int main(int argc, char **argv)
{
    std::srand(std::time(nullptr));
    /*
    {

        int a = std::rand() % 10000;
        int x = std::rand() % 10000;
        int p = std::rand() % 10000;

        int answer = pow_mod(a, x, p);
        std::cout << a << "^" << x << " mod " << p << " = " << answer << std::endl;
    }
    {
        int a = 56;
        int b = 24;
        auto U = gcd(a, b);
        int x = U[1];
        int y = U[2];
        std::cout << "a = " << a << std::endl;
        std::cout << "b = " << b << std::endl;
        std::cout << "x = " << x << std::endl;
        std::cout << "y = " << y << std::endl;
        std::cout << "gcd(a, b) = " << U[0] << std::endl;
        if (a * x + b * y == U[0])
        {
            std::cout << "all good\n";
        }
    }
    {
        Diffi diff_sys;
        auto Xa = diff_sys.get_close_key();
        auto Xb = diff_sys.get_close_key();
        auto Xe = diff_sys.get_close_key();
        auto Ya = diff_sys.get_open_key(Xa);
        auto Yb = diff_sys.get_open_key(Xb);
        auto Ye = diff_sys.get_open_key(Xe);
        auto Zab = diff_sys.get_common_key(Ya, Xb);
        auto Zba = diff_sys.get_common_key(Yb, Xa);
        auto Zae = diff_sys.get_common_key(Ya, Xe);
        std::cout << "Xa = " << Xa << " Ya = " << Ya << std::endl;
        std::cout << "Xb = " << Xb << " Yb = " << Yb << std::endl;
        std::cout << "Xe = " << Xe << " Ye = " << Ye << std::endl;
        std::cout << "Zab = " << Zab << std::endl;
        std::cout << "Zba = " << Zba << std::endl;
        std::cout << "Zae = " << Zae << std::endl;
    }
    {
        long long p = get_prime();
        long long a = std::rand() % p;
        long long x = std::rand();
        long long b = pow_mod(a, x, p);
        std::cout << a << "^" << x << " mod " << p << " = " << b << std::endl;
        auto X = fast_log(a, p, b);
        if (!X.empty())
        {
            for (auto answer : X)
            {
                //std::cout << "X: " << answer << std::endl;
                if (pow_mod(a, answer, p) == b)
                {
                    std::cout << "answer = " << answer << std::endl;

                }
            }
        }
    }
    */
    while (false)
    {
        Safe::long_num p = Safe::Math::get_prime(1000);
        // p = 127;
        Safe::long_num a = Safe::Math::get_mutually_prime(p - 1, p - 1);
        // a = 17;
        auto v = Safe::Math::gcd(a, p - 1);
        Safe::long_num a_ = v[1];
        // a_ = a_ > 0 ? a_ : (-a_);
        // std::cout << "p = " << p << std::endl;
        // std::cout << "a = " << a << std::endl;
        // std::cout << "gcd(a, p) = " << v[0] << " " << v[1] << " " << v[2] << "\n";
        // std::cout << "a_ = " << a_ << std::endl;
        // std::cout << Safe::Math::pow_mod(a * a_, 1, p - 1) << std::endl;
        // std::cout << Safe::Math::mod(a * a_, p - 1) << std::endl;
    }

    while (false)
    {
        Safe::long_num p = Safe::Math::get_prime();
        std::cout << "p = " << p << std::endl;
        Safe::long_num a = Safe::Math::get_mutually_prime(p - 1, p - 1);
        std::cout << "a = " << a << std::endl;
        Safe::long_num a_ = Safe::Math::inverse(a, p - 1);
        std::cout << "a_ = " << a_ << std::endl;
        std::cout << Safe::Math::mod(a * a_, p - 1) << std::endl;
        Safe::long_num b = Safe::Math::get_mutually_prime(p - 1, p - 1);
        std::cout << "b = " << b << std::endl;
        Safe::long_num b_ = Safe::Math::inverse(b, p - 1);
        std::cout << "b_ = " << b_ << std::endl;
        std::cout << Safe::Math::mod(b * b_, p - 1) << std::endl;
        Safe::long_num m = std::rand() % 255;
        auto x1 = Safe::Math::pow_mod(m, a, p);
        auto x2 = Safe::Math::pow_mod(x1, b, p);
        auto x3 = Safe::Math::pow_mod(x2, a_, p);
        auto x4 = Safe::Math::pow_mod(x3, b_, p);
        std::cout << "m = " << m << " x4 = " << x4 << std::endl;
    }

    while (false)
    {

        Safe::long_num p = Safe::Math::get_prime();
        Safe::Encryption::Shamir clientA(p);
        Safe::Encryption::Shamir clientB(p);
        std::thread t1([&]()
                       {
            std::string message = "hello world";
            std::cout << clientA.convert(clientA.convert(message)) << std::endl;
            std::vector<Safe::long_num> x1 = Safe::Encryption::Shamir::convert(message);
            clientB << clientA.encode(x1);
            std::vector<Safe::long_num> x2;
            clientA >> x2;
            clientB << clientA.decode(x2); });
        std::thread t2([&]()
                       {
            std::vector<Safe::long_num> x1;
            clientB >> x1;
            clientA << clientB.encode(x1);
            std::vector<Safe::long_num> x2;
            clientB >> x2;
            std::string message = Safe::Encryption::Shamir::convert(clientB.decode(x2));
            std::cout << message << std::endl; });
        t1.join();
        t2.join();
    }

    while (false)
    {
        Safe::long_num p = Safe::Math::get_prime();
        Safe::Encryption::Shamir clientA(p);
        Safe::Encryption::Shamir clientB(p);
        {
            {
                std::ifstream in("shamir/input", std::ios_base::binary);
                in.unsetf(std::ios_base::skipws);
                std::ofstream out("shamir/file.in", std::ios_base::binary | std::ios_base::trunc);
                while (!in.eof())
                {
                    char c = 'a';
                    in.read(&c, 1);
                    if (in.eof())
                    {
                        break;
                    }
                    Safe::long_num o = static_cast<Safe::long_num>(c);
                    out.write(reinterpret_cast<char *>(&o), sizeof(o));
                }
            }

            std::ifstream is("shamir/file.in", std::ios_base::binary);
            // is.unsetf(std::ios_base::skipws);
            std::ofstream os("shamir/file.out", std::ios_base::binary | std::ios_base::trunc);
            // os.unsetf(std::ios_base::skipws);
            clientA.encode(is, os);
        }
        {
            std::ifstream is("shamir/file.out", std::ios_base::binary);
            // is.unsetf(std::ios_base::skipws);
            std::ofstream os("shamir/encoded.txt", std::ios_base::binary | std::ios_base::trunc);
            // os.unsetf(std::ios_base::skipws);
            clientB.encode(is, os);
        }
        {
            std::ifstream is("shamir/encoded.txt", std::ios_base::binary);
            // is.unsetf(std::ios_base::skipws);
            std::ofstream os("shamir/encoded.out", std::ios_base::binary | std::ios_base::trunc);
            // os.unsetf(std::ios_base::skipws);
            clientA.decode(is, os);
        }
        {
            std::ifstream is("shamir/encoded.out", std::ios_base::binary);
            // is.unsetf(std::ios_base::skipws);
            std::ofstream os("shamir/decoded.in", std::ios_base::binary | std::ios_base::trunc);
            // os.unsetf(std::ios_base::skipws);
            clientB.decode(is, os);
        }

        {
            std::ifstream in("shamir/decoded.in", std::ios_base::binary);
            // in.unsetf(std::ios_base::skipws);
            std::ofstream out("shamir/decoded.jpg", std::ios_base::binary | std::ios_base::trunc);
            while (!in.eof())
            {
                Safe::long_num c;
                in.read(reinterpret_cast<char *>(&c), sizeof(c));
                if (in.eof())
                {
                    break;
                }
                char o = static_cast<char>(c);
                out.write(&o, 1);
            }
        }
    }

    while (false)
    {
        Safe::long_num p = Safe::Math::get_prime();
        Safe::Encryption::LGamal clientA(p);
        Safe::Encryption::LGamal clientB(p);
        std::ifstream in("gamal/gamal.in", std::ios_base::binary);
        std::ofstream out("gamal/gamal.out", std::ios_base::binary | std::ios_base::trunc);
        {
            std::ofstream encoded("gamal/gamal.encoded", std::ios_base::binary | std::ios_base::trunc);
            std::cout << "start encoding\n";
            clientA.encode(in, encoded, clientB.get_open_key());
            std::cout << "encoded\n";
        }
        {
            std::ifstream encoded("gamal/gamal.encoded", std::ios_base::binary);
            clientB.decode(encoded, out);
        }

        // std::string message = "bye world!";
        // auto open_key = clientB.get_open_key();
        // auto vec = clientA.convert(message);
        // auto encoded = clientA.encode(vec, open_key);
        // auto decoded = clientB.decode(encoded);
        // auto converted = clientB.convert(decoded);
        // std::cout << converted << std::endl;
    }

    while (false)
    {
        std::string message = "vernam code";
        auto key = Safe::Encryption::Vernam::generate(message.length());
        auto encoded = Safe::Encryption::Vernam::encode(message, key);
        auto decoded = Safe::Encryption::Vernam::decode(encoded, key);
        std::cout << decoded << std::endl;
    }

    while (false)
    {

        Safe::long_num l = 10;
        {
            std::ifstream in("vernam/vernam.in", std::ios_base::binary | std::ios::ate);
            l = in.tellg();
        }
        {
            std::ofstream key("vernam/vernam.key", std::ios_base::binary | std::ios_base::trunc);
            Safe::Encryption::Vernam::generate(key, l);
        }
        {
            std::ifstream in("vernam/vernam.in", std::ios_base::binary);
            std::ofstream encoded("vernam/vernam.encoded", std::ios_base::binary | std::ios_base::trunc);
            std::ifstream key("vernam/vernam.key", std::ios_base::binary);
            std::cout << "vernam encoding\n";
            Safe::Encryption::Vernam::encode(in, encoded, key);
        }
        {
            std::ifstream encoded("vernam/vernam.encoded", std::ios_base::binary);
            std::ofstream out("vernam/vernam.out", std::ios_base::binary | std::ios_base::trunc);
            std::ifstream key("vernam/vernam.key", std::ios_base::binary);
            std::cout << "vernam decoding\n";
            Safe::Encryption::Vernam::decode(encoded, out, key);
        }
    }

    while (false)
    {
        std::string message = "this is rsa cypher";
        Safe::Encryption::RSA Alice{};
        Safe::Encryption::RSA Bob{};
        auto encoded = Alice.encode(message, Bob.get_open_key());
        auto decoded = Bob.decode(encoded);
        std::cout << decoded << std::endl;
    }

    while (false)
    {
        Safe::Encryption::RSA Alice{};
        Safe::Encryption::RSA Bob{};
        {
            std::ifstream in("rsa/rsa.in", std::ios_base::binary);
            std::ofstream encoded("rsa/rsa.encoded", std::ios_base::binary | std::ios_base::trunc);
            Alice.encode(in, encoded, Bob.get_open_key());
        }
        {
            std::ifstream encoded("rsa/rsa.encoded", std::ios_base::binary);
            std::ofstream out("rsa/rsa.out", std::ios_base::binary | std::ios_base::trunc);
            Bob.decode(encoded, out);
        }
    }

    //while (false)
    {
        Safe::long_num p = Safe::Math::get_prime(10e9);
        std::cout << "P = " << p << std::endl;
        Safe::Signature::LGamal Alice(p);
        const std::string filename = "signature/gamal/doc.txt";
        Alice.subscribe(filename);
        Alice.subscribe_in(filename);
        Safe::Signature::LGamal Bob(p);
        if (Bob.check(Alice, filename, filename + ".sig"))
        {
            std::cout << "LGamal signature is good\n";
        }
        else
        {
            std::cout << "LGamal signature is bad\n";
        }
        if (Bob.check(Alice, filename + ".sgn"))
        {
            std::cout << "LGamal inner signature is good\n";
        }
        else
        {
            std::cout << "LGamal inner signature is bad\n";
        }
    }

    //while (false)
    {
        Safe::Signature::RSA Alice;
        Safe::Signature::RSA Bob;
        const std::string filename = "signature/RSA/doc.txt";
        Alice.subscribe(filename);
        if (Bob.check(Alice, filename, filename + ".sig"))
        {
            std::cout << "RSA signature is good\n";
        }
        else
        {
            std::cout << "RSA signature is bad\n";
        }
        Alice.subscribe_in(filename);
        if (Bob.check(Alice, filename + ".sgn"))
        {
            std::cout << "RSA inner signature is good\n";
        }
        else
        {
            std::cout << "RSA inner signature is bad\n";
        }
    }

    {
        Safe::Signature::GOST ghost;
        std::cout << "ghost created\n";
        Safe::long_num A = ghost.get_close_key();
        Safe::long_num B = ghost.get_close_key();
        const std::string filename = "signature/GOST/doc.txt";
        std::vector<char> message = Safe::Signature::read(filename);
        std::cout << "message readed\n";
        {
            auto sign = ghost.subscribe(message, A);
            std::cout << "ghost subscribed\n";
            ghost.write(filename + ".sig", sign);
            ghost.write(filename + ".sgn", sign, message);
        }

        {
            std::ifstream f(filename + ".sig");
            auto sign = ghost.read(f);
            if (ghost.check(message, ghost.get_open_key(A), sign))
            {
                std::cout << "GHOST signature is ghood\n";
            }
            else
            {
                std::cout << "GHOST signature is bad\n";
            }
        }
        {
            std::ifstream f(filename + ".sgn");
            auto sign = ghost.read(f);
            std::vector<char> message = Safe::Signature::read(f);
            if (ghost.check(message, ghost.get_open_key(A), sign))
            {
                std::cout << "GHOST inner signature is ghood\n";
            }
            else
            {
                std::cout << "GHOST inner signature is bad\n";
            }
        }
    }

    return 0;
}