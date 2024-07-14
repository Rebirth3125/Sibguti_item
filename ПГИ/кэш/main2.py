import struct
import random

def generate_random_color():
    return random.randint(0, (1 << 24) - 1) | 0xFF000000 

input_file_name = "_сarib_TC.bmp"
output_file_name = "output_with_border.bmp"

with open(input_file_name, "rb") as input_file:
    bmp_header = struct.unpack("<2sIHHI", input_file.read(14))
    dib_header = struct.unpack("<IiiHHIIiiII", input_file.read(40))

    if bmp_header[0] != b'BM' or dib_header[4] != 24 or dib_header[5] != 0:
        print("Invalid BMP file format or unsupported color depth/compression method.")
        exit(1)

    border_colors = [generate_random_color() for _ in range(256)]

    with open(output_file_name, "wb") as output_file:
        new_width = dib_header[1] + 30
        new_height = dib_header[2] + 30

        dib_header = list(dib_header)
        dib_header[1] = new_width
        dib_header[2] = new_height

        dib_header[6] = (new_width * 3 + (new_width * 3) % 4) * new_height 
        bmp_header = list(bmp_header)
        bmp_header[2] = bmp_header[4] + dib_header[6]

        output_file.write(struct.pack("<2sIHHI", *bmp_header))
        output_file.write(struct.pack("<IiiHHIIiiII", *dib_header))

        for y in range(new_height):
            for x in range(new_width):
                if x < 15 or x >= new_width - 15 or y < 15 or y >= new_height - 15:
                    output_file.write(struct.pack("<I", border_colors[random.randint(0, 255)])[:3])
                else:
                    pixel = input_file.read(3)
                    output_file.write(pixel)

            output_file.write(b'\x00' * ((4 - (new_width * 3) % 4) % 4))

print("Border added successfully.")
