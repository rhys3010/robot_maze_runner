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
  int i;

  // State first-time entry behaviour
  switch(mainState){

    case MAIN_DRIVE:
      FA_SetMotors(MOTOR_SPEED, MOTOR_SPEED);
    break;

    case MAIN_FINISH:
      // Turn on all front LEDs
      for(i = 0; i < 8; i++){
        FA_LEDOn(i);
      }

      // Play a quick tune
      FA_PlayNote(523,100);
      FA_DelayMillis(100);
      FA_PlayNote(523,100);
      FA_DelayMillis(100);
      FA_PlayNote(523,100);
      FA_DelayMillis(100);
      FA_PlayNote(659,200);
      FA_DelayMillis(100);
    break;
  }
}

/**
  * Temp
*/
void newCell(){

  if(FA_BTConnected()){
    FA_BTSendString("Current Position (X Y): \n", 30);
    FA_BTSendNumber(currentPosX);
    FA_BTSendString("\n", 4);
    FA_BTSendNumber(currentPosY);
    FA_BTSendString("\n", 4);
    FA_BTSendString("Cells Visited: \n", 30);
    FA_BTSendNumber(noVisitedCells);
    FA_BTSendString("\n", 4);
    FA_BTSendString("Current Direction: \n", 30);
    FA_BTSendNumber(currentDirection);
    FA_BTSendString("\n", 4);
  }

  switch(currentDirection){
    case DIR_NORTH:
      currentPosY++;
    break;

    case DIR_EAST:
      currentPosX++;
    break;

    case DIR_SOUTH:
      currentPosY--;
    break;

    case DIR_WEST:
      currentPosX--;
    break;
  }

  currentCell = &maze[currentPosX][currentPosY];

  // Mark Current Cell as Visited
  currentCell->visited = true;
  // Increment number of visited cells
  noVisitedCells++;

  changeMainState(MAIN_DETECT);
}

/**
  * Initialize the application by starting various parts of the state machine
  * and loading the robot's API
*/
void initialize(){
  int x, y, i;

  // Initialize the API-specific things
  FA_RobotInit();
  FA_LCDBacklight(50);
  for(i = 0; i < 8; i++){
    FA_LEDOff(i);
  };

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

  // Set number of visited cells to 0
  noVisitedCells = 0;

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

  // Check if all cells have been visited (crawling finished)
  if(noVisitedCells > (SIZE_X * SIZE_Y)){
    changeMainState(MAIN_FINISH);
    return;
  }

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

  // Check if left turn is possible (PRIORITY #1)
  if(!(currentCell->walls[DIR_WEST])){
    // Turn left and update direction
    FA_Left(TURN_DEGREE);
    currentDirection = DIR_WEST;

    // Otherwise check if forward is possible (PRIORITY #2)
  }else if(!(currentCell->walls[DIR_NORTH])){
    // Update Direction
    currentDirection = DIR_NORTH;

    // Right turn possible? (PRIORITY #3)
  }else if(!(currentCell->walls[DIR_EAST])){
    // Turn right and update direction
    FA_Right(TURN_DEGREE);
    currentDirection = DIR_EAST;

    // If none of the above are possible, robot is in dead end and must turn around
  }else{
    FA_Right(180);
    // Change direction to opposite
    switch(currentDirection){
      case DIR_NORTH:
        currentDirection = DIR_SOUTH;
      break;

      case DIR_EAST:
        currentDirection = DIR_WEST;
      break;

      case DIR_SOUTH:
        currentDirection = DIR_NORTH;
      break;

      case DIR_WEST:
        currentDirection = DIR_EAST;
      break;
    }
  }

  FA_DelayMillis(500);

  // Change state to drive out of cell
  changeMainState(MAIN_DRIVE);
}

/**
  * Implements the cruising aspect of the robot and it's basic reactive behaviour
  * Whilst simultaniously modelling the maze and using the FSM.
  * Constantly check for cell changes then enact the correct behaviour
*/
void drive(){

  if(FA_ReadLine(CHANNEL_LEFT) > CELL_LINE_THRESHOLD){
    // Stop driving and creep forward into cell
    // todo: replace with timer?
    FA_SetMotors(0, 0);
    FA_Forwards(140);
    FA_PlayNote(300, 100);
    newCell();
  }
}


/**
  * The robot's behaviour when it has finished crawling the maze
  * (Some LED flashes and a buzzer sound)
*/
void finish(){

  // Wait for button press to restart the crawler
  if(FA_ReadSwitch(0) > 0 || FA_ReadSwitch(1) > 0){
    changeMainState(MAIN_START);
  }
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
        finish();
      break;

      case MAIN_ERROR:
        // Do some error handling
      break;

      default:
        puts("Error");
      // todo: error handling
    }
  }

  return 0;
}
