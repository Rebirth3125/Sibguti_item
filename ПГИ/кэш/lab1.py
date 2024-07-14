def read_bmp_header(file_path):
    with open(file_path, 'rb') as file:
        header = file.read(54)
        return header

def read_palette(file_path):
    with open(file_path, 'rb') as file:
        file.seek(54) 
        palette = file.read(1024)  
        return palette

def convert_to_grayscale(palette):
    grayscale_palette = []
    for i in range(0, len(palette), 4):
        average_value = int((palette[i] + palette[i + 1] + palette[i + 2]) / 3)
        grayscale_palette.extend([average_value, average_value, average_value, 0])

    return bytes(grayscale_palette)

def write_grayscale_bmp(file_path, header, grayscale_palette, input_file):
    n = bytes()
    with open(input_file, 'rb') as file:
        file.seek(1078) 
        n = file.read()
    with open(file_path, 'wb') as file:
        
        file.write(header)
        file.write(grayscale_palette)
        file.write(n)

def print_bmp_info(header):
    print(f"Ширина изображения: {int.from_bytes(header[18:22], 'little')} пикселей")
    print(f"Высота изображения: {int.from_bytes(header[22:26], 'little')} пикселей")
    print(f"Глубина цвета: {int.from_bytes(header[28:30], 'little')} бит")
    print(f"Размер изображения: {int.from_bytes(header[34:38], 'little')} байт")

def main():
    input_file = "CAT256.BMP"
    output_file = "output_grayscale.bmp"
    header = read_bmp_header(input_file)
    palette = read_palette(input_file)
    print_bmp_info(header)
    grayscale_palette = convert_to_grayscale(palette)
    write_grayscale_bmp(output_file, header, grayscale_palette, input_file)
    print(f"\nПреобразованный файл сохранен как {output_file}")

if __name__ == "__main__":
    main()