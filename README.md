# Screensaver
The application displays variable number of balls bouncing on the screen following the laws of physics. 

## General Features
- The position, speed and colour of each ball is initially random. 
- The positions of the balls are ensured to be non-overlapping. 
- The balls collide elastically with the walls and also with each other.
- The speed of each ball can be increased or decreased by the user. The user can also pause or play the application.

## System Requirements
ScreenSaver runs on Linux based platforms only. It requires the following packages :
- GNU gcc compiler
- OPENGL utility library
- pthread library
- semaphore library

## Usage Instuctions

To run the main program, run:
``
make execute NUM_THREADS = N
``
where N is the number of balls.

To run the debug version, run:
``
make debug NUM_THREADS = N
``
where N is  the number of balls.

To run the random version:
``
make random NUM_THREADS = N
``
In this version the velocities of the balls will change arbitrarily. This is a specific testcase for the program.

Inside the GUI window, N number of bouncing balls will be seen on the screen. 6 buttons are included for various uses.

-	Pause/Play button: Pressing this button once will pause the movement of all the balls. Pressing it again will resume their movement.

-	Speed+ button: This button is used to increase the speed of a specific ball. To use this functionality:
	* Click on any ball. The ball may be moving or stationary. The program may be in the pause or play mode.
	* Click on the speed+ button a specific number of times. One button click multiplies the speed of the ball by a factor of 1.25.
	* In case pause mode was on, press the Pause/Play button to resume the screen saver. 
	* Maximum velocity of the balls is predefined.

-	Speed- button: This button decreases the speed of a specific ball. Usage instructions are same as that of Speed+ button. The speed is 		multiplied by a factor of 0.8 on each press.
	* Minimum velocity of the balls is predefined.

-	Size+ button: This button increases the radius of each ball. Maximum radius is predefined.  

-	Size- button: This button decreases the radius of each ball. Minimum radius is predefined.

-	Gravity button: This button turns gravity on/off.
