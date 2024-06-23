for part in top_shell bottom_shell keyboard_plate top_plate battery_box hinge; do 
    openscad -D part=\"$part\" -o $part.stl "case.scad"
done
