def read_ppm_p3_clean(path):
    # Read and remove null bytes and BOM if any
    with open(path, "rb") as f:
        raw = f.read()

    clean_text = raw.replace(b'\x00', b'').decode("utf-8-sig", errors="ignore")
    lines = clean_text.splitlines()

    # Strip and remove comments/blank lines
    lines = [line.strip() for line in lines if line.strip() and not line.startswith('#')]

    if lines[0] != "P3":
        raise ValueError(f"Invalid PPM header after cleanup: {repr(lines[0])}")

    width, height = map(int, lines[1].split())

    pixels = list(map(int, " ".join(lines[3:]).split()))
    assert len(pixels) == width * height * 3, "Pixel data size mismatch"

    from PIL import Image
    img = Image.new("RGB", (width, height))
    idx = 0
    for y in range(height):
        for x in range(width):
            img.putpixel((x, y), tuple(pixels[idx:idx+3]))
            idx += 3

    return img

if __name__ == "__main__":
    img = read_ppm_p3_clean("image.ppm")
    img.show()