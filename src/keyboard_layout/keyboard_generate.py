from PIL import Image, ImageDraw, ImageFont
import math

"""
⎋ 1 2 3 4 5 6 7 8 9 0 - + ⌫_
⭾_ Q W E R T Y U I O P [ ] \
⇪__ A S D F G H J K L ; ' ⏎_
? ⇧_ Z X C V B N M , . / ↑ ?
⌃_ ⌘ ⌥_ ␣___________ f ← ↓ →
"""

layout = [[
    ["ESC", "", 1],
    ["1", "!", 1],
    ["2", "@", 1],
    ["3", "#", 1],
    ["4", "$", 1],
    ["5", "%", 1],
    ["6", "^", 1],
    ["7", "&", 1],
    ["8", "*", 1],
    ["9", "(", 1],
    ["0", ")", 1],
    ["-", "_", 1],
    ["=", "+", 1],
    ["Backspace", "", 1.5],
],[
    ["Tab", "", 1.5],
    ["Q", "", 1],
    ["W", "", 1],
    ["E", "", 1],
    ["R", "", 1],
    ["T", "", 1],
    ["Y", "", 1],
    ["U", "", 1],
    ["I", "", 1],
    ["O", "", 1],
    ["P", "", 1],
    ["[", "{", 1],
    ["]", "}", 1],
    ["\\", "|", 1],
],[
    ["Caps Lock", "", 2],
    ["A", "", 1],
    ["S", "", 1],
    ["D", "", 1],
    ["F", "", 1],
    ["G", "", 1],
    ["H", "", 1],
    ["J", "", 1],
    ["K", "", 1],
    ["L", "", 1],
    [";", ":", 1],
    ["'", "\"", 1],
    ["↵", "", 1.5],
],[
    ["", "", 1],
    ["Shift", "", 1.5],
    ["Z", "", 1],
    ["X", "", 1],
    ["C", "", 1],
    ["V", "", 1],
    ["B", "", 1],
    ["N", "", 1],
    ["M", "", 1],
    [",", "<", 1],
    [".", ">", 1],
    ["/", "?", 1],
    ["↑", "", 1],
    ["", "", 1],
],[
    ["Fn", "", 1],
    ["Ctrl", "", 1.5],
    ["⊞", "", 1],
    ["Alt", "", 1],
    ["", "", 5],
    ["", "", 1],
    ["", "", 1],
    ["↑", "", 1],
    ["↑", "", 1],
    ["↑", "", 1],
]]

def font_for(l):
    if len(l) == 1:
        return ImageFont.truetype("DejaVuSerif.ttf", 14)
    else:
        return ImageFont.truetype("DejaVuSans.ttf", 10)
def print_key(im, x_mm, y_mm, w_mm, h_mm, l1, l2):
    draw = ImageDraw.Draw(img)

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
if __name__ == "__main__":
    img = Image.new("RGB", (int(300*PPMM), int(200*PPMM)), "white")
            
    y = BORDER*PPMM
    for row in layout:
        x = BORDER*PPMM
        for l1, l2, width in row:
            key_width, key_height = KEY_PITCH*width - KEY_SPACING, KEY_PITCH - KEY_SPACING
            print_key(img, x, y, key_width, key_height, l1, l2)
            x += width * KEY_PITCH
        y += KEY_PITCH
    y += BORDER*PPMM - KEY_SPACING
    x += BORDER*PPMM - KEY_SPACING
    img = img.crop((0,0,int(x*PPMM),int(y*PPMM)))
    img.show()
    img.save("keyboard.png")
