import cv2
import math
import numpy as np

def get_gaussian_value(x: int, y: int, sigma: float) -> float:
    e_expr = math.exp(-((x*x)+(y*y))/(2*(sigma*sigma)))

    return e_expr / (2*math.pi*(sigma*sigma))

def get_kernel_mat(radius: int, sigma: float):
    
    size = (2*radius)+1
    kernel = np.zeros((size, size), dtype=np.float32)
    sum = 0.0
    for x in range(-radius, radius+1):
        col = []
        for y in range(-radius, radius+1):
            val = get_gaussian_value(x,y, sigma)
            kernel[x + radius, y + radius] = val
            sum += val

    kernel /= sum

    return (kernel)

def create_blurred_image(radius: int,sigma: int, image: np.ndarray) -> np.ndarray:
        

        # Perform convolution using OpenCV's filter2D function
        
        kernel = get_kernel_mat(radius=radius, sigma=sigma)

        height, width, channels = image.shape

        output_image = np.zeros((height,width,channels), np.uint8)

        # Apply the kernel to each pixel except the borders
        for x in range(radius, width - radius):
            print(f"{x}/{width-radius}")
            for y in range(radius, height - radius):
                for c in range(channels):  # For each color channel (B, G, R)
                    weighted_sum = 0.0
                    for i in range(-radius, radius + 1):
                        for j in range(-radius, radius + 1):
                            # Get pixel value and kernel value
                            pixel_value = image[y + j, x + i, c]
                            kernel_value = kernel[i + radius, j + radius]
                            weighted_sum += pixel_value * kernel_value

                    output_image[y, x, c] = min(max(int(weighted_sum), 0), 255)

        cv2.imshow('img',output_image)

        cv2.waitKey(0)  
        cv2.destroyAllwindows() 

        return output_image


if __name__ == "__main__":
    # https://en.wikipedia.org/wiki/Gaussian_blur
    filepath = "../../assets/image_lvl3_2.png"
    img = cv2.imread(filepath)

    # Radius is the size of the matrix
    create_blurred_image(radius=1, sigma=1.5, image=img)
    