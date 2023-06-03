# tamsyn: 8x16, 8x15, 7x14, 7x13b, 6x12r, 5x9r, 5x9b all look okay

# xterm -fn 5x8 -e bash -c "echo the quick brown fox jumps over the lazy dog; echo THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG && echo 0123456789 && read"

# First, yay -S tamsyn-font

# Convert PCF font to C-code

#python -c "print(''.join(chr(i) for i in range(32,127)), end='')" | convert -font -misc-tamsyn-medium-r-normal--16-116-100-100-c-80-iso8859-1 "label:@-" tamsyn8x16r.xbm # pbm is also nice if you get ascii mode out

# Again, but Latin-1, and it works better
convert -font -misc-tamsyn-medium-r-normal--16-116-100-100-c-80-iso8859-1 label:"$(python3 -c 'import sys; sys.stdout.buffer.write(b"".join({" ":b"\\ ","\\":b"\\\\","%":b"%%"}.get(chr(c), chr(c).encode("iso-8859-1")) for c in range(32,256)))')" -crop 1792x16+1+3 tamsyn8x16r.png
