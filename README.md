# Graduation-project-3D-printer
This repository will hold all the code written by our team in for the graduation project 3d printer.
Description of the entire control system :
In our control system we used a distributed control system in which we have a master credit-card sized computer - the Raspberry pi- which is used to control the slave microcontrollers- the Arduinos-, the master handles the communications and processing while the slaves handle the motion of the machine and reading the sensors and sending them back to the master. 
The master controller - the Raspberry pi- is the controller responsible for receiving the G-code generated from the slicer and interpreting it and decides the magnitudes of motion or temperature of the heaters and then sends these commands to the Arduinos and receives the results and decides what is needed to be done, the master is responsible for handling communications and processing and coordination between the other slaves.
The slaves controllers - the Arduinos- are responsible for handling the motion of the system and the temperatures of the extruder and the bed and interfacing with the
hardware in the system, it is planned to use 3 Arduinos - one for each axis- to move the stepper motors of the axes and the extruder and the temperatures of the extruder and the bed.
