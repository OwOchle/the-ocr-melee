import cv2

def detect_grid_lines_y(image, target, line_width):
    """Detect grid letters via vertical lines.

    :param image: image to detect letters on
    :param target: image to print lines on.
    :param line_width: width of the marker used to detetc white pixels.
    :return: the target image with lines on it.
    """
    if (len(image.shape) == 3):
        height, width, _c = image.shape
    else:
        height, width = image.shape

    line_width = 3

    sum_line_height = 0

    for w in range(0, width, line_width):
        sum_step = 0
        for w2 in range(w, w+line_width):
            sum = 0
            for h in range(height):
                r,g,b = image[h,w]

                if (r != 0 or g != 0 or b != 0):
                    sum += 1
            sum_step += sum
        # print(w,w+line_width ,sum_step/line_width)
        sum_line_height += sum_step/line_width

    avg = (sum_line_height / (width / line_width))

    for w in range(0, width, line_width):
        sum_step = 0
        for w2 in range(w, w+line_width):
            sum = 0
            for h in range(height):
                r,g,b = image[h,w]

                if (r != 0 or g != 0 or b != 0):
                    sum += 1
            sum_step += sum
        if (sum_step/line_width > avg):
            cv2.line(target, (w, 0), (w+line_width, height), (90, 78, 20), 2)
    return target


def detect_grid_lines_x(image, target, line_width):
    """Detect grid letters via horizontal lines.

    :param image: image to detect letters on
    :param target: image to print lines on.
    :param line_width: width of the marker used to detetc white pixels.
    :return: the target image with lines on it.
    """
    if (len(image.shape) == 3):
        height, width, _c = image.shape
    else:
        height, width = image.shape
    print(height, width)

    sum_line_height = 0

    for h in range(0, height, line_width):
        sum_step = 0
        for h2 in range(h, h+line_width):
            sum = 0
            for w in range(width):
                r,g,b = image[h,w]

                if (r != 0 or g != 0 or b != 0):
                    sum += 1
            sum_step += sum
        # print(h,h+line_width ,sum_step/line_width)

        sum_line_height += sum_step/line_width

    avg = (sum_line_height / (width / line_width))

    for h in range(0, height, line_width):
        sum_step = 0
        for h2 in range(h, h+line_width):
            sum = 0
            for w in range(width):
                r,g,b = image[h,w]

                if (r != 0 or g != 0 or b != 0):
                    sum += 1
            sum_step += sum
        if (sum_step/line_width > avg):
            cv2.line(target, (0, h), (width, h++line_width), (20, 167, 78), 2)

    return target