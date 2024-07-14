#include <time.h>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>

#include "basicalgs.cpp"

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

void write_dec(std::string file_name, Gost_Signed_msg msg) {
    std::ofstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << file_name << std::endl;
        return;
    }

    std::vector<unsigned char> md5_hash = calculate_md5_hash(msg.msg);
    file << "MD5 Hash: ";
    for (unsigned char byte : md5_hash) {
        file << std::hex << static_cast<int>(byte);
    }
    file << '\n';

    file.close();
}


int main() 
{
    Gost_Open_keys keys;
    
    pint secret_key;
    pint secret_key_test = 234567890;

    auto msg = read_dec("image.jpg");
    Gost_Get_keys(keys, secret_key, msg);
    auto signed_msg = Gost_Sign_msg(msg, keys, secret_key);
    auto signed_msg_test = Gost_Sign_msg(msg, keys, secret_key_test);
    write_dec("ecript.txt", signed_msg);
    std::cout << Gost_Check_msg(signed_msg, keys) << "\n";
    std::cout << Gost_Check_msg(signed_msg_test, keys) << "\n";

    return 0;
}
