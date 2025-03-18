bed_size = [230,210];
w = 1;//0.5;
h = 0.2;

for (x = [0:10:bed_size[0]]) {
    if (x%50 == 0 || x > 200) 
    translate([max(0, x-w), 0, 0])
    rotate([-90,0,0])
    cube([w, h, bed_size[1]]);
}
for (y = [0:10:bed_size[1]]) {
    if (y%50 == 0 || y > 200) 
    translate([0, max(0, y-w), 0])
    rotate([0,90,0])
    cube([h, w, bed_size[0]]);
}