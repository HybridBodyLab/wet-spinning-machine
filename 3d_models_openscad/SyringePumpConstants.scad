
/* === Project-universal Constants === */

//Not all of these variables are used in this specific model,
//but the same variable set is used across all models in this project.

$fs = 0.1;

inch = 25.4; //for mm conversion

//width of base
baseSizeY = 2*inch;
centerY = baseSizeY / 2; //both axes go right down the middle of the base

//stepper motor
stepperZSize = 42;
stepperYSize = 42;

//608 bearing 
bearing608Height = 8.5;
bearing608Diameter = 22.5;

//height of holder for 80/20 screws
mountPlateHeight = 5;

//axis heights
threadedAxisHeight = stepperZSize/2 + mountPlateHeight + 4;
smoothAxisHeight = (threadedAxisHeight-6.6) / 2;
smoothRodRadius = 4.1; //original value: 4.2, previously changed to 4.05

//syringe properties
barrelDiameter = 32;

//barrelSlotThickness = 3;
//barrelSlotHeight = 28;

syringeCenterHeight = threadedAxisHeight + bearing608Diameter/2 + 4 + barrelDiameter/2;

//Megan
plungerDiameter = 34; //previously 26.5, prev 35
plungerThickness = 2.7; //previously 3
plungerInnerDiameter = 19;//previously 15
plungerClipThickness = 3.5; //previously 3

//slot parameters
barrelSlotThickness = 2.5; //2.5, 1.7
barrelSlotHeight = 35;




//fixes render oddities
floatCorrection = 0.001; 
floatCorrection2 = 2*floatCorrection; 

//8020 mounting plate
screwPlateSize = 25;	
screwRadius = 3.5;
