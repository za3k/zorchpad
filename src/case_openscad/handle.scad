nothing=0.01;

module rounded_linear_extrude(depth, radius, slices=50) {
    // Slice from 0...radius,             use 'offset'
    // Slice from radius..(depth-radius), just flat
    // Slice from (depth-radius)..radius, use 'offset'
    slice_thickness = depth/slices;
    union() {
        for (dz = [0:slice_thickness:depth]) {
            translate([0,0,dz])
            linear_extrude(slice_thickness+nothing)
            rounded_linear_extrude_crossection(depth, radius, dz) children();
        }
    }
}
module rounded_linear_extrude_crossection(depth, radius, dz) {
    d_end = (dz >= depth-radius) ? depth-dz-radius : (
        (dz <= radius) ? dz-radius : 0
    );
    
    // Rounded chamfer, not triangular
    inset = sqrt(radius*radius-d_end*d_end)-radius;
    
    offset(inset)
    children();
}

module handle(width, length, cutout_width, cutout_length, thickness, rounding) {
    intersection() {
        cube([length, width, thickness]);
        //linear_extrude(thickness) {
        rounded_linear_extrude(thickness, rounding) {
            difference() {
                hull() {
                    translate([width,width, 0])
                    circle(r=width);
                    translate([length-width,width, 0])
                    circle(r=width);
                }
            
                translate([length/2-cutout_length/2, width-cutout_width+nothing])
                square([cutout_length, cutout_width]);
            }
        }
    }
}

handle(width=25, length=80, 
       cutout_width=15, cutout_length=60,
       thickness=10, rounding=2);
