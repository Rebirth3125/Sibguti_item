#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#pragma pack(push, 1) 

struct BMPHeader {
    char signature[2];
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};

#pragma pack(pop) 

void addRandomFrame(const char* inputFileName, const char* outputFileName) {
    std::ifstream inputFile(inputFileName, std::ios::binary);
    std::ofstream outputFile(outputFileName, std::ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return;
    }

    BMPHeader header;
    inputFile.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

    header.width += 30;
    header.height += 30;

    header.fileSize += 30 * header.width * 3;

    outputFile.write(reinterpret_cast<const char*>(&header), sizeof(BMPHeader));

    srand(time(nullptr));
    for (int y = 0; y < header.height; ++y) {
        for (int x = 0; x < header.width; ++x) {
            if (x < 15 || x >= header.width - 15 || y < 15 || y >= header.height - 15) {
                uint8_t color[3] = {static_cast<uint8_t>(rand() % 256),
                                    static_cast<uint8_t>(rand() % 256),
                                    static_cast<uint8_t>(rand() % 256)};
                outputFile.write(reinterpret_cast<const char*>(&color), sizeof(color));
            } else {
                uint8_t pixel[3];
                inputFile.read(reinterpret_cast<char*>(&pixel), sizeof(pixel));
                outputFile.write(reinterpret_cast<const char*>(&pixel), sizeof(pixel));
            }
        }

        for (int i = 0; i < (header.width * 3) % 4; ++i) {
            uint8_t paddingByte = 0;
            outputFile.write(reinterpret_cast<const char*>(&paddingByte), sizeof(paddingByte));
        }
    }

    inputFile.close();
    outputFile.close();
}

int main() {
    const char* inputFileName = "_сarib_TC.bmp";
    const char* outputFileName = "output.bmp";

    addRandomFrame(inputFileName, outputFileName);

    std::cout << "Frame added successfully. Output file: " << outputFileName << std::endl;

    return 0;
}