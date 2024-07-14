import numpy as np
from PIL import Image

def rgb_delta(color1, color2):
    r1, g1, b1 = color1
    r2, g2, b2 = color2
    delta = (r1 - r2) ** 2 + (g1 - g2) ** 2 + (b1 - b2) ** 2
    return delta

def find_similar_color(color, palette):
    min_delta = float('inf')
    closest_color = None
    for palette_color in palette:
        delta = rgb_delta(color, palette_color)
        if delta < min_delta:
            min_delta = delta
            closest_color = palette_color
    return closest_color

def reduce_colors(image, num_colors):
    image_rgb = image.convert('RGB')
    color_freq = image_rgb.getcolors()
    sorted_colors = sorted(color_freq, key=lambda x: x[0], reverse=True)
    most_frequent_colors = [color for count, color in sorted_colors[:num_colors]]
    palette = most_frequent_colors
    reduced_image = Image.new('RGB', image_rgb.size)
    pixels = reduced_image.load()
    for y in range(image_rgb.size[1]):
        for x in range(image_rgb.size[0]):
            pixel_color = image_rgb.getpixel((x, y))
            closest_color = find_similar_color(pixel_color, palette)
            pixels[x, y] = closest_color
    return reduced_image

input_image = Image.open("CAT256.PCX")
output_image = reduce_colors(input_image, 16)
output_image.save("CAT_256_LESS_COLOR.PCX")

from PIL import Image, ImageTk
import tkinter as tk

def display_images(image1_path, image2_path):
    image1 = Image.open(image1_path)
    image2 = Image.open(image2_path)

    root = tk.Tk()
    root.title("PCX Images")

    tk_image1 = ImageTk.PhotoImage(image1)
    tk_image2 = ImageTk.PhotoImage(image2)

    canvas1 = tk.Canvas(root, width=image1.width, height=image1.height)
    canvas1.pack(side=tk.LEFT)
    canvas1.create_image(0, 0, anchor=tk.NW, image=tk_image1)

    canvas2 = tk.Canvas(root, width=image2.width, height=image2.height)
    canvas2.pack(side=tk.RIGHT)
    canvas2.create_image(0, 0, anchor=tk.NW, image=tk_image2)

    root.mainloop()

image1_path = "CAT256.PCX"
image2_path = "CAT_256_LESS_COLOR.PCX"
display_images(image1_path, image2_path)
