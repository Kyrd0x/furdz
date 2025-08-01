from PIL import Image, ImageDraw, ImageFont

# === Config ===
width, height = 160, 90
text = "Ransomed"
output_c_path = "background.c"

# === Generate 1-bit image ===
img = Image.new("1", (width, height), color=0)  # black background
draw = ImageDraw.Draw(img)

try:
    font = ImageFont.truetype("arial.ttf", 14)
except IOError:
    font = ImageFont.load_default()

# Center the text
bbox = draw.textbbox((0, 0), text, font=font)
tw, th = bbox[2] - bbox[0], bbox[3] - bbox[1]
x, y = (width - tw) // 2, (height - th) // 2
draw.text((x, y), text, font=font, fill=1)  # white (monochrome)

# === Convert to raw 1-bit bytes ===
bitmap = img.tobytes()

# === Apply RLE compression ===
compressed = []
i = 0
while i < len(bitmap):
    run_value = bitmap[i]
    run_length = 1
    while i + run_length < len(bitmap) and bitmap[i + run_length] == run_value and run_length < 255:
        run_length += 1
    compressed.append(run_length)
    compressed.append(run_value)
    i += run_length

# === Format C array ===
def format_bytes(data, varname):
    lines = [f"const unsigned char {varname}[] = {{"]
    for i in range(0, len(data), 16):
        chunk = data[i:i+16]
        line = "    " + ", ".join(f"0x{b:02x}" for b in chunk) + ","
        lines.append(line)
    lines.append("};")
    return "\n".join(lines)

# === Write to C file ===
with open(output_c_path, "w") as f:
    f.write("// RLE-compressed 1-bit image\n")
    f.write(f"// Size: {len(compressed)} bytes, Dimensions: {width}x{height}\n\n")
    f.write(format_bytes(compressed, "background_image"))
    f.write(f"\n\nconst unsigned int background_image_len = {len(compressed)};\n")
    f.write(f"const unsigned int image_width = {width};\n")
    f.write(f"const unsigned int image_height = {height};\n")

print(f"[✔] C file written to: {output_c_path}")
