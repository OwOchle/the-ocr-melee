import math
import numpy as np
import cv2


rho_step = 9
theta_step = 0.261

def hough_lines(image, edges: np.ndarray, threshold: float, min_theta: float, max_theta: float) -> np.ndarray:
    height, width = image.shape[:2]
    diagonal = np.sqrt(height**2 + width**2)  # Max possible rho

    # Compute the range of theta and rho values
    theta_angles = np.arange(min_theta, max_theta, theta_step)
    rho_values = np.arange(-diagonal, diagonal, rho_step)
    num_thetas = len(theta_angles)
    num_rhos = len(rho_values)
    
    # Initialize the accumulator (Hough space)
    accumulator = np.zeros((num_rhos, num_thetas), dtype=np.int32)
    
    # Precompute sin and cos values for each theta
    sin_theta = np.sin(theta_angles)
    cos_theta = np.cos(theta_angles)

    # Get coordinates of edge points
    y_idxs, x_idxs = np.nonzero(edges)

    # Fill accumulator
    for x, y in zip(x_idxs, y_idxs):
        for t_idx in range(num_thetas):
            rho = x * cos_theta[t_idx] + y * sin_theta[t_idx]
            rho_idx = int((rho + diagonal) / rho_step)
            if 0 <= rho_idx < num_rhos:
                accumulator[rho_idx, t_idx] += 1

    # Find the accumulator values above the threshold
    # print(np.mean(accumulator))
    
    rho_idx, theta_idx = np.where(accumulator > threshold)

    # Copy the image to draw lines
    output_image = np.copy(image)
    
    # Draw detected lines on the image
    for r_idx, t_idx in zip(rho_idx, theta_idx):
        rho = rho_values[r_idx]
        theta = theta_angles[t_idx]

        # Convert polar coordinates to Cartesian for the lines
        a = np.cos(theta)
        b = np.sin(theta)
        x0 = a * rho
        y0 = b * rho
        
        # Points (x1, y1) and (x2, y2) to define the line
        x1 = int(x0 + 1000 * (-b))  # Extend the line in one direction
        y1 = int(y0 + 1000 * (a))
        x2 = int(x0 - 1000 * (-b))  # Extend the line in the other direction
        y2 = int(y0 - 1000 * (a))
        
        # Clip points to stay within the image bounds
        print((x1, y1), (x2, y2))
        cv2.line(output_image, (x1, y1), (x2, y2), (0, 0, 255), 2)

    return output_image

