import os
import cv2
import numpy as np


from pre_treatement.gaussian import create_blurred_image
from pre_treatement.sobel import apply_sobel
from pre_treatement.grayscale import apply_grayscale
from algos.hough import hough_lines
from algos.line_detection import detect_grid_lines_x, detect_grid_lines_y

def load_image(filename : str, filepath : str = "../../assets/"):
    img_path = os.path.join(filepath, filename)
    img = cv2.imread(img_path)

    return img

if __name__ == "__main__":
    # https://en.wikipedia.org/wiki/Gaussian_blur
    filepath = "../../assets/"
    output_folder = "outputs/"
    filename = "image_lvl4_1.png"

    img = load_image(filename=filename)
    
    
    
    # Radius is the size of the matrix
    output_img = create_blurred_image(radius=2, sigma=1.5, image=img)
    cv2.imwrite(filename=f"{output_folder}output_grad_{filename}", img=output_img)
    
    
    img = load_image(filename=f"output_grad_{filename}", filepath=output_folder)
    

    
    gray_out = apply_grayscale(img) # Apply grayscale to the image. 
    sobel_out = apply_sobel(gray_out) # Apply the sobel operator to the grayscaled image.

    cv2.imwrite(filename=f"{output_folder}output_sobel_{filename}", img=sobel_out) 
    

    # Hough lines; Will prob not use it.
    polar_lines = hough_lines(sobel_out, sobel_out, threshold=0, min_theta=1, max_theta=np.pi)
    cv2.imwrite(filename=f"{output_folder}output_hough_{filename}", img=polar_lines)
    polar_lines = load_image(filename=f"output_hough_{filename}", filepath=output_folder)
    
    # Custom line detection algo. Takes the sobel and goes line by line to detect which lines have more than average number of white pixels.
    polar_lines_target = load_image(filename=f"output_sobel_{filename}", filepath=output_folder)
    lines = detect_grid_lines_y(polar_lines_target, target=img, line_width=5)
    # lines = detect_grid_lines_x(polar_lines_target, target=lines,  line_width=3)
    
    cv2.imwrite(filename=f"{output_folder}output_lines_{filename}", img=lines)

    
    