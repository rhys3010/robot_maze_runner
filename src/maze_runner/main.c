/**
  * Main file of the 'Maze Runner' hybrid robot controller
  * @author Rhys Evans (rhe24@aber.ac.uk)
  * @version 0.1
*/

#include "allcode_api.h"


/**
  * Initialize the application by starting various parts of the state machine
  * and loading the robot's API
*/
int init(){
  FA_RobotInit();

  FA_LCDBacklight(50);
}


/**
  *
*/
int main(){
  init();

  // Main Loop
  while(1){

  }

}
