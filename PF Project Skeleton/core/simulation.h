#ifndef SIMULATION_H
#define SIMULATION_H

// ============================================================================
// SIMULATION.H - Simulation tick logic
// ============================================================================
void SpawnTrains(int g_currenttick);                    	
int CalculateManhattanDistance(int x1, int y1, int x2, int y2); 
bool SolvePriority(int trainid, int destinationx, int destinationy); 
// ----------------------------------------------------------------------------
// MAIN SIMULATION FUNCTION
// ----------------------------------------------------------------------------
// Run one simulation tick.
void simulateOneTick(g_grid,g_trains [],g_traincount,g_switches[],g_switchcount, g_currenttick );

// ----------------------------------------------------------------------------
// INITIALIZATION
// ----------------------------------------------------------------------------
// Initialize the simulation after loading a level.
void initializeSimulation(g_grid,g_trains [],g_traincount,g_switches[],g_switchcount, g_currenttick );

// ----------------------------------------------------------------------------
// UTILITY
// ----------------------------------------------------------------------------
// True if all trains are delivered or crashed.
bool isSimulationComplete(g_trains [],g_traincount);
void UpdatingSingnalLight();
int GetSignalColor(int g_direction);
int WeatherAffect(g_weather);
void PauseSimulation();
void ContinueSimulation();
#endif

