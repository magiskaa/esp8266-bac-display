include <BOSL2/std.scad>

wall_t = 2;

w = 101.5;
d = 72;
h = 46;

$fn = 100;

diff() {
	cuboid([w + 2*wall_t, d + wall_t, h + 2*wall_t], rounding = 3, edges = ["Y", BACK]) {
		tag("remove") fwd(wall_t / 2 + 0.001) cuboid([w, d, h], rounding = 2, edges = "Y");
		
		// USB PORT
		up(h / 2 - 21.6 + 9) back((d-wall_t) / 2 - 17.9)
		tag("remove") position(RIGHT) cuboid([5, 13.5, 6.3], anchor = TOP+BACK, rounding = 0.75, edges = "X");
		
		up(h / 2 - 21.6 + 2.6) right(w / 2 - 9.2) fwd(3)
		tag("remove") position(BACK) cuboid([9, 5, 4.7], anchor = BOTTOM+RIGHT, rounding = 1.5, edges = "Y");

		// BATTERY CHARGE INDICATOR
		up(h / 2 - 21.6 + 2.8 - 7.2) right(w / 2 - 30.5)
		tag("remove") position(BACK) cuboid([12, 5, 4.7], anchor = BOTTOM+RIGHT, rounding = 1.5, edges = "Y");
		
		// BATTERY SHIELD PORTS
		up(h / 2 - 21.6 + 1.1) left(w / 2 - 7.1)
		tag("remove") position(BACK) cuboid([9.3, 5, 4.3], anchor = TOP+LEFT, rounding = 1.5, edges = "Y");
		
		up(h / 2 - 21.6 + 1) left(w / 2 - 19.8)
		tag("remove") position(BACK) cuboid([8.7, 5, 4], anchor = TOP+LEFT, rounding = 1.5, edges = "Y");
		
		// ESP & ARDUINO PORTS
//		down(h / 2 - 5 - 6.6) left(70 / 2 - 22.3)
//		tag("remove") position(BACK) cuboid([8.7, 5, 4], rounding = 1.5, edges = "Y");
//		
//		down(h / 2 - 5 - 6.6 - 1.5) right(70 / 2 - 16)
//		tag("remove") position(BACK) cuboid([9.3, 5, 4.3], rounding = 1.5, edges = "Y");
	}	
}

xflip_copy(offset = 0)
right(w/2-wall_t) fwd((d+wall_t)/2-6) down(3)
diff() {
	hull() {
		ycyl(r = 3.5, h = 12);
		
		right(3.5) back(7) cuboid([1, 26, 9]);
	}
	
	tag("remove") ycyl(r = 2, h = 12.01);
}

// BATTERY

xflip_copy(offset = 0)
up(h / 2 - 21.6) left(w / 2) back((d-wall_t) / 2)
cuboid([4, 48, 2], anchor = TOP+LEFT+BACK, rounding = 0.8, edges = RIGHT, except = [BACK, FRONT]);

up(h / 2 - 21.6 + 3.3) left(w / 2) back((d-wall_t) / 2)
cuboid([4, 48, 2], anchor = BOTTOM+LEFT+BACK, rounding = 0.8, edges = RIGHT, except = [BACK, FRONT]);

up(h / 2 - 21.6 + 9.1) right(w / 2) back((d-wall_t) / 2)
cuboid([4, 48, 2], anchor = BOTTOM+RIGHT+BACK, rounding = 0.8, edges = LEFT, except = [BACK, FRONT]);


// ESP & ARDUINO

xflip_copy(offset = 0)
down(h / 2 - 5) left(w / 2) back((d-wall_t) / 2)
cuboid([(w - 70) / 2 + 4, 48, 2], anchor = TOP+LEFT+BACK, rounding = 0.8, edges = RIGHT, except = [BACK, FRONT]);

xflip_copy(offset = 0)
down(h / 2 - 5 - 1.75) left(w / 2) back((d-wall_t) / 2)
cuboid([(w - 70) / 2 + 4, 48, 2], anchor = BOTTOM+LEFT+BACK, rounding = 0.8, edges = RIGHT, except = [BACK, FRONT]);

xflip_copy(offset = 0)
down(h / 2 - 5 - 2) left(w / 2) back((d-wall_t) / 2)
cuboid([(w - 70) / 2 - 0.5, 48, 3], anchor = TOP+LEFT+BACK);

