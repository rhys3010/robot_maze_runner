===================
=== Maze Runner ===
===================

Author: Rhys Evans (rhe24@aber.ac.uk)
Version: 1.0

== Introduction ==
Maze Runner is a hybrid controller for the Formula Allcode Robot Buggy that
combines low-level reactive behaviours and higher level deliberative behaviours
to safely map and navigate a 4x4 maze.

== Prerequisites ==

* MPLAB xc16 compiler (http://microchipdeveloper.com/xc16:installation)
* Ensure the compiler's installation directory is updated in 'compile.sh' and '/bin/loader/faload.c'

== Installation ==

* Ensure the two scripts 'compile.sh' and 'load.sh' are executable,
(if not try chmod u+x)

* First run 'compile.sh' to generate the .bin and .hex file needed for
the robot's bootloader

* Then, ensuring the robot's bootloader is active by pressing the 'reset' button,
run the script 'load.sh'.
