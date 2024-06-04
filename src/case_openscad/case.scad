// Designed by Zachary Vance for the "Zorchpad"
//
// TO EXPORT STL FILES, run the following in a shell:
//   for part in top_shell bottom_shell keyboard_plate top_plate battery_box; do openscad -D part=\"$part\" -o $part.stl "case.scad" done
// (or if you have no idea what that means, change this variable:
part = "";
// To each of the 5 values listed above and export STL manually after each re-render.

keyboard_thickness = 4.8;

// Add a little extra room on the keyboard edges (l, r, t, b)
keyboard_edges = [0, 0, 5, 2];
// There's a rim around the bottom of the keyboard to hold it up. How wide is it?
keyboard_rim = 1.5;
// How tall is the rim around the keyboard and the spacers
spacer_height = 3;

edge_left = keyboard_edges[0];
edge_right = keyboard_edges[1];
edge_top_suggestion = keyboard_edges[2]; // at y>0
edge_bottom = keyboard_edges[3];

// Keyboard grid size (h, v)
grid = [12, 5];
rows = grid[1]; cols = grid[0];
// Horizontal and vertical key spacing
key_size = [17.8, 17.8];
key_gap_size = [0.5, 0.5];
key_pitch = [key_size[0]+key_gap_size[0],key_size[1]+key_gap_size[1]];
key_spacing_h = key_pitch[0];
key_spacing_v = key_pitch[1];
// Hole size (h, v)
hole_size = [14.5, 14];
hole_size_h = hole_size[0];
hole_size_v = hole_size[1];

spacer_side_h = key_spacing_h - hole_size_h;
spacer_side_v = key_spacing_v - hole_size_v;

// M2 screw sizes
screw_diameter = 2;
screw_countersink_diameter = 3.5;
screw_countersink_depth = 1.5;

// add room for 17-pin header (h, v)
pins_header = [43.5, 2.5];
pins_width = pins_header[0];
pins_height = pins_header[1];
edge_top = max(edge_top_suggestion, pins_height + 4);


// Clamshell (bottom keyboard half)
clamshell_depth_b = 5;
clamshell_thickness = 2;

// Clamshell (top half)
clamshell_depth_t = 15;
plate_thickness = 2;
screw_insert_diameter = 6;

magnet_diameter = 6;
magnet_thickness = 1.5;

logo_depth = 0.7;

nothing = 0.001;
big = 1000;

// x = width = col #
// y = thickness = row # 
// z = height

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

module flat_hole(x, y, width, height) {
    translate([x, y]) {
        square([width, height]);
    }
}

connections = [
    [1,[0]],
    [2.25, [0,24]],
    [2.75, [10,34]],
];
module key(x, y, width=1) {
    if (width <=1) key_hole(x, y, width);
    else {
        for (option = connections) {
            if (option[0] == width) {
                for (j = option[1]) {
                    translate([j,0])
                    key_hole(x, y,1);
                }
            }
        }
    }
}

module key_hole(x, y, width) {
    side_h = (key_spacing_h - hole_size_h)/2;
    side_v = (key_spacing_v - hole_size_v)/2;
    w = key_spacing_h*width - side_h*2;
    flat_hole(
        edge_left + x*key_spacing_h + side_h,
        edge_top + y*key_spacing_v + side_v,
        w, hole_size_v
    );
}

module support(x, y) {
    translate([x, y, -spacer_height/2])
    cube([spacer_side_h, spacer_side_v, spacer_height], center=true);
}

module key_support(x, y, width) {
    corner_x = edge_left + x*key_spacing_h;
    corner_y = edge_top + y*key_spacing_v;
    support(
       corner_x, corner_y
    );
}

board_w = edge_left + key_spacing_h * cols + edge_right;
board_h = edge_top  + key_spacing_v * rows + edge_bottom;

module pin_hole() {
    translate([board_w/2-pins_width/2, edge_top/2]) {
        square([pins_width, pins_height]);
    }
}

keys = [
    // Row, Column
    [0, 0, 1], [0, 1, 1], [0, 2, 1], [0, 3, 1], [0, 4, 1], [0, 5, 1], [0, 6, 1], [0, 7, 1], [0, 8, 1], [0, 9, 1], [0, 10, 1], [0, 11, 1],
    [1, 0, 1], [1, 1, 1], [1, 2, 1], [1, 3, 1], [1, 4, 1], [1, 5, 1], [1, 6, 1], [1, 7, 1], [1, 8, 1], [1, 9, 1], [1, 10, 1], [1, 11, 1],
    [2, 0, 1], [2, 1, 1], [2, 2, 1], [2, 3, 1], [2, 4, 1], [2, 5, 1], [2, 6, 1], [2, 7, 1], [2, 8, 1], [2, 9, 1], [2, 10, 1], [2, 11, 1],
    [3, 0, 1], [3, 1, 1], [3, 2, 1], [3, 3, 1], [3, 4, 1], [3, 5, 1], [3, 6, 1], [3, 7, 1], [3, 8, 1], [3, 9, 1], [3, 10, 1], [3, 11, 1],
    [4, 0, 1], [4, 1, 1], [4, 2, 1],
    [4, 3, 2.25], [4, 5.68, 2.25], // Spacebar, enter
    [4, 8, 1], [4, 9, 1], [4, 10, 1], [4, 11, 1],
];

module key_holes() {
  for (key = keys) key(key[1], key[0], key[2]);
}

module key_supports() {
  for (key = keys) key_support(key[1], key[0], key[2]);
}

module screw_hole(y, x, r=screw_diameter/2) {
    center_x = edge_left + x*key_spacing_h;
    center_y = edge_top + y*key_spacing_v;
    
    translate([center_x, center_y, -spacer_height/2-nothing])
    cylinder(h=spacer_height, r=r, center=true);
}

module top_screw_holes(r) {
    screw_hole(0.3, 1, r);
    screw_hole(1, cols-1, r);
    screw_hole(rows-0.5, 1, r);
    screw_hole(rows-0.5, cols-1, r);
}

module bottom_screw_holes(r) {
    screw_hole(1, 1, r);
    screw_hole(1, cols-1, r);
    screw_hole(rows-1, 1, r);
    screw_hole(rows-1, cols-1, r);
}

module keyboard_plate() {
    color("lightgreen")
    translate([0,0,spacer_height])
    union() {
    difference() {
    translate([0,board_h,0])
    scale([1,-1,1]) {
        linear_extrude(height=keyboard_thickness)
        difference() {
            square([board_w, board_h]);
            
            union() {
                key_holes();
                pin_hole();
            }
        }
        
        // Edge supports
        translate([0, 0, -spacer_height])
        cube([board_w, keyboard_rim, spacer_height]); // Top
            
        translate([0, board_h-keyboard_rim, -spacer_height])
        cube([board_w, keyboard_rim, spacer_height]); // Bottom

        translate([0, 0, -spacer_height])
        cube([keyboard_rim, board_h, spacer_height]); // Right
        
        translate([board_w-keyboard_rim, 0, -spacer_height])
        cube([keyboard_rim, board_h, spacer_height]); // Left
        
        // Grid intersection supports
        difference() {
            intersection() {
                translate([0,0,-big/2]) cube([board_w, board_h, big]);
                key_supports();
            }
            
            translate([0,0,-big/2])
            linear_extrude(height=big)
            pin_hole();
            
            bottom_screw_holes();
        }
    }
    hinge_hole_bottom(od=8);
    }
    
    handle_bottom();
    hinge_bottom();
    }
}

module rounded_thing(w, h, d) {
    r = d;
    x1 = 0 + r;
    y1 = 0 + r;
    x2 = w - r;
    y2 = h - r;
    
    intersection() {
        hull() {
            translate([x1,y1])
            sphere(r);
            
            translate([x2,y1])
            sphere(r);
            
            translate([x1,y2])
            sphere(r);
            
            translate([x2,y2])
            sphere(r);
        }
        translate([0,0,-d])
        cube([w, h, d]);
    }
}


module infinitely_tall(thing) {
    translate([0, 0, big/2])
    scale([1,1,big])
    children();
}
module remove_cutout(thing, cut) {
    difference() {
        children(0);
        infinitely_tall() children([1:$children-1]);
    }
}

module top_cutouts() {
    // Left sharp display screen
    translate([45,10])
    sharp_screen_cutout();
    
    // Left sharp display screen
    translate([board_w-45-43,10])
    sharp_screen_cutout();
    
    // E-ink screen
    translate([5,16])
    eink_screen_cutout();
    
    // GPIO cutout
    translate([board_w-45,2])
    gpio_cutout();
    
    // Power switch cutout
    translate([board_w-35,8])
    powerswitch_cutout();
    
    // Battery cutout
    translate([board_w-34,31])
    battery_cutout();
    
    // Connector cable to the keyboard
    translate([0,board_h,0])
    scale([1,-1,1])
    pin_hole();
}

module battery_cutout() {
    square([25,53]);
}


module clip_cutout() {
    // 8mm wide, 1.71mm dia hole, .25mm thick
    cylinder(h=100,d=1.8,center=true);
    
    translate([0,0,12])
    cube([5.89,8.38,0.5],center=true);
}

finger_width = 25; // To pull out battery
module battery_holder_attach(){
    w = (finger_width-12)/2;
    h = (53-30)/2;
    x_inset = w/2;
    ;
    y_inset = h/2;
    x1 = board_w-35-nothing/2+x_inset+1;
    x2 = x1+finger_width/2+12.5-w;
    y1 = 30-nothing/2+y_inset+1;
    y2 = y1+53-h;
    
    
    d = 2; // Screw diameter
    
    for (x = [x1,x2])
    for (y = [y1,y2])
    
    translate([x,y,plate_thickness/2])
    difference() {
        cube([w+nothing, h+nothing, plate_thickness], center=true);
        translate([0,0,-5])
        cylinder(d=2, h=10);
    }
}
module battery_holder() {
    difference() {
        cube([finger_width+2,55,13]);
        
        // Finger
        translate([1,12.5,-nothing])
        cube([finger_width,30,12]);
        
        // Battery hole
        translate([finger_width/2-5,1,-nothing])
        cube([12,53,12]);
        
        // Wire cutouts
        translate([finger_width/2+1,2,-nothing])
        cylinder(h=100,d=1.5,center=true);
        
        translate([finger_width/2+1,53,-nothing])
        cylinder(h=100,d=1.5,center=true);
        
        // Clip screw cutout(s)
        translate([finger_width/2+1,12.5+4.5,-nothing])
        clip_cutout();
        
        translate([finger_width/2+1,54-12.5-4.5,-nothing])
        clip_cutout();
        
        // Attach to the main piece with screws
        w = (finger_width-12)/2;
        h = (53-30)/2;
        x_inset = w/2;
        ;
        y_inset = h/2;
        x1 = nothing/2+x_inset+1;
        x2 = x1+finger_width/2+12.5-w;
        y1 = nothing/2+y_inset+1;
        y2 = y1+53-h;
        
        for (x = [x1,x2])
        for (y = [y1,y2])
        translate([x,y,-nothing])
        cylinder(d=2, h=5);
    }
}

module powerswitch_cutout() {
    square([18.6,12.1]);
}

module gpio_cutout() {
    square([22.7, 2.5]);
}

module eink_screen_cutout() {
    square([32, 37]);
}

module sharp_screen_cutout() {
    square([43, 63]);
}
    
module top_plate(include_battery_box) {
    color("orange")
    union() {
        difference() {
            remove_cutout() {
                difference() {
                    intersection() {
                        cube([board_w, board_h, plate_thickness]);
                        
                        scale([1,1,-1])
                        rounded_thing(board_w, board_h, clamshell_depth_t);
                    }
                    top_clamshell(board_w, board_h, clamshell_depth_t);
                }
                
                top_screw_holes();
                
                translate([0,0,-20])
                linear_extrude(20)
                top_cutouts();
            }
            
            // Countersinks
            translate([0,0,screw_countersink_depth])
            top_screw_holes(screw_countersink_diameter / 2);
            
            hinge_hole_top(od=8);
        }
 
        if (include_battery_box)
            translate([board_w-35,30,plate_thickness])
            battery_holder();
        battery_holder_attach();
        
        handle_top();
        hinge_top();
    }
}

module top_clamshell(w, h, d) {
    difference() {
        rounded_thing(w, h, d);
    
        color("yellow")
        translate([clamshell_thickness,clamshell_thickness,nothing])
        rounded_thing(w-clamshell_thickness*2, h-clamshell_thickness*2, d-clamshell_thickness);
    }
}

module logo(size=10) {
    linear_extrude(logo_depth)
    translate([board_w/2, board_h/2])
    text("Zorchpad", size, halign="center", valign="center");
}

module top_piece() {
    color("lightblue")
    translate([0,0,clamshell_depth_t])
    difference() {
        union() {
            top_clamshell(board_w, board_h, clamshell_depth_t);
            remove_cutout() {
                intersection() {
                    infinitely_tall()
                    top_screw_holes(screw_insert_diameter / 2);
                    
                    rounded_thing(board_w, board_h, clamshell_depth_t);
                }
                top_screw_holes();
            }
        }
        
        translate([0,0,-clamshell_depth_t])
        logo(20);
        
        microsd_port();
        audio_port();
    }
}

module bottom_clamshell(w, h, d) { 
    translate([0,0,d])
    difference() {
        color("pink")
        remove_cutout() {
            rounded_thing(w, h, d);
            bottom_screw_holes();
        }
        
        // Countersink
        translate([0,0,-d+screw_countersink_depth])
        bottom_screw_holes(screw_countersink_diameter / 2);
        
        translate([0,0,-logo_depth+nothing])
        logo(30);
    }
}

handle_size = [80, 25];
handle_length = handle_size[0];
handle_width = handle_size[1];
handle_groove_size = [50, 15];
handle_groove_length = handle_groove_size[0];
handle_groove_width = handle_groove_size[1];
handle_rounding = 1;
module handle_base(thickness) {
    translate([board_w/2-handle_length/2,-handle_width+handle_rounding,0])
    intersection() {
        cube([handle_length, handle_width, thickness]);
        rounded_linear_extrude(thickness, handle_rounding) {
            difference() {
                hull() {
                    translate([handle_width,handle_width, 0])
                    circle(r=handle_width);
                    translate([handle_length-handle_width,handle_width, 0])
                    circle(r=handle_width);
                }
            
                translate([handle_length/2-handle_groove_length/2, handle_width-handle_groove_width])
                square([handle_groove_length, handle_groove_width]);
            }
        }
    }
}

module audio_port() {
    translate([0,60,-11.6])
    rotate([0,0,270]) {
        translate([29.5,0,11.6/2])
        rotate([270,0,0])
        cylinder(d=6.8, h=7.7);
    
        translate([32-6,3.7,0])
        cube([10.5, 12, 11.6]);
        
    }
}

module microsd_port() {
    translate([0,board_h-40,-2.1+nothing])
    cube([31.9,24,2.1]);
}
module magnet_holes() {
    translate([board_w/2-handle_length/2+(handle_width-handle_groove_width)/2+magnet_diameter/2,-handle_width/2+magnet_diameter,-magnet_thickness])
    
    cylinder(h=magnet_thickness, d=magnet_diameter);
    
    translate([board_w/2+handle_length/2-(handle_width-handle_groove_width)/2-magnet_diameter/2,-handle_width/2+magnet_diameter+handle_rounding,-magnet_thickness])
    
    cylinder(h=magnet_thickness, d=magnet_diameter);
}
module handle_top() {
    difference() {
        handle_base(plate_thickness);
        
        translate([0,0,magnet_thickness-nothing])
        magnet_holes();
    }
}
module handle_bottom() {
    difference() {
        handle_base(keyboard_thickness);
    
        translate([0,0,keyboard_thickness+nothing])
        magnet_holes();
        
        translate([board_w/2,-handle_width,keyboard_thickness])
rotate([20,0,0])
cube([30, 15, 5], center=true);
    }
}

hinge_offset=1;
module hinge_offset_bottom(off, od) {
   translate([off,board_h+hinge_offset,0.9+plate_thickness
]) children();
}
module hinge_offset_top(off, od) {
    translate([off,board_h+hinge_offset,0-plate_thickness]) children();
}



module hinge_part(w, od, id, c) {
    difference() {
        union() {
        rotate([0,90,0])
        cylinder(h=w,d=od);
        
        if (c)
        translate([0,-od/2,-od/2])
        cube([w,hinge_offset+od/2,od]);
        }
    
        if (id > 0)
            translate([-nothing/2,0,0])
            rotate([0,90,0])
            cylinder(h=w+nothing,d=id);
    }
}

module hinge_hole_top(od) {
    w=102;
    part_w=w/6;
    
    hinge_offset_top(part_w+nothing/2, od)
    hinge_part(board_w-part_w*2-nothing,od-nothing,0,false);
}

module hinge_hole_bottom(od) {
    hinge_offset_bottom(0, od)
    hinge_part(board_w,od,0,false);
}

module hinge_part_top(off, w, od, id) {
    hinge_offset_top(off, od)
    hinge_part(w,od,id, false);
}

module hinge_part_bottom(off, w, od, id) {
    hinge_offset_bottom(off, od)
    hinge_part(w,od,id,true);
}

module hinge_top() {
    w=102;
    od=8;
    id=2;
    part_w=w/6;
    
    for (i = [1,3,5]) {
        hinge_part_top(part_w*i,part_w,od=8,id=2);
    }
    
    for (i = [2,4,6]) {
        hinge_part_top(board_w-part_w*i,part_w,od=8,id=2);
    }
}

module hinge_tapered_holes(id, depth) {
    translate([-nothing/2,board_h+hinge_offset,0.9+plate_thickness])
    rotate([0,90,0])
    cylinder(h=depth,d1=id*1.5,d2=id);
    
    translate([board_w+nothing/2,board_h+hinge_offset,0.9+plate_thickness])
    rotate([0,270,0])
    cylinder(h=depth,d1=id*1.5,d2=id);
}

module hinge_bottom() {
    w=102;
    od=8;
    id=2;
    part_w=w/6;
    
    difference() {
    union() {    
        for (i = [0,2,4]) {
            hinge_part_bottom(part_w*i,part_w,od=8,id=2);
        }
        
        for (i = [1,3,5]) {
            hinge_part_bottom(board_w-part_w*i,part_w,od=8,id=2);
        }
        
        // center solid
        center_w = board_w-102*2;
        hinge_part_bottom(102,w=center_w, od=8, id=0);
    }
    // Two tapered holes at the end, to help the pins lock in place
    hinge_tapered_holes(2, part_w/2);
    
    
    }
}

cool_render = false;
if (part && part == "top_shell") {
    top_piece();
} else if (part && part == "top_plate") {
    translate([0, board_h, plate_thickness])
    rotate([0,180,180])
    top_plate(false);
} else if (part && part == "bottom_shell") {
    bottom_clamshell(board_w, board_h, clamshell_depth_b);
} else if (part && part == "keyboard_plate") {
    translate([0,handle_width,0])
    keyboard_plate();
} else if (part && part == "battery_box") {
    translate([27,0,13])
    rotate([0,180,0])
    battery_holder();
} else if (cool_render) {
    translate([0, 0, clamshell_depth_b*20])
    scale([1,1,-1])
    top_piece();

    translate([0, 0, clamshell_depth_b*10])
    top_plate(true);

    keyboard_plate();

    translate([0, 0, -clamshell_depth_b*10])
    bottom_clamshell(board_w, board_h, clamshell_depth_b);
} else {
    translate([0, 150, 0])
    top_piece();

    translate([470, 0, plate_thickness])
    rotate([0,180,0])
    top_plate(false);
    
    translate([520, 0, 13])
    rotate([0,180,0])
    battery_holder();

    translate([250, 150, 0])
    keyboard_plate();

    translate([0, 0, 0])
    bottom_clamshell(board_w, board_h, clamshell_depth_b);
}