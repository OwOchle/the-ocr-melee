import numpy as np

def apply_grayscale(img):
    R_COEF = 0.2989
    G_COEF = 0.5870
    B_COEF = 0.1140

    # expecting the format of img to be BGR
    b, g, r = img[..., 0], img[..., 1], img[..., 2]
    grayscale_image = B_COEF * b + G_COEF * g + R_COEF * r

    return grayscale_image