def rotate_bmp_90(input_file, output_file):
    with open(input_file, "rb") as f:
        data = f.read()

    width = int.from_bytes(data[18:22], byteorder='little')
    height = int.from_bytes(data[22:26], byteorder='little')

    data = bytearray(data)
    data[18:22] = height.to_bytes(4, byteorder='little')
    data[22:26] = width.to_bytes(4, byteorder='little')

    pixel_data_offset = int.from_bytes(data[10:14], byteorder='little')

    new_pixel_data = bytearray(width * height * 3)

    for y in range(height):
        for x in range(width):
            new_x = y
            new_y = width - x - 1
            old_index = (y * width + x) * 3
            new_index = (new_y * height + new_x) * 3
            new_pixel_data[new_index:new_index+3] = data[pixel_data_offset+old_index:pixel_data_offset+old_index+3]

    data[pixel_data_offset:pixel_data_offset+len(new_pixel_data)] = new_pixel_data

    with open(output_file, "wb") as f:
        f.write(data)

input_file = "_сarib_TC.bmp"
output_file = "rotated_output.bmp"
rotate_bmp_90(input_file, output_file)