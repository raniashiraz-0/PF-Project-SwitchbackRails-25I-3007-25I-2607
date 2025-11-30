
#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

// ============================================================================
// SIMULATION_STATE.H - Global constants and state
// ============================================================================
// Global constants and arrays used by the game.
// ============================================================================
extern char g_weather;
const char WeatherNormal = 'N';
const char WeatherRain = 'R';
const char WeatherFog = 'F';
extern char g_direction;
const char DirectionNorth = 'N';
const char DirectionSouth = 'S';
const char DirectionEast = 'E';
const char DirectionWest = 'W';
extern char g_trainstate[Max_Trains];
const char TrainWaiting = 'W';
const char TrainMoving = 'M';
const char TrainReached = 'R';
const char TrainCrashed = 'C';
extern g_switchmode[Max_Trains];
const char ModePerDirection = 'P';
const char ModeCount ='C';
extern g_SignalColor;
const char SignalGreen = 'G';
const char SignalRed = 'R';
const char SignalYellow = 'Y';
// ----------------------------------------------------------------------------
// GRID CONSTANTS
// ----------------------------------------------------------------------------
const int MaxRows =100;
const int MaxCols=100;
const int TileCount =32;

// ----------------------------------------------------------------------------
// TRAIN CONSTANTS
// ----------------------------------------------------------------------------
const int MaxTrains=50;
const int MaxColors =5;

// ----------------------------------------------------------------------------
// SWITCH CONSTANTS
// ----------------------------------------------------------------------------
const int MaxSwitches =26;
const int DirectionCount=4;

// ----------------------------------------------------------------------------
// WEATHER CONSTANTS
// ----------------------------------------------------------------------------
const int RainSlow =5;
const int FogDelay =2;

// ----------------------------------------------------------------------------
// SIGNAL CONSTANTS
// ----------------------------------------------------------------------------
const int SafeDistance =3;
const int WarnDistance= 2;



// ----------------------------------------------------------------------------
// GLOBAL STATE: GRID
// ----------------------------------------------------------------------------
extern int g_gridrows;
extern int g_gridcol;
extern char g_gridtiles[MaxRows][MaxCols];

// ----------------------------------------------------------------------------
// GLOBAL STATE: TRAINS
// ----------------------------------------------------------------------------
extern int g_traincount;
extern int g_trainid[Max_Trains];
extern int g_trainX[Max_Trains];
extern int g_trainY[Max_Trains];
extern int g_trainspawntick[Max_Trains];
extern int g_traindestinationx[Max_Trains];
extern int g_traindestinationy[Max_Trains];
extern int g_traincolor[Max_Trains];
extern int g_trainmovementinrain[Max_Trains];
extern bool g_trainhalt[Max_Trains];


// ----------------------------------------------------------------------------
// GLOBAL STATE: SWITCHES (A-Z mapped to 0-25)
// ----------------------------------------------------------------------------
extern int g_switchcount;
extern char g_switchletter[MaxSwitches];
extern SwitchMode g_switchmodes[MaxSwitches];
extern int g_switchinit[MaxSwitches];
extern int g_switchtrainentercount[MaxSwitches][DirectionCount];
extern int g_kcount[MaxSwitches][DirectionCount];
extern bool g_switchtoflip[MaxSwitches];
extern int g_switchflipcount[MaxSwitches];
extern char g_switchstate0[MaxSwitches];
extern char g_switchstate1[MaxSwitches];	
// ----------------------------------------------------------------------------
// GLOBAL STATE: SPAWN POINTS
// ----------------------------------------------------------------------------
extern int g_spawncount;
extern int g_spawnx[Max_Trains];
extern int g_spawny[Max_Trains];
// ----------------------------------------------------------------------------
// GLOBAL STATE: DESTINATION POINTS
// ----------------------------------------------------------------------------
extern int g_destinationcount;
extern int g_destinationx[Max_Trains];
extern int g_destinationy[Max_Trains];

// ----------------------------------------------------------------------------
// GLOBAL STATE: SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
extern int g_currenttick;
extern int g_seed;
extern WeatherType g_weather;
extern bool g_simulationrunning;
extern bool g_simulationpaused;

// ----------------------------------------------------------------------------
// GLOBAL STATE: METRICS
// ----------------------------------------------------------------------------
extern int g_metricsTotalTicks;
extern int g_metricsTrainsDelivered;
extern int g_metricsTrainsCrashed;

// ----------------------------------------------------------------------------
// GLOBAL STATE: EMERGENCY HALT
// ----------------------------------------------------------------------------
extern bool g_emergencyhaltactive;
extern int g_emergencyhalt_remainingticks;
extern int g_metricsttlswitchflips;
extern int g_metricstotalsafetybuffersused;

// ----------------------------------------------------------------------------
// INITIALIZATION FUNCTION
// ----------------------------------------------------------------------------
// Resets all state before loading a new level.
void initializeSimulationState();

#endif
