#include "basicalgs.cpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

void write_signature(const std::string& file_name, const std::vector<unsigned char>& signature) {
    std::ofstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << file_name << std::endl;
        return;
    }

    for (unsigned char byte : signature) {
        file << std::hex << static_cast<int>(byte);
    }

    file.close();
}

int main() {
    Elgamal_Open_key ok1; // Открытый ключ для проверки подписи
    ok1.g = 356; // Установите нужные значения для открытого ключа
    ok1.p = 1464;
    ok1.y = 58;

    // Чтение содержимого файла "ecript.txt"
    std::ifstream file("ecript.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file ecript.txt" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<unsigned char> md5_hash;
    std::vector<unsigned char> signature;

    while (std::getline(file, line)) {
        if (line.find("MD5 Hash:") != std::string::npos) {
            // Извлечь MD5 хеш из строки
            std::string md5_str = line.substr(line.find("MD5 Hash:") + 10);
            // Преобразовать MD5 хеш в байты
            for (size_t i = 0; i < md5_str.length(); i += 2) {
                unsigned char byte = std::stoi(md5_str.substr(i, 2), nullptr, 16);
                md5_hash.push_back(byte);
            }
        } else {
            // Извлечь подпись из строки
            for (size_t i = 0; i < line.length(); i += 2) {
                unsigned char byte = std::stoi(line.substr(i, 2), nullptr, 16);
                signature.push_back(byte);
            }
        }
    }

    file.close();

    // Проверить подпись
    Elgamal_Signed_msg signed_msg;
    signed_msg.msg = md5_hash; // Используем MD5 хеш для проверки

    bool signature_valid = Elgamal_Check_msg(signed_msg, ok1, signature);

    if (signature_valid) {
        std::cout << "Signature is valid." << std::endl;
    } else {
        std::cout << "Signature is not valid." << std::endl;
    }

    return 0;
}