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
  * Change the system's main state and perform necessary actions
*/
void changeMainState(MainState newState){
  mainState = newState;
  // todo: timing stuff?
}

/**
  * Change the system's detect state and perform necessary actions
*/
void changeDetectState(DetectState newState){
  detectState = newState;
  // todo: timing stuff?
}

/**
  * Initialize the application by starting various parts of the state machine
  * and loading the robot's API
*/
void initialize(){
  // Loop variables - C99 :(
  int x;
  int y;

  // Initialize the API-specific things
  FA_RobotInit();
  FA_LCDBacklight(50);

  // Initialize the maze by iteratively setting its walls attributes
  for(x = 0; x < SIZE_X; x++){
    for(y = 0; y < SIZE_Y; y++){
      maze[x][y].walls[DIR_NORTH] = false;
      maze[x][y].walls[DIR_EAST] = false;
      maze[x][y].walls[DIR_SOUTH] = false;
      maze[x][y].walls[DIR_WEST] = false;

      maze[x][y].dark = false;
      maze[x][y].visited = false;
    }
  }

  // Construct the left + right border walls
  for(x = 0; x < SIZE_X; x++){
    maze[x][0].walls[DIR_WEST] = true;
    maze[x][SIZE_X - 1].walls[DIR_EAST] = true;
  }

  for(y = 0; y < SIZE_Y; y++){
    maze[SIZE_Y - 1][y].walls[DIR_SOUTH] = true;
    maze[0][y].walls[DIR_NORTH] = true;
  }

  // Move to the next state...
  changeMainState(MAIN_DETECT);
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
        detect();
      break;

      case MAIN_TURN:
      break;

      case MAIN_DRIVE:
        drive();
      break;

      case MAIN_FINISH:
      break;

      default:
      break;
      // todo: error handling
    }
  }
}
