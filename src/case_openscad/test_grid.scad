bed_size = [220,220];
w = 1;//0.5;

for (x = [0:10:bed_size[0]]) {
    translate([x, 0, 0])
    rotate([-90,0,0])
    cylinder(h=bed_size[1], d=w);
}
for (y = [0:10:bed_size[1]]) {
    translate([0, y, 0])
    rotate([0,90,0])
    cylinder(h=bed_size[1], d=w);
}