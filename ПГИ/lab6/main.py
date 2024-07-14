from PIL import Image
import os
import numpy as np

if __name__ == "__main__":
    path_ = os.path.dirname(__file__)
    filename = "_сarib_TC"
    ext = "bmp"

    if not os.path.exists(f"{path_}//output"):
        os.mkdir(f"{path_}//output")

    img = Image.open(f"{path_}//input//{filename}.{ext}").convert("RGBA")  
    img_matrix = np.array(img.convert("RGB"))

    logo_filename = "logo"
    logo_ext = "png"

    logo = Image.open(f"{path_}//input//{logo_filename}.{logo_ext}").convert("RGBA")  
    logo_matrix = np.array(logo.convert("RGB"))

    assert logo.height < img.height and logo.width < img.width

    k = 0.5
    
    for row in range(logo.height):
        for col in range(logo.width):
            if not np.array_equal(logo_matrix[row, col][:3], np.array([255, 176, 203])):
                r, g, b = [int(img_matrix[row][col][i] * k + logo_matrix[row][col][i] * (1 - k)) for i in range(3)]

                img_matrix[row][col] = r, g, b

    new_img = Image.fromarray(img_matrix, 'RGB')
    new_img.save(f"{path_}//output//{filename}_watermark.{ext}", "BMP")

    new_img.show()
