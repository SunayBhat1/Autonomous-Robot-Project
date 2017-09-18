# Autonomous-Robot-Project
Hobby Autonomous Robot Project

This autonomous robot utilizes an ultrasonic to navigate and make path decisions. It is a fairly basic project that could be done at home. The required hardware components are listed below. Additional equipment such as wires, soldering set, glue, etc would also be needed. Project does require basic circuitry/power electronics knowledge, C++/Arduino expereince, and soldering expereince. There are many similar projects online that could also be referenced. See primary code file for more details. Plenty of room for modifications and improvements on hardware and software sides. 

Would like to add in future designs:
	-IR sensor for failsafe and edge detection
	-Improved algorithms for tossing bad ultrasonic readings
	-Improved path decision algorithm
	-Options for bluetooth controls

Software in Repo:
	-Robot.ino: Arduino program for robot. See comments for details.  
	-Test.ino: Aduino program for testing components of the primary code. Not commented, be reference Robot.ino for component functionality. Modify to build own algorithms. 

Other Files in Repo:
	-autoRobo.mp4: Video of completed design.
	-L298 Tutorial.pdf: Wesbite printout with instructions/details for using L298

Hardware utilized in Project:
 	-Custom Arduino UNO (Use any standard Arduino UNO)
 	-L298 Motor Driver Module
 	-Emgreat® Motor Robot Car Chassis Kit (with two DC gear motors)
 	-HC-SR04 Ultrasonic Senor
 	-SG90 TowerPro Micro-Servo
 	-18V Battery Source (made by combining two 9V Alkaline batteries in series)