#include "Lab1/Lab1algs.hpp"

int main() {
    ElgamalKeys keys;
    ElgamalInit(keys);
    std::string enc_file = ElgamalEncrypt("image.png", keys);
    ElgamalDecrypt(enc_file, keys);
    return 0;
}