#include "grid.h"
#include "simulation_state.h"

// ============================================================================
// GRID.CPP - Grid utilities
// ============================================================================

// ----------------------------------------------------------------------------
// Check if a position is inside the grid.
// ----------------------------------------------------------------------------
// Returns true if x,y are within bounds.
// ----------------------------------------------------------------------------
bool isInBounds(int x, int y, const SimulationState& state) 
{
	return x>=0 && x<g_col && y>=0 && y<g_row
}

// ----------------------------------------------------------------------------
// Check if a tile is a track tile.
// ----------------------------------------------------------------------------
// Returns true if the tile can be traversed by trains.
// ----------------------------------------------------------------------------
bool isTrackTile(char tile) {
	if (tile =='+' || tile =='-' || tile =='/' || tile =='\\' || tile=='|'|| tile == '=')
		return true;
	if (tile >= '1' && tile <='9')
		return true;
	if (tile == 'G' || tile =='R' || tile =='Y' || tile =='D' || tile =='S')
		return true;
	return false;	
}

// ----------------------------------------------------------------------------
// Check if a tile is a switch.
// ----------------------------------------------------------------------------
// Returns true if the tile is 'A'..'Z'.
// ----------------------------------------------------------------------------
bool isSwitchTile(char tile) {
	if (tile >= 'A' && tile<= 'Z')
		return true;
}
;
// ----------------------------------------------------------------------------
// Get switch index from character.
// ----------------------------------------------------------------------------
// Maps 'A'..'Z' to 0..25, else -1.
// ----------------------------------------------------------------------------
int getSwitchIndex(char tile) {
	if (tile >= 'A' && tile<= 'Z')
		return tile -'A';
	return -1;
}

// ----------------------------------------------------------------------------
// Check if a position is a spawn point.
// ----------------------------------------------------------------------------
// Returns true if x,y is a spawn.
// ----------------------------------------------------------------------------
bool isSpawnPoint(int x, int y, const SimulationState& state) {
	if (isInBounds(int x, int y, const SimulationState& state) == true)
		if (state.grid[x][y] =='S')
			return true;
		return false;	
	return false;
	 
}

// ----------------------------------------------------------------------------
// Check if a position is a destination.
// ----------------------------------------------------------------------------
// Returns true if x,y is a destination.
// ----------------------------------------------------------------------------
bool isDestinationPoint(int x, int y, const SimulationState& state) {
	if (isInBounds(int x, int y, const SimulationState& state) == true)
		if (state.grid[x][y] =='D')
			return true;
		return false;	
	return false;
}

// ----------------------------------------------------------------------------
// Toggle a safety tile.
// ----------------------------------------------------------------------------
// Returns true if toggled successfully.
// ----------------------------------------------------------------------------
bool toggleSafetyTile() {
	if (isInBounds(int x, int y, const SimulationState& state) != true)
	{		
		&char CurrentTile = state.grid[x][y];
		if (CurrentTile == '=')
		{
			CurrentTile = '-';
			return true;
		}
		else if (CurrentTile != 'S' && CurrentTile != 'D' && isTrackTile(CurrentTile))
		{
			CurrentTile = '=';
			return true;
		}
		return false;
	}
}
