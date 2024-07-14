from PIL import Image
import os
import numpy as np
from sklearn.cluster import KMeans
import imageio
import cv2

def read_bmp(file_path):
    bmp_image = Image.open(file_path)
    return bmp_image

def reduce_colors(image, num_colors):
    pixels = np.array(image)
    all_pixels = pixels.reshape(-1, pixels.shape[-1])
    kmeans = KMeans(n_clusters=num_colors, n_init=10)
    kmeans.fit(all_pixels)

    new_colors = kmeans.cluster_centers_.astype(int)
    new_pixels = np.array([new_colors[label] for label in kmeans.labels_])
    new_image_array = new_pixels.reshape(pixels.shape)

    new_image = Image.fromarray(new_image_array.astype('uint8'), 'RGB').convert('P', palette=Image.ADAPTIVE)

    return new_image

def convert_bmp_to_pcx(bmp_path, pcx_path, num_colors=256):
    if os.path.exists(pcx_path):
        converted_image = Image.open(pcx_path)
        converted_image.show()
        print("Файл PCX уже существует, открыт.")
        return
    
    image = read_bmp(bmp_path)
    reduced_image = reduce_colors(image, num_colors)

    if image.size != reduced_image.size:
        raise ValueError("Ошибка в размерах изображения после уменьшения цветов")

    reduced_image.save(pcx_path, format='PCX')
    image.show()
    reduced_image.show()

    converted_image = Image.open(pcx_path)
    converted_image.show()

bmp_path = '_сarib_TC.bmp'
pcx_path = 'fish_256_color.pcx'

convert_bmp_to_pcx(bmp_path, pcx_path)
