include <BOSL2/std.scad>

wall_t = 2;

w = 101.5;
d = 3;
h = 46;

display_w = 50.6;
display_d = 10;
display_h = 19.3;

led_d = 11.9;
led_r = led_d / 2;

button_side = 6.2;

screw_r = 1.8;
screw_head_r = 3;

pwr_btn_r = 3;

$fn = 100;

diff() {
	cuboid([w + 2*wall_t, d, h + 2*wall_t], anchor = BOTTOM, rounding = 3, edges = ["Y", FRONT]) {
		down(3) fwd(d / 2) cuboid([display_w + 2, 5, display_h + 2], anchor = BOTTOM+FRONT, rounding = 1, edges = "Y");
		tag("remove") down(2) back(2) cuboid([display_w, display_d, display_h], anchor = BOTTOM);
		
		tag("remove") down(2 + 12) left(19) back(0.5) ycyl(r = led_r, h = d);
		tag("remove") down(2 + 12) left(19) ycyl(r = led_r - 0.5, h = d, anchor = BACK);
		
		tag("remove") down(2 + 12) left(1) back(0.5) ycyl(r = led_r, h = d);
		tag("remove") down(2 + 12) left(1) ycyl(r = led_r - 0.5, h = d, anchor = BACK);
		
		tag("remove") down(2 + 9.5) right(21.5) fwd(1) cuboid([button_side, 10, button_side], anchor = FRONT);
		tag("remove") down(2 + 9.5) right(21.5) cuboid([button_side - 1, d, button_side - 1], anchor = BACK, rounding = 0.5, edges = "Y");
		down(2 + 9.5) fwd(d / 2) right(21.5) cuboid([button_side + 2, 7, button_side + 2], anchor = FRONT, rounding = 1, edges = "Y");
		
		xflip_copy(offset = 0)
		tag("remove") right(w / 2 - wall_t) down(3) ycyl(r = screw_r, h = 10);
		xflip_copy(offset = 0)
		tag("remove") right(w / 2 - wall_t) fwd(0.5) down(3) ycyl(r = screw_head_r, h = 2.01);
		
		right(w / 2 - 10.8 + 0.7) up(h / 2 - 14.5 + 0.7) back(1.5) ycyl(r = pwr_btn_r + 1, h = 7, anchor = FRONT+TOP+RIGHT);
		tag("remove") right(w / 2 - 10.8) up(h / 2 - 14.5) fwd(1) ycyl(r = pwr_btn_r, h = 20, anchor = FRONT+TOP+RIGHT);
		tag("remove") right(w / 2 - 10.8 - 0.35) up(h / 2 - 14.5 - 0.35) ycyl(r = pwr_btn_r - 0.5, h = 20, anchor = TOP+RIGHT);
	}	
}

diff() {
	up(wall_t) back(d)
	cuboid([w, d, h], anchor = BOTTOM, rounding = 2, edges = "Y") {
		tag("remove") cuboid([w - 2, 10, h - 2], rounding = 2, edges = "Y");
		tag("remove") cuboid([w + 10, 10, h - 20]);
		tag("remove") cuboid([w - 20, 10, h + 10]);
	}
}
