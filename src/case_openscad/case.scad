// TODO: Add hinge

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
key_pitch = [18.5, 18.5];
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

logo_depth = 0.7;

epsilon = 0.001;
big = 1000;

// x = width = col #
// y = thickness = row # 
// z = height

module flat_hole(x, y, width, height) {
    translate([x, y]) {
        square([width, height]);
    }
}

module key(x, y, width=1) {
    if (width <=1) key_hole(x, y, width);
    else if (width <= 1.5) key_hole(x+(width-1)/2);
    else {
        key_hole(x, y);
        key_hole(x+width-1, y);
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
    translate([board_w/2, edge_top/2]) {
        square([pins_width, pins_height]);
    }
}

keys = [
    // Row, Column
    [0, 0], [0, 1], [0, 2], [0, 3], [0, 4], [0, 5], [0, 6], [0, 7], [0, 8], [0, 9], [0, 10], [0, 11],
    [1, 0], [1, 1], [1, 2], [1, 3], [1, 4], [1, 5], [1, 6], [1, 7], [1, 8], [1, 9], [1, 10], [1, 11],
    [2, 0], [2, 1], [2, 2], [2, 3], [2, 4], [2, 5], [2, 6], [2, 7], [2, 8], [2, 9], [2, 10], [2, 11],
    [3, 0], [3, 1], [3, 2], [3, 3], [3, 4], [3, 5], [3, 6], [3, 7], [3, 8], [3, 9], [3, 10], [3, 11],
    [4, 0], [4, 1], [4, 2],
    [4, 3, 2.5], [4, 5.5, 2.5], // Spacebar, enter
    [4, 8], [4, 9], [4, 10], [4, 11],
];

module key_holes() {
  for (key = keys) {
    if (len(key) == 2) key_hole(key[1], key[0], 1);
    else key_hole(key[1], key[0], key[2]);       
  }
}

module key_supports() {
  for (key = keys) {
    if (len(key) == 2) key_support(key[1], key[0], 1);
    else key_support(key[1], key[0], key[2]);       
  }  
}

module screw_hole(y, x, r=screw_diameter/2) {
    center_x = edge_left + x*key_spacing_h;
    center_y = edge_top + y*key_spacing_v;
    
    translate([center_x, center_y, -spacer_height/2-epsilon])
    cylinder(h=spacer_height, r=r, center=true);
}

module top_screw_holes(r) {
    screw_holes(r);
}

module bottom_screw_holes(r) {
    screw_holes(r);
}

module screw_holes(r) {
    screw_hole(1, 1, r);
    screw_hole(1, cols-1, r);
    screw_hole(rows-1, 1, r);
    screw_hole(rows-1, cols-1, r);
}

module keyboard_plate() {
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
    
module top_plate() {
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
        }
        
        // Countersinks
        translate([0,0,screw_countersink_depth])
        bottom_screw_holes(screw_countersink_diameter / 2);
    }
}

module top_clamshell(w, h, d) {
    difference() {
        rounded_thing(w, h, d);
    
        color("yellow")
        translate([clamshell_thickness,clamshell_thickness,epsilon])
        rounded_thing(w-clamshell_thickness*2, h-clamshell_thickness*2, d-clamshell_thickness);
    }
}

module logo(size=10) {
    linear_extrude(logo_depth)
    translate([board_w/2, board_h/2])
    text("Zorchpad", size, halign="center", valign="center");
}

module top_piece() {
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
    }
}

module bottom_clamshell(w, h, d) { 
    difference() {
        remove_cutout() {
            rounded_thing(w, h, d);
            bottom_screw_holes();
        }
        
        // Countersink
        translate([0,0,-d+screw_countersink_depth])
        bottom_screw_holes(screw_countersink_diameter / 2);
        
        translate([0,0,-logo_depth+epsilon])
        logo(30);
    }
}

translate([0, 0, clamshell_depth_b*20])
color("lightblue")
scale([1,1,-1])
top_piece();

translate([0, 0, clamshell_depth_b*10])
color("orange")
top_plate();

color("lightgreen")
keyboard_plate();

translate([0, 0, -clamshell_depth_b*10])
color("pink")
bottom_clamshell(board_w, board_h, clamshell_depth_b);
