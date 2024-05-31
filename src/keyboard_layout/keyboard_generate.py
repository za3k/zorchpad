from PIL import Image, ImageDraw, ImageFont
import math

"""
⎋ 1 2 3 4 5 6 7 8 9 0 - + ⌫_
⭾_ Q W E R T Y U I O P [ ] \
⇪__ A S D F G H J K L ; ' ⏎_
? ⇧_ Z X C V B N M , . / ↑ ?
⌃_ ⌘ ⌥_ ␣___________ f ← ↓ →
"""

# Modeled off the KLE format from https://golem.hu/guide/concept-to-reality/
layouts = [
    [
        ["ESC", "!\n1", "@\n2", "#\n3", "$\n4", "%\n5", "^\n6", "&\n7", "*\n8", "(\n9", ")\n0", "_\n-", "+\n=", ("Backspace", 1.5),],
        [("Tab", 1.5), "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{\n[", "{\n]", "|\n\\",],
        [("Caps Lock", 2), "A", "S", "D", "F", "G", "H", "J", "K", "L", ":\n;", "\"\n'", ("↵", 1.5),],
        ["", ("Shift", 1.5), "Z", "X", "C", "V", "B", "N", "M", "<\n,", ">\n.", "?\n/", "↑", "",],
        [ "Fn", ("Ctrl", 1.5), "OS", "Alt", (" ", 5), "", "", "←", "↓", "→", ]
    ],
    [ # 12 keys = 180mm wide. Missing: `~, [{, ]}, \|, F1-12, DEL, Sys/Home/End/Ins/PrtSc
        ["!\n1", "@\n2", "#\n3", "$\n4", "%\n5", "^\n6", "&\n7", "*\n8", "(\n9", ")\n0", "_\n-", "+\n=",],
        [("Tab", 1), "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P","←"],
        [("Esc", 1), "A", "S", "D", "F", "G", "H", "J", "K", "L", ":\n;", "\"\n'",],
        [("Shift", 1), "Z", "X", "C", "V", "B", "N", "M", "<\n,", ">\n.", "↑", "?\n/"],
        [("Ctrl", 1), "Os", "Alt", ("↵", 2.5), (" ", 2.5), "Fn", "←", "↓", "→",]
    ],
]

def parse_key(d):
    if isinstance(d, tuple):
        text, w = d
    else:
        text, w = d, 1.0
    lines = text.split("\n")
    if len(lines) == 2:
        l2, l1 = lines
    else:
        l1, l2 = lines[0], ""
    return l1, l2, w

def font_for(l):
    if len(l) == 1:
        return ImageFont.truetype("DejaVuSerif.ttf", 14)
    else:
        return ImageFont.truetype("DejaVuSans.ttf", 10)
def print_key(im, x_mm, y_mm, w_mm, h_mm, l1, l2):
    draw = ImageDraw.Draw(im)

    # draw rectangle
    W,H=w_mm*PPMM,h_mm*PPMM
    x1, y1 = x_mm*PPMM, y_mm*PPMM
    x2, y2 = x1 + W-2, y1 + H-2
    shape = ((x1, y1), (x2, y2))
    draw.rectangle(shape, outline="black", fill="white")
    
    # draw text
    if l1:
        t,l,w,h = draw.textbbox((0,0), l1)
        if len(l1)==1: t += 0.8*PPMM
        if l1 == "Backspace": l+= 0.5*PPMM
        draw.text((x1-l+(W-w+l)/2, y1-t+(H-h+t)/2),l1,"black", font=font_for(l1))
    if l2:
        t,l,w,h = draw.textbbox((0,0), l2)
        draw.text((x2-w-l-1*PPMM, y1+0.5*PPMM),l2,"black", font=font_for(l2))

KEY_PITCH = 15 # mm
KEY_SPACING = 1 # mm
BORDER = 0.5 # mm
PPMM = 2.8 # pixels per mm
def generate_layout(layout):
    img = Image.new("RGB", (int(300*PPMM), int(200*PPMM)), "white")
            
    y = BORDER*PPMM
    maxx = 0
    keys = 0
    for row in layout:
        x = BORDER*PPMM
        for d in row:
            l1, l2, width = parse_key(d)
            key_width, key_height = KEY_PITCH*width - KEY_SPACING, KEY_PITCH - KEY_SPACING
            print_key(img, x, y, key_width, key_height, l1, l2)
            x += width * KEY_PITCH
            keys += 1
        y += KEY_PITCH
        maxx = max(maxx, x)
    y += BORDER*PPMM - KEY_SPACING
    maxx += BORDER*PPMM - KEY_SPACING
    img = img.crop((0,0,int(maxx*PPMM),int(y*PPMM)))
    return img, keys

if __name__ == "__main__":
    for layout in layouts:
        img, keys = generate_layout(layout)
        img.save("keyboard{}.png".format(keys))
    img.show()

