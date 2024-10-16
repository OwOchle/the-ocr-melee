import os
import cv2


from gaussian import create_blurred_image
from sobel import apply_sobel

def load_image(filename : str, filepath : str = "../../assets/"):
    img_path = os.path.join(filepath, filename)
    img = cv2.imread(img_path)

    return img

if __name__ == "__main__":
    # https://en.wikipedia.org/wiki/Gaussian_blur
    filepath = "../../assets/"
    filename = "image_lvl3_2.png"

    

    img = load_image(filename=filename)

    # Radius is the size of the matrix
    # output_img = create_blurred_image(radius=1, sigma=1.5, image=img)
    # cv2.imwrite(filename=f"{filepath}output_grad_{filename}", img=output_img)

    img = load_image(filename=f"output_grad_{filename}")
    sobel_out = apply_sobel(img)

    cv2.imwrite(filename=f"{filepath}output_sobel_{filename}", img=sobel_out)

    
    