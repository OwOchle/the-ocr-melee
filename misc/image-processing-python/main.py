import os
import cv2
import numpy as np
import math

from matplotlib import pyplot as plt
from pathlib import Path
import typing as tp


from gaussian import create_blurred_image
from sobel import apply_sobel
from grayscale import apply_grayscale
from hough import hough_lines

def load_image(filename : str, filepath : str = "../../assets/"):
    img_path = os.path.join(filepath, filename)
    img = cv2.imread(img_path)

    return img

if __name__ == "__main__":
    # https://en.wikipedia.org/wiki/Gaussian_blur
    filepath = "../../assets/"
    filename = "image_lvl2_1_res2.png"

    img = load_image(filename=filename)

    # Radius is the size of the matrix
    output_img = create_blurred_image(radius=3, sigma=1.5, image=img)
    cv2.imwrite(filename=f"{filepath}output_grad_{filename}", img=output_img)
    
    
    img = load_image(filename=f"output_grad_{filename}")
    gray_out = apply_grayscale(img)
    sobel_out = apply_sobel(gray_out)

    cv2.imwrite(filename=f"{filepath}output_sobel_{filename}", img=sobel_out)

    polar_lines = hough_lines(sobel_out, sobel_out, threshold=0, min_theta=1, max_theta=np.pi)


    cv2.imwrite(filename=f"{filepath}output_hough_{filename}", img=polar_lines)

    
    