
#include <openssl/pem.h>
#include <openssl/md5.h>
#include <fstream>
#include <vector>
#include <iostream>

// Глобальные переменные для длины ключа и файлов
const int RSA_KEY_LENGTH = 2048;
const char* PUBLIC_KEY_FILE = "public_key.pem";
const char* PRIVATE_KEY_FILE = "private_key.pem";

// Функция для создания пары ключей и сохранения их в файлы
bool GenerateKeyPair() {
    RSA *rsa = RSA_generate_key(RSA_KEY_LENGTH, RSA_F4, nullptr, nullptr);
    if (rsa == nullptr) {
        std::cerr << "Ошибка генерации ключей." << std::endl;
        return false;
    }

    FILE *privateFile = fopen(PRIVATE_KEY_FILE, "wb");
    if (privateFile == nullptr) {
        std::cerr << "Ошибка открытия файла для записи секретного ключа." << std::endl;
        RSA_free(rsa);
        return false;
    }

    FILE *publicFile = fopen(PUBLIC_KEY_FILE, "wb");
    if (publicFile == nullptr) {
        std::cerr << "Ошибка открытия файла для записи открытого ключа." << std::endl;
        RSA_free(rsa);
        fclose(privateFile);
        return false;
    }

    if (!PEM_write_RSAPrivateKey(privateFile, rsa, nullptr, nullptr, 0, nullptr, nullptr)) {
        std::cerr << "Ошибка записи секретного ключа в файл." << std::endl;
        RSA_free(rsa);
        fclose(privateFile);
        fclose(publicFile);
        return false;
    }

    if (!PEM_write_RSAPublicKey(publicFile, rsa)) {
        std::cerr << "Ошибка записи открытого ключа в файл." << std::endl;
        RSA_free(rsa);
        fclose(privateFile);
        fclose(publicFile);
        return false;
    }

    RSA_free(rsa);
    fclose(privateFile);
    fclose(publicFile);

    return true;
}

// Функция для подписи файла
bool SignFile(const char* inputFileName, const char* outputFileName) {
    FILE *privateFile = fopen(PRIVATE_KEY_FILE, "rb");
    if (privateFile == nullptr) {
        std::cerr << "Ошибка открытия файла с секретным ключом." << std::endl;
        return false;
    }

    RSA* rsa = PEM_read_RSAPrivateKey(privateFile, nullptr, nullptr, nullptr);
    if (rsa == nullptr) {
        std::cerr << "Ошибка чтения секретного ключа." << std::endl;
        fclose(privateFile);
        return false;
    }

    fclose(privateFile);

    std::ifstream inputFile(inputFileName, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Ошибка открытия файла для подписи." << std::endl;
        RSA_free(rsa);
        return false;
    }

    // Чтение и подписание файла
    inputFile.seekg(0, std::ios::end);
    std::streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::vector<char> fileData(fileSize);
    inputFile.read(fileData.data(), fileSize);
    inputFile.close();

    unsigned char md5_hash[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(fileData.data()), fileSize, md5_hash);

    std::vector<unsigned char> signature(RSA_size(rsa));
    unsigned int signature_length;
    int result = RSA_sign(NID_md5, md5_hash, MD5_DIGEST_LENGTH, signature.data(), &signature_length, rsa);

    if (result != 1) {
        std::cerr << "Ошибка подписи файла." << std::endl;
        RSA_free(rsa);
        return false;
    }

    std::ofstream outputFile(outputFileName, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Ошибка открытия файла для записи подписи." << std::endl;
        RSA_free(rsa);
        return false;
    }

    outputFile.write(reinterpret_cast<char*>(signature.data()), signature_length);
    RSA_free(rsa);
    return true;
}

// Функция для верификации подписи файла
bool VerifySignature(const char* inputFileName, const char* signatureFileName) {
    FILE *publicFile = fopen(PUBLIC_KEY_FILE, "rb");


    RSA* rsa = PEM_read_RSAPublicKey(publicFile, nullptr, nullptr, nullptr);

    fclose(publicFile);


    inputFile.seekg(0, std::ios::end);
    std::streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::vector<char> fileData(fileSize);
    inputFile.read(fileData.data(), fileSize);
    inputFile.close();

    unsigned char md5_hash[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(fileData.data()), fileSize, md5_hash);


    signatureFile.seekg(0, std::ios::end);
    std::streampos signatureSize = signatureFile.tellg();
    signatureFile.seekg(0, std::ios::beg);

    std::vector<unsigned char> signature(signatureSize);
    signatureFile.read(reinterpret_cast<char*>(signature.data()), signatureSize);
    signatureFile.close();

    int result = RSA_verify(NID_md5, md5_hash, MD5_DIGEST_LENGTH, signature.data(), signatureSize, rsa);
    RSA_free(rsa);

    return result == 1;
}

int main() {
    // Генерация пары ключей (выполняется один раз)
    if (!GenerateKeyPair()) {
        return 1;
    }

    // Подписание файла
    const char* inputFileName = "image.jpg";
    const char* signatureFileName = "file_signature.bin";

    if (!SignFile(inputFileName, signatureFileName)) {
        return 2;
    }

    // Верификация подписи файла
    if (VerifySignature(inputFileName, signatureFileName)) {
        std::cout << "Подпись верифицирована." << std::endl;
    } else {
        std::cerr << "Ошибка верификации подписи." << std::endl;
    }

    return 0;
}
