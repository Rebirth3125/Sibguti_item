#include "basicalgs.cpp"

#include <vector>
#include <iostream>

std::vector<char> read_dec(std::string file_name)
{
    std::ifstream fin(file_name);
	char ch;
    std::vector<char> x;
    while (fin.get(ch))
    {
        x.push_back(ch);
    }
	fin.close();
	return x;
}

// int isPrime(int n)
// {
//     if (n <= 1)
//     {
//         return 0;
//     }

//     for (int i = 2; i < sqrt(n); i++)
//     {
//         if (n % i == 0)
//         {
//             return 0;
//         }
//     }

//     return 1;
// }

// long long int generate_prime_numbers(long long int& c, long long int& d, long long int p)
// {
//     c = rand() % p;

//     while (!isPrime(c))
//     {
//         c = rand() % p;
//     }

//     long int gcd_x, gcd_y;

//     while (c < p)
//     {
//         if (gcd(c, p, &gcd_x, &gcd_y) == 1)
//         {
//             d = gcd_x;

//             if ((c * d) % p == 1)
//             {
//                 return 1;
//             }
//         }

//         c++;
//     }

//     return 0;
// }

// int gcd(long a, long b, long* x, long* y)
// {
//     if (a == 0)
//     {
//         *x = 0;
//         *y = 1;
//         return b;
//     }

//     long x1, y1;
//     long d = gcd(b % a, a, &x1, &y1);

//     *x = y1 - (b / a) * x1;
//     *y = x1;

//     return d;
// }

// std::vector<unsigned char> calculate_md5_hash(const std::vector<char>& msg) {
//     unsigned char digest[MD5_DIGEST_LENGTH];
//     MD5(reinterpret_cast<const unsigned char*>(msg.data()), msg.size(), digest);
//     return std::vector<unsigned char>(digest, digest + MD5_DIGEST_LENGTH);
// }

void write_dec(std::string file_name, RSA_Signed_msg msg) {
    std::ofstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << file_name << std::endl;
        return;
    }

    std::vector<unsigned char> md5_hash = calculate_md5_hash(msg.data);
    upint md5_hash_value = 0;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        md5_hash_value = md5_hash_value * 256 + static_cast<upint>(md5_hash[i]);
    }

    file << "MD5 Hash: " << md5_hash_value << '\n';

    file.close();
}


int main() {
    srand(static_cast<unsigned>(time(0)));;

    RSA_Open_key open_key, open_key2;
    RSA_Secret_key secret_key;
    
    RSA_Get_keys(open_key, secret_key);
    auto msg = read_dec("image.jpg");
    RSA_Signed_msg signed_msg = RSA_Sign_msg(msg, open_key, secret_key);
    std::cout << "1) " << RSA_Check_msg(signed_msg, open_key) << "\n";
    std::cout << "2) " << RSA_Check_msg(signed_msg, open_key2) << "\n";

    // Проверка подписи
    // if (RSA_Check_msg(signed_msg, open_key))
    // {
    //     std::cout << "да\n";
    // }
    // else
    // {
    //     std::cout << "нет\n";
    // }
    return 0;
}
