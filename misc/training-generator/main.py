from PIL import Image, ImageDraw, ImageFont
import random
import os

fonts = {
    "roboto": ImageFont.truetype("./fonts/Roboto-Regular.ttf", 20),
    "notoserif": ImageFont.truetype("./fonts/NotoSerif-Regular.ttf", 20),
    "sourgummy": ImageFont.truetype("./fonts/SourGummy-Regular.ttf", 20),
    "jbm": ImageFont.truetype("./fonts/JetBrainsMono-Regular.ttf", 20),
    "mavenpro": ImageFont.truetype("./fonts/MavenPro-Regular.ttf", 20),
    "garamond": ImageFont.truetype("./fonts/EBGaramond-Regular.ttf", 20),
    "cinzel": ImageFont.truetype("./fonts/Cinzel-Regular.ttf", 20)
    }

OUTPUT = "./outputs"

chars = [chr(c) for c in range(ord("A"), ord("Z")+1)] # + [chr(c) for c in range(ord("a"), ord("z")+1)]

def generate_plain(font, char) -> Image.Image:
    img = Image.new("RGBA", (20, 20), (255, 255, 255))

    gray = img.convert('1')
    img = gray.point(lambda x: 0 if x<128 else 255, '1')

    d = ImageDraw.Draw(img)

    d.text((10, 10), char, anchor="mm", fill=0, font=font, align="center")

    return img


def generate_offcenter(font, char) -> Image.Image:
    img = Image.new("RGB", (25, 25), (255, 255, 255))

    d = ImageDraw.Draw(img)

    x = random.randint(10, 14)
    y = random.randint(10, 14)

    d.text((x, y), char, anchor="mm", fill=(0,0,0), font=font, align="center")

    return img


def generate_rotated(font, char) -> Image.Image:
    img = Image.new("RGB", (25, 25), (255, 255, 255))

    d = ImageDraw.Draw(img)

    d.text((12, 12), char, anchor="mm", fill=(0,0,0), font=font, align="center")

    rotation = random.randint(-20, 20)

    return img.rotate(rotation, fillcolor=(255, 255, 255))


def main():
    if not os.path.exists(OUTPUT):
        os.mkdir(OUTPUT)

    for f_name, f in fonts.items():
        for char in chars:
            for i in range(5):
                generate_plain(f, char).save(f"{OUTPUT}/{f_name}_plain_{i}_{char}.bmp")
                # generate_offcenter(f, char).save(f"{OUTPUT}/{f_name}_offcenter_{i}_{char}.bmp")
                # generate_rotated(f, char).save(f"{OUTPUT}/{f_name}_rotated_{i}_{char}.bmp")


if __name__ == "__main__":
    main()
