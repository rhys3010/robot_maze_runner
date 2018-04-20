/**
  * 'Maze Runner' hybrid robot controller
  * Explore a maze using a combination of reactive and deliberative behaviours, using
  * FSMs to implement a left wall following system and detect/store information about given cells.
  * @author Rhys Evans (rhe24@aber.ac.uk)
  * @version 0.1
*/

// Preprocessor constants for directions
#define DIR_NORTH 0
#define DIR_EAST  1
#define DIR_SOUTH 2
#define DIR_WEST  3

// Size of the maze (4x4 by default)
#define SIZE_X    4
#define SIZE_Y    4

/**
  * SYSTEM CONSTANTS
*/
const int WALL_DIST_THRESHOLD = 0;
const int MOTOR_SPEED = 0;

/**
  * INFO VARIABLES
*/
int currentDirection = DIR_NORTH;

/**
  * STATE MACHINE SETUP
*/
// The 5 possible states for the primary FSM
typedef enum{
  MAIN_START,
  MAIN_DETECT,
  MAIN_TURN,
  MAIN_DRIVE,
  MAIN_FINISH,
} MainState;

// The possible states for the Obstacle Detection FSM
typedef enum{
  DETECT_START,
  DETECT_TURN_LEFT,
  DETECT_CHECK_LEFT,
  DETECT_TURN_RIGHT,
  DETECT_CHECK_RIGHT,
  DETECT_FINISH,
} DetectState;

// The state machine variables
MainState mainState;
DetectState detectState;

// State machine functions
void changeMainState(MainState newState);
void changeDetectState(DetectState newState);

/**
  * MAZE MODEL
  * The maze is represented by a 4x4 array of cells
  * The cells are structs with various attributes that need to be stored
*/

typedef struct{
  // Store wether or not the cell is dark
  bool dark;
  // Store wether or not the cell has been previously visited
  bool visited;
  // Store the status of the walls for each cell (NORTH, EAST, SOUTH, WEST) - 0,1,2 and 3rd element of the Array
  bool walls[4];
} Cell;

// Create the 2D maze array and store pointers to the current cell
Cell maze[SIZE_X][SIZE_Y];

Cell *currentCell;
