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

  // Set the current cell pointer to the current position
  currentCell = &maze[currentPosX][currentPosY];

  // Move to the next state...
  changeMainState(MAIN_DETECT);
}

/**
  * Handle the detection behaviour of the state machine by
  * updating the model of the system depending on wall locations and light levels etc
*/
void detect(){

  // Mark Current Cell as Visited
  currentCell->visited = true;

  // Detect all the cell's walls and update the maze model
  currentCell->walls[DIR_NORTH] = FA_ReadIR(IR_FRONT) > WALL_DIST_THRESHOLD;
  currentCell->walls[DIR_EAST]  = FA_ReadIR(IR_RIGHT) > WALL_DIST_THRESHOLD;
  currentCell->walls[DIR_SOUTH] = FA_ReadIR(IR_REAR) > WALL_DIST_THRESHOLD;
  currentCell->walls[DIR_WEST]  = FA_ReadIR(IR_LEFT) > WALL_DIST_THRESHOLD;

  // Detect Light Level in the cell to check for nest then update model
  if(FA_ReadLight() < LIGHT_LEVEL_THRESHOLD){
    nestCell = currentCell;
  }

  // Advance the state machine
  changeMainState(MAIN_TURN);
}

/**
  * Implements the turning aspect of the robot's behaviour
  * by using the 'Left Hand Rule', allowing the robot to solve the maze
*/
void turn(){

  // Todo when turning:
  // * Update Direction
  // * Crash preventing reactive behaviours?
  // * Turn the robot

  // Check if left turn is possible (PRIORITY #1)
  if(!currentCell->walls[DIR_WEST]){
    // Turn left

    // Otherwise check if forward is possible (PRIORITY #2)
  }else if(!currentCell->walls[DIR_NORTH]){
    // Go forward
    // Probably change state?

    // Right turn possible? (PRIORITY #3)
  }else if(!currentCell->walls[DIR_EAST]){
    // Turn right

    // If none of the above are possible, robot is in dead end and must turn around
  }else{
    // Turn around
  }

  // Change state to drive out of cell
  changeMainState(MAIN_DRIVE);
}

/**
  * Implements the cruising aspect of the robot and it's basic reactive behaviour
  * Whilst simultaniously modelling the maze and using the FSM.
  * Constantly check for cell changes then enact the correct behaviour
*/
void drive(){

  // Todo when driving:
  // * Basic reactive behaviour to stop crash
  // * Check for floor lines (cell change)
  //    > Update Current pos + current cell
  //    > Change to 'detect' state
}


/**
  * The robot's behaviour when it has finished crawling the maze
  * (Some LED flashes and a buzzer sound)
*/
void end(){

  // Todo to end:
  // * Stop the robot
  // * Play a tune
  // * Flash some LEDs
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
        turn();
      break;

      case MAIN_DRIVE:
        drive();
      break;

      case MAIN_FINISH:
        end();
      break;

      default:
        puts("Error");
      // todo: error handling
    }
  }
}
