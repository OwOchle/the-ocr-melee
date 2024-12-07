import cv2
import numpy as np
import os


filter = False

filepath = "../../assets/"
filename = "image_lvl1_1.png"

file_path = os.path.join(filepath, filename)

img = cv2.imread(file_path)
rows, cols, _ = map(int, img.shape)

# img = cv2.pyrDown(img)
img_process = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

rows, cols, _ = map(int, img.shape)
print(rows, cols)

img_process = cv2.GaussianBlur(img_process, (3, 3), 0)
_, img_process = cv2.threshold(img_process, 127, 255, cv2.THRESH_BINARY_INV)

contours, _ = cv2.findContours(img_process, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
bbox = [cv2.boundingRect(c) for c in contours]
print("Box found = ", len(bbox))

cv2.imwrite("img_process.png", img_process)

for x, y, w, h in bbox:
    if (2 < w < 50) and (4 < h < 50):
        cv2.rectangle(img, (x, y), (x + w, y + h), (255, 0, 0), 2)

cv2.imwrite("img_result.png", img)