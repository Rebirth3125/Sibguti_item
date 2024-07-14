from PIL import Image, ImageDraw

def convert_to_grayscale(input_file, output_file):
    with Image.open(input_file) as img:
        if img.mode != 'P':
            raise ValueError("Input image is not in indexed mode (8-bit color)")
        grayscale_palette = []
        for i in range(0, 256, 3):
            avg_intensity = sum(img.palette.palette[i:i+3]) // 3
            grayscale_palette.extend([avg_intensity, avg_intensity, avg_intensity])

        img.putpalette(grayscale_palette)
        img = img.convert('L')  
        img.save(output_file)

def print_bmp_info(input_file):
    with Image.open(input_file) as img:
        print("Характеристики BMP изображения:")
        print(f"Формат изображения: {img.format}")
        print(f"Размер изображения: {img.size}")
        print(f"Глубина цвета: {img.mode}")

input_file = "CAT256.BMP"
output_file = "output_grayscale.bmp"

convert_to_grayscale(input_file, output_file)
print_bmp_info(output_file)