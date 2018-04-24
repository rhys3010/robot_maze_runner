/**
  * 'Maze Runner' hybrid robot controller
  * Explore a maze using a combination of reactive and deliberative behaviours, using
  * FSMs to implement a left wall following system and detect/store information about given cells.
  * @author Rhys Evans (rhe24@aber.ac.uk)
  * @version 0.3
*/

#define DEBUG

// Preprocessor constants for directions
#define DIR_NORTH       0
#define DIR_EAST        1
#define DIR_SOUTH       2
#define DIR_WEST        3

// Size of the maze (4x4 by default)
#define SIZE_X          4
#define SIZE_Y          4

// Preprocessor constants for the Infra Red detectors
#define IR_LEFT         0
#define IR_FRONT_LEFT   1
#define IR_FRONT        2
#define IR_FRONT_RIGHT  3
#define IR_RIGHT        4
#define IR_REAR_RIGHT   5
#define IR_REAR         6
#define IR_REAR_LEFT    7

/**
  * SYSTEM CONSTANTS
*/
// The IR reading distance to detect a wall
const int WALL_DIST_THRESHOLD = 100;
// The light level reading to mark cell as nest
const int LIGHT_LEVEL_THRESHOLD = 0;
// The line reading to mark a new cell
const int CELL_LINE_THRESHOLD = 50;
const int MOTOR_SPEED = 10;
const int TURN_DEGREE = 90;
const int CRASH_THRESHOLD = 700;

/**
  * SYSTEM INFO VARIABLES
*/
int currentDirection = DIR_NORTH;
// 1 as default
int currentPosX = 1;
int currentPosY = 0;
// The number of cells that the robot has visited
int noVisitedCells = 0;

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
  MAIN_DEBUG,
} MainState;


// The state machine variables
MainState mainState;

// State machine functions
void changeMainState(MainState newState);

/**
  * MAZE MODEL
  * The maze is represented by a 4x4 array of cells
  * The cells are structs with various attributes that need to be stored
*/

typedef struct{
  // Store wether or not the cell has been previously visited
  bool visited;
  // Store the status of the walls for each cell (NORTH, EAST, SOUTH, WEST) - 0,1,2 and 3rd element of the Array
  bool walls[4];
} Cell;

// Create the 2D maze array and store pointers to the current cell
Cell maze[SIZE_X][SIZE_Y];

// The current cell of the robot and the nest cell (default null)
Cell *currentCell;
Cell *nestCell;


/**
  * BEHAVIOURAL FUNCTIONS
*/
void initialize();
void detect();
void drive();
void turn();
void end();

/**
  * Utility Functions
*/
void avoidObstacle();
void newCellEntered();
