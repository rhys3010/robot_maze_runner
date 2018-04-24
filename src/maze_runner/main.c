/**
  * 'Maze Runner' hybrid robot controller
  * Explore a maze using a combination of reactive and deliberative behaviours, using
  * FSMs to implement a left wall following system and detect/store information about given cells.
  * @author Rhys Evans (rhe24@aber.ac.uk)
  * @version 0.3
*/
#include "allcode_api.h"
#include "main.h"


/**
  * Change the system's main state and perform necessary actions
*/
void changeMainState(MainState newState){
  FA_DelayMillis(500);
  mainState = newState;
  int i;

  // State first-time entry behaviour
  // Code that only needs to be executed once on entry into the state
  switch(mainState){

    case MAIN_DRIVE:
      FA_SetMotors(MOTOR_SPEED, MOTOR_SPEED);
    break;

    case MAIN_DETECT:
    break;

    case MAIN_FINISH:
      FA_SetMotors(0, 0);

      // Turn on all front LEDs
      for(i = 0; i < 8; i++){
        FA_LEDOn(i);
      }

      // Play a quick tune once
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
  * Implements the reactive behaviour of the robot to correct its position when turning and entering a new cell
*/
void avoidObstacle(){
  if(FA_ReadIR(IR_FRONT) > CRASH_THRESHOLD){
  }

  if(FA_ReadIR(IR_LEFT) > CRASH_THRESHOLD){
    FA_Right(5);
  }

  if(FA_ReadIR(IR_RIGHT) > CRASH_THRESHOLD){
    FA_Left(5);
  }
}

/**
  * The operations to perform when the buggy enters a new cell
  * Print debug information to bluetooth console
  * Update current position
  * Update number of cells seen
*/
void newCellEntered(){

#ifdef DEBUG

  // Log debug info to bluetooth console
  if(FA_BTConnected()){
    FA_BTSendString("============================ \n", 30);
    FA_BTSendString("Current Position (X Y): \n", 30);
    FA_BTSendNumber(currentPosX);
    FA_BTSendString(" , ", 4);
    FA_BTSendNumber(currentPosY);
    FA_BTSendString("\n", 4);
    FA_BTSendString("Cells Visited: ", 30);
    FA_BTSendNumber(noVisitedCells);
    FA_BTSendString("\n", 4);
    FA_BTSendString("Current Direction: ", 30);
    FA_BTSendNumber(currentDirection);
    FA_BTSendString("\n", 4);
    FA_BTSendString("---------------------------- \n", 30);
    FA_BTSendString("Walls: ", 30);
    FA_BTSendNumber(currentCell->walls[0]);
    FA_BTSendNumber(currentCell->walls[1]);
    FA_BTSendNumber(currentCell->walls[2]);
    FA_BTSendNumber(currentCell->walls[3]);
    FA_BTSendString("============================ \n", 30);

  }

#endif

  // Update position of the buggy based on its direction of travel
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

  // Update visited flag and increment visited cells
  if(!currentCell->visited){
    currentCell->visited = true;
    noVisitedCells++;
  }

  changeMainState(MAIN_DETECT);
}

/**
  * Draw and update a representation of the maze to the
  * Buggy's LCD screen
  * Maze is represented by a 28x28 grid of 7x7 cells
*/
void drawMaze(){
  int x, y, i;
  FA_LCDClear();

  // Draw Maze Border
  for(x = 0; x < MAZE_DRAW_WIDTH; x++){
    for(y = 0; y < MAZE_DRAW_LENGTH; y++){
      // Draw Maze Border
      FA_LCDPlot(x, 0);
      FA_LCDPlot(x, MAZE_DRAW_WIDTH);
      FA_LCDPlot(0, y);
      FA_LCDPlot(MAZE_DRAW_LENGTH, y);
    }
  }

  /**
    * Iterate over the 2d maze array and draw all the walls of each cell
    * The logic below is a bit in-depth, due to the fact that the LCD screen draws with 0,0
    * at the top left, but my maze array has 0,0 as bottom left.
  */
  for(x = 0; x < SIZE_X; x++){
    for(y = 0; y < SIZE_Y; y++){

      // DRAW ROBOT POSITION
      if(x == currentPosX && y == currentPosY){
        FA_LCDPlot((x * MAZE_DRAW_CELL_WIDTH) + 3, (MAZE_DRAW_CELL_LENGTH * ((SIZE_Y - y) - 1) + 5));
      }

      // DRAW CELL WALLS
      for(i = (x * MAZE_DRAW_CELL_WIDTH); i < (x * MAZE_DRAW_CELL_WIDTH) + MAZE_DRAW_CELL_WIDTH; i++){
        if(maze[x][y].walls[DIR_NORTH]){
          FA_LCDPlot(i, MAZE_DRAW_CELL_LENGTH * ((SIZE_Y - y) - 1));
        }

        if(maze[x][y].walls[DIR_SOUTH]){
          FA_LCDPlot(i, MAZE_DRAW_CELL_LENGTH * (SIZE_Y - y));
        }
      }

      for(i = (MAZE_DRAW_CELL_LENGTH * ((SIZE_Y - y) - 1)); i < (MAZE_DRAW_CELL_LENGTH * (SIZE_Y - y)); i++){
        if(maze[x][y].walls[DIR_EAST]){
          FA_LCDPlot((x * MAZE_DRAW_CELL_WIDTH) + MAZE_DRAW_CELL_WIDTH, i);
        }

        if(maze[x][y].walls[DIR_WEST]){
          FA_LCDPlot((x * MAZE_DRAW_CELL_WIDTH), i);
        }
      }
    }
  }
}

/**
  * Initialize the application by starting various parts of the state machine
  * and loading the robot's API
*/
void initialize(){
  int x, y, i;

  // Initialize the API-specific things
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


  // Set number of visited cells to 0
  noVisitedCells = 0;

  // Set the current cell pointer to the current position
  currentCell = &maze[currentPosX][currentPosY];


  // Move to the next state...
  changeMainState(MAIN_DEBUG);
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
    FA_Left(TURN_DEGREE-3);
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
    FA_Right(TURN_DEGREE*2);
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

  // Change state to drive out of cell
  changeMainState(MAIN_DRIVE);
}

/**
  * Implements the cruising aspect of the robot and it's basic reactive behaviour
  * Whilst simultaniously modelling the maze and using the FSM.
  * Constantly check for cell changes then enact the correct behaviour
*/
void drive(){
  if(FA_ReadLine(CHANNEL_LEFT) < CELL_LINE_THRESHOLD || FA_ReadLine(CHANNEL_RIGHT) < CELL_LINE_THRESHOLD){
    FA_SetMotors(0, 0);
    FA_Forwards(100);
    newCellEntered();
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
  // Intialize the API one time only
  FA_RobotInit();
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

      case MAIN_DEBUG:
        currentPosX = 3;
        currentPosY = 3;

        maze[0][0].walls[DIR_EAST] = true;
        maze[1][0].walls[DIR_WEST] = true;
        maze[2][0].walls[DIR_EAST] = true;
        maze[2][0].walls[DIR_NORTH] = true;
        maze[3][0].walls[DIR_WEST] = true;

        maze[1][1].walls[DIR_NORTH] = true;
        maze[1][1].walls[DIR_EAST] = true;

        maze[2][1].walls[DIR_NORTH] = true;
        maze[2][1].walls[DIR_WEST] = true;
        maze[2][1].walls[DIR_SOUTH] = true;

        maze[0][2].walls[DIR_NORTH] = true;

        maze[1][2].walls[DIR_NORTH] = true;
        maze[1][2].walls[DIR_SOUTH] = true;

        maze[2][2].walls[DIR_SOUTH] = true;
        maze[2][2].walls[DIR_EAST] = true;

        maze[3][2].walls[DIR_WEST] = true;

        maze[0][3].walls[DIR_SOUTH] = true;

        maze[1][3].walls[DIR_SOUTH] = true;

        FA_DelayMillis(1000);
        drawMaze();

      break;
    }
  }

  return 0;
}
