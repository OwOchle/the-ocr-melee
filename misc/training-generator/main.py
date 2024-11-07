from PIL import Image, ImageDraw, ImageFont

fonts = {
    "roboto": "./fonts/Roboto-Regular.ttf",
    "notoserif": "./fonts/NotoSerif-Regular.ttf",
    "sourgummy": "./fonts/SourGummy-Regular.ttf",
    "jbm": "./fonts/JetBrainsMono-Regular.ttf"
    }

OUTPUT = "./outputs"

chars = [chr(c) for c in range(ord("A"), ord("Z")+1)] + [chr(c) for c in range(ord("a"), ord("z")+1)]

def main():
    for f_name, f_file in fonts.items():
        for char in chars:
            img = Image.new("RGB", (25, 25), (255, 255, 255))

            d = ImageDraw.Draw(img)

            font = ImageFont.truetype(f_file, 20)

            d.text((12,12), char, anchor="mm", fill=(0,0,0), font=font, align="center")

            # img.show()
            img.save(f"{OUTPUT}/{f_name}_{char}.bmp")


if __name__ == "__main__":
    main()