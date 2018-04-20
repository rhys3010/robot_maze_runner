/**
  * 'Maze Runner' hybrid robot controller
  * Explore a maze using a combination of reactive and deliberative behaviours, using
  * FSMs to implement a left wall following system and detect/store information about given cells.
  * @author Rhys Evans (rhe24@aber.ac.uk)
  * @version 0.1
*/
#include "allcode_api.h"
#include "main.h"


/**
  * Initialize the application by starting various parts of the state machine
  * and loading the robot's API
*/
void initialize(){
  // Initialize the API-specific things
  FA_RobotInit();
  FA_LCDBacklight(50);

  // Initialize the maze by iteratively setting its walls attributes
  for(int x = 0; x < SIZE_X; x++){
    for(int y = 0; y < SIZE_Y; y++){
      maze[x][y].walls[DIR_NORTH] = false;
      maze[x][y].walls[DIR_EAST] = false;
      maze[x][y].walls[DIR_SOUTH] = false;
      maze[x][y].walls[DIR_WEST] = false;

      maze[x][y].dark = false;
      maze[x][y].visited = false;
    }
  }
}

/**
  *
*/
void detect(){
}

/**
  *
*/
void drive(){
}

/**
  * The main loop of the system, through each iteration it checks the state of the
  * system and invokes the relevant behaviour
*/
int main(){
  // Initialize the State Machine
  mainState = MAIN_START;

  // Main Loop
  while(1){
    // Check state and behave accordingly
    switch(mainState){

      case MAIN_START:
        initialize();
      break;

      case MAIN_DETECT:
      break;

      case MAIN_TURN:
      break;

      case MAIN_DRIVE:
      break;

      case MAIN_FINISH:
      break;

      default:
      // todo: error handling
    }

  }
}
