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
g_gridrows=0;
g_gridcols=0;
for (int i=0;i<MaxRows;i++)
{
	for (int j=0;j<MaxCols;j++)
	{
		g_gridtiles[i][j] =" ";
	}
} 
int g_traincount =0;
int g_spawncount =0 ;
int g_destinationcount = 0;
for (int i =0;i<Max_Trains;i++)
{
	g_trainX[i]=-1;
	g_trainY[i] =-1;
	g_trainid[i] = i;
	g_trainspawntick[i]=0;
	g_traindestinationx[i] = 0;
	g_traindestinationy[i] =0;
	g_traincolor[i]=0;
	g_trainmovementinrain[i]= 0;
	g_trainhalt[i] =false;
	g_spawncountx[i]=-1;
	g_spawncounty[i]=-1;
	g_destinationx[i] = -1;
	g_destinationyy[i] = -1;
	
}
int g_switchcount =0;
for (int i=0;i<MaxSwitches;i++)
{
	g_switchletter[i] = 'A'+i;
	g_switchmode[i]=ModePerDirection;
	g_switchinit[i]=0;
	for (int j=0;j<DirectionCount; j++)
	{	
		g_switchtrainentercount[i][j]=0;
		g_kcount[i][j] =0;
	}
	g_switchtoflip[i]=false;
	g_switchfipcount[i]=0;
	g_switchstate0[i]=" ";
	g_switchstate1[i] = " ";
}
int g_currenttick=0;
int g_seed=0;
WeatherType g_weather = WeatherNormal;
bool g_simulationrunning = false;
bool g_simulationpaused=false;
int g_metricsTotalTicks =0;
int g_metricsTrainsDelivered =0 ;
int g_metricsTrainsCrashed= 0;
bool g_emergencyhaltactive =false ;
int g_emergencyhalt_remainingticks = 0;
int g_metricsttlswitchflips= 0 ;
int g_metricstotalsafetybuffersused = 0;
}
