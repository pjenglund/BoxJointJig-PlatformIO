# BoxJointJig-PlatformIO
Driver code for a Box Joint Jig with an Arduino and a stepper motor.

**Use at your own risk.** 

This examle is done in Visual Studio Code and PlatformIO but can be modified to work in the Arduino IDE as well.

## Hardware used in this example
* Arduino Uno
* Stepper Motor Driver (to be specified)
* Stepper Motor
* 4 tactile buttons (Left, Right, Begin, Advance)

Hardware is similar, if not exactly the same, as used in this example by Cosmas: https://youtu.be/Stz70oeY37Q

## Operation
1. Move the sled into position by pressing the left or right buttons
2. When sled is in correct position related to the table saw blade, press the Begin button. The sled will wibrate slightly to indicate that start postion has been set.
3. Do you first cut (this may not cut anything off the material) and press the Advance button untill all fingers are cut

**Please make sure the sled and/or work piece is NOT over the blade when pressing the Advance button!**
