include <BOSL2/std.scad>

rod_r = 2.4;
rod_side = 6.1 - 0.2;
rod_h = 5;

pwr_rod_r = 2.85;
pwr_rod_h = 26.9;
pwr_rod_r_small = 2.4;


$fn = 100;

left(10)
cuboid([rod_side, rod_side, rod_h], rounding = 0.5, edges = "Z") {
	position(TOP) zcyl(r = rod_r, h = 2, anchor = BOTTOM);
}

right(10)
zcyl(r = pwr_rod_r, h = pwr_rod_h) {
	position(TOP) zcyl(r = pwr_rod_r_small, h = 1.6, anchor = BOTTOM, rounding2 = 1);
}

diff() {
	fwd(30)
	cuboid([60, 19.5, 8], rounding = 2) {
		yflip_copy(offset = 0)
		tag("remove") position(RIGHT) fwd(3) cuboid([6, 4, 4.2]);
	}
}
