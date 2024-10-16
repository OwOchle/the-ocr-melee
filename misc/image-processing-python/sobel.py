import numpy as np
import math
from typing import Tuple

def get_sobel_operator() -> Tuple[:]:
    x_mat = np.array([[-1, 0, 1],
                     [-2, 0, 2],
                     [-1, 0, 1]])
    y_mat = np.array([[-1, -2, -1],
                     [0, 0, 0],
                     [1, 2, 1]])
    
    return (x_mat, y_mat)

def apply_sobel(image: np.ndarray, radius: int = 1) -> np.ndarray:        
        x_sobel, y_sobel = get_sobel_operator()

        height, width, channels = image.shape

        output_image = np.zeros((height,width,channels), np.uint8)

        for x in range(radius, width - radius):
            print(f"{x}/{width-radius}")
            for y in range(radius, height - radius):
                for c in range(channels):  
                    gx = 0.0
                    gy = 0.0
                    for i in range(-radius, radius + 1):
                        for j in range(-radius, radius + 1):
                            pixel_value = image[y + j, x + i, c]
                            gx += pixel_value * x_sobel[i + radius, j + radius]
                            gy += pixel_value * y_sobel[i + radius, j + radius]

                    weighted_sum = np.sqrt(gx ** 2 + gy ** 2)
                    output_image[y, x, c] = min(max(int(weighted_sum), 0), 255)

        return output_image
