// Designed by Zachary Vance for the "Zorchpad"
//
// TO EXPORT STL FILES, run the following in a shell:
//   for part in top_shell bottom_shell keyboard_plate top_plate battery_box; do openscad -D part=\"$part\" -o $part.stl "case.scad" done
// (or if you have no idea what that means, change this variable:
part = "";
// To each of the 5 values listed above and export STL manually after each re-render.
cool_render = true;

side_thickness = 2;
keyboard_thickness = 4.8;

// Add a little extra room on the keyboard edges (l, r, t, b)
keyboard_edges = [side_thickness, side_thickness, 5, 2];
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
key_gap_size = [0.25, 0.25];
key_pitch = [key_size[0]+key_gap_size[0],key_size[1]+key_gap_size[1]];
key_spacing_h = key_pitch[0];
key_spacing_v = key_pitch[1];
// Hole size (h, v)
hole_size = [14, 14];
hole_size_h = hole_size[0];
hole_size_v = hole_size[1];

// How much do the complete keys protrude above the keyboard?
keycap_height = 15;

spacer_side_h = key_spacing_h - hole_size_h;
spacer_side_v = key_spacing_v - hole_size_v;

// M2 screw sizes
screw_diameter = 2;
screw_countersink_diameter = 3.5+2;
screw_countersink_depth = 1.5;
screw_diameter_safety = 1;
screw_insert_diameter = 4;

// add room for 17-pin header (h, v)
pins_header = [43.5, 2.5];
pins_width = pins_header[0];
pins_height = pins_header[1];
edge_top = max(edge_top_suggestion, pins_height + 6);


// Clamshell (bottom keyboard half)
clamshell_depth_b = 5;
clamshell_thickness = 2;

// Clamshell (top half)
clamshell_depth_t = 15;
plate_thickness = 2;


magnet_diameter = 6;
magnet_thickness = 1.5;

logo_depth = 0.7;

// Handle
handle_size = [100, 32];
handle_length = handle_size[0];
handle_width = handle_size[1];
handle_groove_size = [65, 22];
handle_groove_length = handle_groove_size[0];
handle_groove_width = handle_groove_size[1];
handle_rounding = 1;

// Hinge
hinge_offset=1;
//hinge_offset_bottom=-1.5-keycap_height;
hinge_offset_bottom=-.7;
hinge_w=102;
hinge_od=7;
hinge_id=2;
hinge_id_safety=2;
hinge_w_safety=1;

side_hinge_gap_safety=1;
side_hinge_gap = hinge_od/2+side_hinge_gap_safety;

logo_back = true;
logo_ui = true;
logo_hidden = true;

// Sides (attached to top plate)

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
    min_gap_to_keys = (key_spacing_v-hole_size_v);
    
    translate([board_w/2-pins_width/2, edge_top-min_gap_to_keys]) {
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

module screw_hole(y, x, d=undef) {
    hole_d = (d == undef ? screw_diameter : d) + screw_diameter_safety;
    center_x = edge_left + x*key_spacing_h;
    center_y = edge_top + y*key_spacing_v;
    
    translate([center_x, center_y, -spacer_height/2-nothing])
    cylinder(h=spacer_height, d=hole_d, center=true);
}

module top_plate_screwholes(d) {
    top_screw_holes(d);
    
    // TODO: Move battery box holes here
}

module top_clamshell_screwholes(d) {
    translate([0, board_h,0])
    mirror([0,1,0])
    top_screw_holes(d);
}

module top_screw_holes(d) {
    screw_hole(0.1, 1, d);
    screw_hole(1, cols-1, d);
    screw_hole(rows-0.5, 1, d);
    screw_hole(rows-0.5, cols-1, d);
}

module keyboard_screwholes(d) {
    bottom_screw_holes(d);
}

module bottom_clamshell_screwholes(d) {
    bottom_screw_holes(d);
}

module bottom_screw_holes(d) {
    screw_hole(1, 1, d);
    screw_hole(1, cols-1, d);
    screw_hole(rows-1, 1, d);
    screw_hole(rows-1, cols-1, d);
}

module edge_supports() {
    // Bottom
    translate([0, board_h-keyboard_rim, -spacer_height])
    cube([board_w, keyboard_rim, spacer_height]);

    // Right
    translate([0, 0, -spacer_height])
    cube([keyboard_rim, board_h, spacer_height]);

    // Left
    translate([board_w-keyboard_rim, 0, -spacer_height])
    cube([keyboard_rim, board_h, spacer_height]);
}

module hinge_support_bottom() {
    top_rim_thickness = 4;
    top_pin_edge = 1.7;
    gap_width = pins_width;
    translate([0, board_h-top_rim_thickness-2.5, -keycap_height])
    difference() {
        cube([board_w, top_rim_thickness, keycap_height]);
            
        translate([board_w/2-gap_width/2, -nothing/2, -nothing/2])
        cube([gap_width+nothing, top_rim_thickness-top_pin_edge+nothing, keycap_height+nothing]);
    }
}

module all_key_supports() {
        difference() {
        intersection() {
            translate([0,0,-big/2]) cube([board_w, board_h, big]);
            key_supports();
        }
        
        translate([0,0,-big/2])
        linear_extrude(height=big)
        pin_hole();
        
        keyboard_screwholes();
    }
}

module keyboard_plate_main() {
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
        edge_supports();
        
        // Grid intersection supports
        all_key_supports();
    }
}

module keyboard_plate() {
    color("lightgreen")
    translate([0,0,spacer_height])
    union() {
        difference() {
            translate([0,board_h,0])
            keyboard_plate_main();
            
            hinge_hole_bottom(od=hinge_od);
        }
        
        handle_bottom();
        
        //hinge_support_bottom();
        hinge_bottom();
        //hinge_hole_bottom(od=hinge_od);
    }
}

rounding_min_radius = 7;
rounding_flat_part = 2;
module rounded_thing(w, h, d) {
    add_smoothing=d<rounding_min_radius;
    flat_d = add_smoothing ? rounding_flat_part : 0;
    r = d - flat_d;
    x1 = 0 + r;
    y1 = 0 + r;
    x2 = w - r;
    y2 = h - r;
    
    intersection() {
        hull() {
            for (x = [x1, x2])
            for (y = [y1, y2]) {
                translate([x,y])
                sphere(r);
                
                if (add_smoothing)
                translate([x,y,-flat_d])
                sphere(r);
            }
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
    translate([43,10])
    sharp_screen_cutout();
    
    // Left sharp display screen
    translate([board_w-43-64,10])
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
    cylinder(h=100,d=1.8+screw_diameter_safety,center=true);
    
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
    
    for (x = [x1,x2])
    for (y = [y1,y2])
    
    translate([x,y,plate_thickness/2])
    difference() {
        cube([w+nothing, h+nothing, plate_thickness], center=true);
        
        // Screw hole
        translate([0,0,-5])
        cylinder(d=screw_diameter+screw_diameter_safety, h=10);
        
        // Countersink
        translate([0,0,plate_thickness-screw_countersink_depth*2-nothing/2])
        cylinder(d=screw_countersink_diameter, h=screw_countersink_depth+nothing);
    }
}

battery_holder_w = finger_width+2;
battery_holder_h = 55;
battery_holder_d = 13;
module battery_holder() {
    difference() {
        cube([battery_holder_w,battery_holder_h,battery_holder_d]);
        
        // Finger
        translate([1,12.5,-nothing])
        cube([finger_width,30,12]);
        
        // Battery hole
        translate([finger_width/2-5,1,-nothing])
        cube([12,53,12]);
        
        // Wire cutouts
        translate([finger_width/2+1,2.5,-nothing])
        cylinder(h=100,d=1.5+screw_diameter_safety,center=true);
        
        translate([finger_width/2+1,52.5,-nothing])
        cylinder(h=100,d=1.5+screw_diameter_safety,center=true);
        
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
        cylinder(d=screw_diameter + screw_diameter_safety, h=5);
    }
}

module powerswitch_cutout() {
    square([19.6,12.1]);
}

module gpio_cutout() {
    square([22.7, 2.5]);
}

module eink_screen_cutout() {
    square([32, 37]);
}

module sharp_screen_cutout() {
    square([64, 43]);
}

module top_plate_shape() {
    square([board_w, board_h]);
}

/* Cool looking version
module top_plate_shape() {
    projection()
    difference() {
        intersection() {
            cube([board_w, board_h, plate_thickness]);
            
            scale([1,1,-1])
            rounded_thing(board_w, board_h, clamshell_depth_t);
        }
        top_clamshell(board_w, board_h, clamshell_depth_t);
    }
}
*/

module outline(t) {
    difference() {
        children();
        
        offset(r=-t)
        children();
    }
}

module sides_main(d) { 
    linear_extrude(d) {    
        outline(side_thickness)
        top_plate_shape();
    }
}

module round_corner(corner, normal) {
    sr = 3;
    dist = 7;
    dir = -normal / norm(normal);
    
    difference() {
        children();
    
        translate(corner)
        difference() {    
            cube(sr*2, center=true);
            
            translate(dir * dist)
            sphere(r=sr*2);
        }
    }    
}
    
module sides(d) {
    translate([0,0,-d])
    sides_main(d);
}

module top_plate_countersinks() {
    translate([0,0,screw_countersink_depth])
            top_plate_screwholes(screw_countersink_diameter);
}

module top_plate() {
    color("orange")
    union() {
        difference() {
            linear_extrude(plate_thickness)
            difference() {
                top_plate_shape();
                projection()
                top_plate_screwholes();
                top_cutouts();
            }
            
            top_plate_countersinks()
            translate([board_w/2, board_h-25,-nothing])
            mirror([0,1,0])
            logo(10);
        }
        
        battery_holder_attach();
        
        difference() {
            sides(keycap_height);
            hinge_hole_top(od=hinge_od);
        }
        
        translate([0,0,-keycap_height])
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

module logo_offset(d=0) {
    translate([board_w/2, board_h/2, d])
    children();
}
module logo(size=10) {
    font = abs(size);
    reversed = size <= 0 ? 1 : 0;
    
    linear_extrude(logo_depth)    
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
                    top_clamshell_screwholes(screw_insert_diameter);
                    
                    rounded_thing(board_w, board_h, clamshell_depth_t);
                }
                top_clamshell_screwholes();
            }
        }
        
        if (logo_back)
        logo_offset(-clamshell_depth_t)
        mirror([1,0,0])
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
            bottom_clamshell_screwholes();
        }
        
        // Countersink
        translate([0,0,-d+screw_countersink_depth])
        bottom_clamshell_screwholes(screw_countersink_diameter);
        
        if (logo_hidden)
        logo_offset(-logo_depth+nothing)
        rotate([0,0,180])
        logo(30);
    }
}

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
    // Height of the boxy part of the jack
    bh=11.6;
    // Diameter of the jack cylinder
    cd = 8;
    // Length of the jack cyclinder
    ch = 7.7;
    
    translate([0,60,-bh])
    rotate([0,0,270]) {
        translate([29.5,0,bh/2]) {
            rotate([270,0,0])
            cylinder(d=cd, h=ch);
        
            //translate([32-6,3.7,0])
            translate([-cd/2,0+ch-nothing,-bh/2])
            cube([cd, 12, bh]);
        }
    }
}

module microsd_port() {
    spacer = 3;
    translate([0,board_h-40,-2.1-spacer+nothing])
    cube([31.9,24,2.1]);
}
module magnet_holes() {
    hole_diameter = magnet_diameter + 0.5;
    hole_depth = magnet_thickness + 0.5;
    
    translate([board_w/2-handle_length/2+(handle_width-handle_groove_width)/2+hole_diameter/2,-handle_width/2+hole_diameter,-hole_depth])
    
    cylinder(h=hole_depth, d=hole_diameter);
    
    translate([board_w/2+handle_length/2-(handle_width-handle_groove_width)/2-hole_diameter/2,-handle_width/2+hole_diameter+handle_rounding,-hole_depth])
    
    cylinder(h=hole_depth, d=hole_diameter);
}

handle_top_thickness = 4;
module handle_top() {
    difference() {
        handle_base(handle_top_thickness);
        
        translate([0,0,magnet_thickness-nothing])
        magnet_holes();
    }
}
module handle_bottom() {
    difference() {
    //union() {
        translate([0,0,-spacer_height])
        handle_base(keyboard_thickness+spacer_height);
    
        translate([0,0,keyboard_thickness+nothing])
        magnet_holes();
        
        translate([board_w/2,-handle_width+2,keyboard_thickness])
        rotate([70,0,0])
        cube([30, 15, 5], center=true);
    }
}



module hinge_offset_bottom(off, od) {
   translate([off,board_h+hinge_offset,plate_thickness+hinge_offset_bottom
]) children();
}
module hinge_offset_top(off, od) {
    translate([off,board_h+hinge_offset,0-plate_thickness-keycap_height]) children();
}



module hinge_part(w, od, id, c, reversed) {
    // Width, outer diameter, inner diameter, cube support
    
    // Normal or cutout safety
    poffset = hinge_w_safety/2 * (reversed ? -1 : 1);
    part_w = w + hinge_w_safety * (reversed ? 1 : -1);
    
    translate([poffset,0,0])
    difference() {
        union() {
        
        rotate([0,90,0])
        cylinder(h=part_w,d=od);
        
        if (c)
        translate([0,-od/2,-od/2])
        cube([part_w,hinge_offset+od/2,od]);
        }
    
        if (id > 0)
            translate([-nothing/2,0,0])
            rotate([0,90,0])
            cylinder(h=part_w+nothing,d=id+hinge_id_safety);
    }
}

hinge_diameter_gap = 1;
module hinge_hole_top(od) {
    part_w=hinge_w/6;
    
    // Center hole
    hinge_offset_top(part_w+nothing/2, od)
    hinge_part(board_w-part_w*2-nothing,od-nothing,0,false);
    
    // Remove where the hinge will go
    for (i = [0,2,4]) {
        hinge_part_top(part_w*i,part_w,od+hinge_diameter_gap,0, true);
    }
    
    for (i = [0,2,4]) {
        hinge_part_top(board_w-part_w*(i+1),part_w,od+hinge_diameter_gap,0, true);
    }
}

module hinge_hole_bottom(od) {
    /*
    hinge_offset_bottom(0, od) {        
        hinge_part(board_w,od,0,false);
        
        translate([-nothing/2,-od/2-nothing/2,nothing/2])
        cube([board_w+nothing, od+nothing, od/2+big]);
    }*/
    
    part_w=hinge_w/6;
    // Remove where the hinge will go
    for (i = [1,3,5]) {
        hinge_part_bottom(part_w*i,part_w,od+hinge_diameter_gap,0, true);
    }
    
    for (i = [1,3,5]) {
        hinge_part_bottom(board_w-part_w*(i+1),part_w,od+hinge_diameter_gap,0, true);
    }
}

module hinge_part_top(off, w, od, id, reversed=false) {
    hinge_offset_top(off, od)
    hinge_part(w,od,id,false,reversed);
}

module hinge_part_bottom(off, w, od, id, reversed=false) {
    hinge_offset_bottom(off, od)
    //hinge_part(w,od,id,!reversed,reversed);
    hinge_part(w,od,id,true,reversed);
}

module hinge_top() {
    w=hinge_w;
    od=hinge_od;
    id=hinge_id;
    part_w=w/6;
    
    for (i = [1,3,5]) {
        hinge_part_top(part_w*i,part_w,od,id);
    }
    
    for (i = [2,4,6]) {
        hinge_part_top(board_w-part_w*i,part_w,od,id);
    }
}

module hinge_tapered_holes(od, id, depth) {
    translate([0,0,hinge_offset_bottom]){
    
    translate([-nothing/2,board_h+hinge_offset,plate_thickness])
    rotate([0,90,0])
    cylinder(h=depth,d1=id*1.5,d2=id);
    
    translate([board_w+nothing/2,board_h+hinge_offset,plate_thickness])
    rotate([0,270,0])
    cylinder(h=depth,d1=id*1.5,d2=id);
        
    }
}

module hinge_bottom() {
    w=hinge_w;
    od=hinge_od;
    id=hinge_id;
    
    part_w=w/6;
    
    difference() {
    union() {    
        for (i = [0,2,4]) {
            hinge_part_bottom(part_w*i,part_w,od,id);
        }
        
        for (i = [1,3,5]) {
            hinge_part_bottom(board_w-part_w*i,part_w,od,id);
        }
        
        // center solid
        center_w = board_w-w*2;
        hinge_part_bottom(w,center_w, od, 0);
    }
    // Two tapered holes at the end, to help the pins lock in place
    hinge_tapered_holes(od, 2, part_w/2);
    
    
    }
}

if (part && part == "top_shell") {
    top_piece();
} else if (part && part == "top_plate") {
    translate([0, board_h, plate_thickness])
    rotate([0,180,180])
    top_plate();
} else if (part && part == "bottom_shell") {
    bottom_clamshell(board_w, board_h, clamshell_depth_b);
} else if (part && part == "keyboard_plate") {
    translate([board_w,handle_width,keyboard_thickness+spacer_height])
    rotate([0,180,0])
    keyboard_plate();
} else if (part && part == "battery_box") {
    translate([27,0,13])
    rotate([0,180,0])
    battery_holder();
} else if (part && part == "hinge") {
    intersection() {
        cube([board_w,40,30]);
        union() {
            translate([board_w, -90, plate_thickness])
            rotate([0,180,0])
            top_plate();
            
            translate([0, 130, (keyboard_thickness+spacer_height)])
            rotate([0,180,180])
            keyboard_plate();
        }
    }
} else if (cool_render) {
    // Assembly Render
    translate([0, board_h, clamshell_depth_b*20])
    rotate([0,180,180])
    top_piece();

    translate([0, 0, clamshell_depth_b*10])
    rotate([180,180,180])
    top_plate();
    
    translate([-35,30,plate_thickness])
    translate([board_w,0,clamshell_depth_b*10])
    battery_holder();

    keyboard_plate();

    translate([board_w, board_h, -clamshell_depth_b*10])
    rotate([0,0,180])
    bottom_clamshell(board_w, board_h, clamshell_depth_b);
} else {
    // Printing Preview
    translate([0, 150, 0])
    top_piece();

    translate([450, 0, plate_thickness])
    rotate([0,180,0])
    top_plate();
    
    translate([265, -100, 13])
    rotate([0,180,0])
    battery_holder();

    translate([450, 150, (keyboard_thickness+spacer_height)])
    rotate([0,180,0])
    keyboard_plate();

    translate([0, 0, 0])
    bottom_clamshell(board_w, board_h, clamshell_depth_b);
}

echo(board_w);