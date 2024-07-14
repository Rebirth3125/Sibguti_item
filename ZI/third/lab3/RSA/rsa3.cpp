#include <iostream>
#include <vector>
#include <openssl/bn.h>
#include <openssl/sha.h>

// Глобальные переменные для длины ключа и открытого ключа
const int RSA_KEY_LENGTH = 2048;

// Функция для генерации случайного большого простого числа (для упрощения, используется случайное значение)
BIGNUM* generatePrime() {
    BIGNUM* prime = BN_new();
    BN_generate_prime_ex(prime, RSA_KEY_LENGTH, 1, NULL, NULL, NULL);
    return prime;
}

// Функция для создания ключей RSA
void generateRSAKeyPair(BIGNUM** publicKey, BIGNUM** privateKey) {
    BIGNUM* p = generatePrime();
    BIGNUM* q = generatePrime();

    *publicKey = BN_new();
    *privateKey = BN_new();

    // Вычисляем n = p * q
    BN_mul(*publicKey, p, q, NULL);

    // Вычисляем ф-ксию Эйлера (phi(n) = (p-1) * (q-1))
    BIGNUM* phi = BN_new();
    BN_sub(p, p, BN_value_one());
    BN_sub(q, q, BN_value_one());
    BN_mul(phi, p, q, NULL);

    // Генерация открытого ключа (e) - публичная экспонента (стандартное значение)
    BIGNUM* e = BN_new();
    BN_set_word(e, 65537);  // Обычно используется стандартное значение 65537

    // Вычисление закрытого ключа (d) - секретная экспонента
    BN_mod_inverse(*privateKey, e, phi, NULL);

    // Освобождение ресурсов
    BN_free(p);
    BN_free(q);
    BN_free(phi);
    BN_free(e);
}

// Функция для подписи сообщения
std::vector<unsigned char> signMessage(const std::string& message, BIGNUM* privateKey) {
    const char* msg = message.c_str();
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)msg, message.length(), hash);

    BIGNUM* signature = BN_new();
    BN_bin2bn(hash, SHA256_DIGEST_LENGTH, signature);

    BIGNUM* result = BN_new();
    BN_mod_exp(result, signature, privateKey, privateKey, NULL);

    int size = BN_num_bytes(result);
    std::vector<unsigned char> signatureData(size);
    BN_bn2bin(result, signatureData.data());

    BN_free(signature);
    BN_free(result);

    return signatureData;
}

int main() {
    BIGNUM* publicKey;
    BIGNUM* privateKey;

    // Генерация ключей
    generateRSAKeyPair(&publicKey, &privateKey);

    // Пример сообщения для подписи
    std::string message = "Hello, world!";

    // Создание подписи
    std::vector<unsigned char> signature = signMessage(message, privateKey);

    // Вывод подписи
    std::cout << "Signature: ";
    for (const unsigned char& byte : signature) {
        std::cout << std::hex << (int)byte;
    }
    std::cout << std::endl;

    // Освобождение ресурсов
    BN_free(publicKey);
    BN_free(privateKey);

    return 0;
}
