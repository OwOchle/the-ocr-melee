from PIL import Image, ImageDraw, ImageFont
import random
import os

fonts = {
    "roboto": "./fonts/Roboto-Regular.ttf",
    "notoserif": "./fonts/NotoSerif-Regular.ttf",
    "sourgummy": "./fonts/SourGummy-Regular.ttf",
    "jbm": "./fonts/JetBrainsMono-Regular.ttf"
    }

OUTPUT = "./outputs"

chars = [chr(c) for c in range(ord("A"), ord("Z")+1)] # + [chr(c) for c in range(ord("a"), ord("z")+1)]

def generate_plain(f_file, char) -> Image.Image:
    img = Image.new("RGB", (25, 25), (255, 255, 255))

    d = ImageDraw.Draw(img)

    font = ImageFont.truetype(f_file, 20)

    d.text((12, 12), char, anchor="mm", fill=(0,0,0), font=font, align="center")

    return img


def generate_offcenter(f_file, char) -> Image.Image:
    img = Image.new("RGB", (25, 25), (255, 255, 255))

    d = ImageDraw.Draw(img)

    font = ImageFont.truetype(f_file, 20)

    x = random.randint(7, 17)
    y = random.randint(7, 17)

    d.text((x, y), char, anchor="mm", fill=(0,0,0), font=font, align="center")

    return img


def generate_rotated(f_file, char) -> Image.Image:
    img = Image.new("RGB", (25, 25), (255, 255, 255))

    d = ImageDraw.Draw(img)

    font = ImageFont.truetype(f_file, 20)

    d.text((12, 12), char, anchor="mm", fill=(0,0,0), font=font, align="center")

    rotation = random.randint(-20, 20)

    return img.rotate(rotation, fillcolor=(255, 255, 255))


def main():
    if not os.path.exists(OUTPUT):
        os.mkdir(OUTPUT)

    for f_name, f_file in fonts.items():
        for char in chars:
            generate_plain(f_file, char).save(f"{OUTPUT}/{f_name}_plain_0_{char}.bmp")
            for i in range(3):
                generate_offcenter(f_file, char).save(f"{OUTPUT}/{f_name}_offcenter_{i}_{char}.bmp")
                generate_rotated(f_file, char).save(f"{OUTPUT}/{f_name}_rotated_{i}_{char}.bmp")


if __name__ == "__main__":
    main()