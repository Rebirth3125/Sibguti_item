#include <iostream>
#include <fstream>
#include <vector>

void read_bmp_header(const std::string& file_path, std::vector<char>& header) {
    std::ifstream file(file_path, std::ios::binary);
    file.read(header.data(), 54);
}

void read_palette(const std::string& file_path, std::vector<char>& palette) {
    std::ifstream file(file_path, std::ios::binary);
    file.seekg(54);
    file.read(palette.data(), 1024);
}

std::vector<char> convert_to_grayscale(const std::vector<char>& palette) {
    std::vector<char> grayscale_palette;
    for (size_t i = 0; i < palette.size(); i += 4) {
        char average_value = static_cast<char>((static_cast<unsigned char>(palette[i]) +
                                               static_cast<unsigned char>(palette[i + 1]) +
                                               static_cast<unsigned char>(palette[i + 2])) / 3);
        grayscale_palette.insert(grayscale_palette.end(), {average_value, average_value, average_value, 0}); 
    }
    return grayscale_palette;
}

void write_grayscale_bmp(const std::string& file_path, const std::vector<char>& header, const std::vector<char>& grayscale_palette, const std::string& input_file) {
    std::ifstream file(input_file, std::ios::binary);
    file.seekg(1078);
    std::vector<char> n((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::ofstream output_file(file_path, std::ios::binary);
    output_file.write(header.data(), header.size());
    output_file.write(grayscale_palette.data(), grayscale_palette.size());
    output_file.write(n.data(), n.size());
}

void print_bmp_info(const std::vector<char>& header) {
    std::cout << "Ширина изображения: " << *reinterpret_cast<const int*>(&header[18]) << " пикселей" << std::endl;
    std::cout << "Высота изображения: " << *reinterpret_cast<const int*>(&header[22]) << " пикселей" << std::endl;
    std::cout << "Глубина цвета: " << *reinterpret_cast<const short*>(&header[28]) << " бит" << std::endl;
    std::cout << "Размер изображения: " << *reinterpret_cast<const int*>(&header[34]) << " байт" << std::endl;
}

int main() {
    const std::string input_file = "CAT256.BMP";
    const std::string output_file = "output_grayscale.bmp";

    std::vector<char> header(54);
    read_bmp_header(input_file, header);

    std::vector<char> palette(1024);
    read_palette(input_file, palette);

    print_bmp_info(header);

    std::vector<char> grayscale_palette = convert_to_grayscale(palette);

    write_grayscale_bmp(output_file, header, grayscale_palette, input_file);

    std::cout << "\nПреобразованный файл сохранен как " << output_file << std::endl;

    return 0;
}