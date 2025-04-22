
include <syringePumpConstants.scad>

/* === Model-specific Constants === */

//cube containing rods and syringe barrel stop
mountXSize = 20;
mountZSize = syringeCenterHeight;


barrelSlotOffset = mountXSize/2 - barrelSlotThickness/2;


/* === Model === */

//main cube, holds the two axes
difference(){
	cube(size=[mountXSize,baseSizeY,mountZSize]);

	
	//bore hole for threaded rod
    //made bore hole same size as hole for smooth rod - Megan
	translate([-floatCorrection,centerY,threadedAxisHeight]){
		rotate(a=[0,90,0]){
			cylinder(h=mountXSize+floatCorrection2, r=smoothRodRadius+0.08);
		}
	}

	//hole for smooth rod (needs to fit fairly tight)
    //added a 5 unit thickness to hold the rod - Megan
	translate([-floatCorrection+5,centerY,smoothAxisHeight]){
		rotate(a=[0,90,0]){
			cylinder(h=mountXSize+floatCorrection2-5, r=smoothRodRadius+0.08);
		}
	}

	//hole for syringe barrel top to go in
	translate([barrelSlotOffset,centerY,syringeCenterHeight]){
		translate([0,-baseSizeY/2,-barrelSlotHeight/2]){
			cube(size=[barrelSlotThickness, baseSizeY, barrelSlotHeight]);
		}
	}	
	translate([-floatCorrection,centerY,syringeCenterHeight]){
		rotate(a=[0,90,0]){
			cylinder(h=mountXSize+floatCorrection2, r=plungerDiameter/2);
		}	
		translate([0,-plungerInnerDiameter/2,0]){
		}
	}	
	
}



// 80/20 screw plate
difference(){
	translate([-screwPlateSize,0,0]){
		cube(size=[screwPlateSize,baseSizeY,mountPlateHeight]);
	}

	translate([-screwPlateSize/2,baseSizeY/4,0]){
		cylinder(h=mountPlateHeight, r=screwRadius);
	}
	translate([-screwPlateSize/2,3*baseSizeY/4,0]){
		cylinder(h=mountPlateHeight, r=screwRadius);
	}

}

module bearing608(){
	cylinder(h=bearing608Height, r=bearing608Diameter/2);	
}
