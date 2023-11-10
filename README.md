
#INF2004_Team14

INF 2004 Embedded Systems Robocar Project (Team 14)

Project Overview

This repository contains the source code, documentation, and related resources for Team14's Embedded Systems Robocar Project. Our mission was to design an intelligent autonomous 2-wheel robot car powered by the Raspberry Pi Pico microcontroller. This robot is equipped with a suite of sensors and implements a PID (Proportional-Integral-Derivative) controller to navigate a predefined track, efficiently avoiding obstacles and recognizing barcodes using infrared sensors.

Features:

Autonomous Navigation: Efficient navigation through a predefined track, avoiding obstacles.
Barcode Recognition: Ability to recognize and process barcodes using infrared sensors.
Dynamic Control: Integration of a PID controller to ensure smooth motion and accurate navigation.
User Interface: An interactive UI allowing users to initiate the robot, adjust parameters, visualize performance, and monitor the PID controller behavior in real-time.

Flow Process:
Initialize Raspberry Pi Pico and sensors.
User interface initialization and input PID parameters.
Start robot car and begin navigation.
Read data from various sensors:
Ultrasonic sensors for obstacle detection.
Infrared sensors for line following and barcode recognition.
Magnetometer for orientation.
Implement PID controller for movement adjustments.
Detect and process barcodes.
Navigate and map the route while optimizing for efficiency.
Provide real-time feedback through the user interface.
Continuous loop until end of track or user stops the robot.

Installation & Usage:
tbc

Team Members:
Awil Alessandra Antoinette Javier,
Sim Xin Rong, 
Ryan Raidley Yiap Chang Feng, 
Denzel Low E Loong, 
Muhammad Hidayah Bin Mohd Latif

Acknowledgments:
Special thanks to the INF 2004 course instructors for their guidance and support throughout this project.

License:

⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣴⣶⣶⣶⣤⠀⢀⣠⣤⣤⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢀⣴⣿⣿⡿⣿⠿⠟⠛⠛⠛⠟⠛⠻⠾⣯⣛⠿⣷⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⣠⣯⠞⠋⠁⡴⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⡟⠺⣿⣽⣆⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⢀⡼⢻⠏⠀⠀⠀⠀⠀⠀⠀⣀⠤⠴⠒⠚⠛⠒⠲⠤⣄⡀⠈⠻⣏⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⡞⠀⠀⠀⠀⠀⠀⠀⢀⡾⠋⠀⣀⡴⠖⠛⠛⠛⠛⠶⣤⡉⢳⡀⠘⣆⠀⠀⠀⠀⠀⠀⠀
⠀⣸⠁⠀⠀⠀⠀⠀⠀⢠⠏⠀⢠⡾⠋⠀⠀⠀⠀⠀⠀⠀⠀⠹⣦⠹⡄⠸⡄⠀⠀⠀⠀⠀⠀
⢀⣏⡀⠀⣀⣀⣀⣠⣴⡟⠀⢠⡟⠀⠀⠀⠀⠀⠀⠀⣶⡿⢿⡆⠘⣇⢹⣾⣿⡄⠀⠀⠀⠀⠀
⢸⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠻⣿⡾⠇⠀⣿⠈⡿⣿⠃⠀⠀⠀⠀⠀
⠈⢻⠛⠛⠛⠛⠛⠻⢼⣿⠀⠘⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡏⢸⠃⢸⠀⠀⠀⠀⠀⠀
⠀⢸⠀⠀⠀⠀⠀⠀⠀⠈⢧⠀⠹⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠟⢠⠏⠀⢸⠀⠀⠀⠀⠀⠀
⠀⠘⡇⠀⠀⠀⠀⠀⠀⠀⠀⠳⣄⡈⠻⢦⣤⣀⣀⣀⣠⡴⠞⢁⣴⠋⠀⠀⢸⡇⠀⠀⠀⠀⠀
⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠒⠤⠤⣤⣭⣤⡤⠤⠚⠉⠀⠀⠀⠀⠈⡇⠀⠀⠀⠀⠀
⠀⠀⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠁⠀⠀⠀⢀⡀⠀⠀⠀⠀⣹⡆⠀⠀⠀⠀
⠀⢀⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠲⠤⠤⠤⠤⠤⠤⠒⠉⠀⠀⠀⠀⣰⣿⣷⠀⠀⠀⠀
⠀⠸⣿⣷⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣴⣿⣿⡿⡀⠀⠀⠀
⠀⠀⣿⣿⣿⣿⣶⣤⣀⣀⣀⣀⣀⣀⣀⣀⣠⣤⣤⣤⣤⣶⣶⣾⣿⣿⡟⢙⡿⠋⣄⠹⡄⠀⠀
⠀⢰⠃⠈⠙⠿⣿⣿⡟⠛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⣿⣿⣿⣿⣿⣿⣿⠀⠀⢸⠀⢳⡀⠀
⢠⡏⠀⢠⠀⠀⠀⠹⣷⣾⣿⣿⣿⣿⣿⣼⣿⡿⠛⠻⢿⣿⣿⣿⣿⣿⣿⣿⡇⠀⣸⣀⠈⣧⠀
⡾⠀⢠⣯⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⢿⣿⡀⠿⠟⣩⣿⣏⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣼⡀
⡇⢀⣼⣿⣤⣤⣤⣤⣼⣿⣿⣿⣿⣿⣿⣯⣿⣿⠶⣾⣟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁
⣧⡀⠀⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠁⠀⠀⠀
⠈⠉⠓⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠋⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠉⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢿⣿⣿⣿⣿⣿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⡇⠈⢿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡿⠿⠿⠟⠛⠛⠁⠀⠘⠿⣿⡿⠿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀

