#include "simulation_state.h"
#include <cstring>

// ============================================================================
// SIMULATION_STATE.CPP - Global state definitions
// ============================================================================

// ----------------------------------------------------------------------------
// GRID
// ----------------------------------------------------------------------------
 int g_gridrows=0;
int g_gridcol=0;
char g_gridtiles[MaxRows][MaxCols]= {{0}};
// ----------------------------------------------------------------------------
// TRAINS
// ----------------------------------------------------------------------------
 int g_traincount=0;
 int g_trainX[Max_Trains]={0};
 int g_trainY[Max_Trains]={0};
 int g_trainspawntick[Max_Trains]={0};
 int g_traindestinationx[Max_Trains]={0};
 int g_traindestinationy[Max_Trains] ={0};
 int g_traincolor[Max_Trains] ={0};
 int g_trainmovementinrain[Max_Trains]={0};
 bool g_trainhalt[Max_Trains]={0};

// ----------------------------------------------------------------------------
// SWITCHES
// ----------------------------------------------------------------------------
 int g_switchcoumt =0;
 char g_switchletter[MaxSwitches] = {0};
 SwitchMode g_switchmodes[MaxSwitches]= {ModePerDirection};
 int g_switchinit[MaxSwitches]= {0};
 int g_switchcount[MaxSwitches][DirectionCount] ={{0}};
 int g_kcount[MaxSwitches][DirectionCount]= {{0}};
 bool g_switchtoflip[MaxSwitches]={false};
 int g_switchflipcount[MaxSwitches]={0};
 char g_switchstate0[MaxSwitches]={0};
 char g_switchstate1[MaxSwitches]={0};	
// ----------------------------------------------------------------------------
// SPAWN AND DESTINATION POINTS
// ----------------------------------------------------------------------------
 int g_spawncount=0;
 int g_spawnx[Max_Trains]={0};
 int g_spawny[Max_Trains]={0};
 int g_destinationcount=0;
 int g_destinationx[Max_Trains]={0};
 int g_destinationy[Max_Trains]={0};
// ----------------------------------------------------------------------------
// SIMULATION PARAMETERS
// ----------------------------------------------------------------------------
int g_currenttick=0;
 int g_seed=0;
WeatherType g_weather=WeatherNormal;
bool g_simulationrunning=false;
 bool g_simulationpaused=false;
// ----------------------------------------------------------------------------
// METRICS
// ----------------------------------------------------------------------------
 int g_metricsTotalTicks=0;
 int g_metricsTrainsDelivered=0;
  int g_metricsTrainsCrashed=0;

// ----------------------------------------------------------------------------
// EMERGENCY HALT
// ----------------------------------------------------------------------------
 bool g_emergencyhaltactive=false;
 int g_emergencyhalt_remainingticks=0;
 int g_metricsttlswitchflips=0;
 int g_metricstotalsafetybuffersused=0;
// ============================================================================
// INITIALIZE SIMULATION STATE
// ============================================================================
// ----------------------------------------------------------------------------
// Resets all global simulation state.
// ----------------------------------------------------------------------------
// Called before loading a new level.
// ----------------------------------------------------------------------------
void initializeSimulationState() {
}
