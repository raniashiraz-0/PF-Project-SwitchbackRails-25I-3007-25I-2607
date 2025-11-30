#include "trains.h"
#include "simulation_state.h"
#include "grid.h"
#include "switches.h"
#include <cstdlib>

// ============================================================================
// TRAINS.CPP - Train logic
// ============================================================================

// Storage for planned moves (for collisions).
PlannedMove plannedMoves[MAX_TRAINS];
int plannedMoveCount = 0;

// Previous positions (to detect switch entry).
int prevTrainX[MAX_TRAINS];
int prevTrainY[MAX_TRAINS];

// ----------------------------------------------------------------------------
// SPAWN TRAINS FOR CURRENT TICK
// ----------------------------------------------------------------------------
// Activate trains scheduled for this tick.
// ----------------------------------------------------------------------------
void spawnTrainsForTick() {
 for (int i = 0; i < activeTrainCount; i++) {
        if (trains[i].spawnTick == currentTick && trains[i].state == WAITING) {
            int x = trains[i].x;
            int y = trains[i].y;
            
            // Check if spawn tile is free
            if (!isTileBlocked(x, y)) {
                trains[i].state = MOVING;
            } else {
                // Spawn tile occupied - retry next tick
                trains[i].spawnTick++;
            }
        }
    }
}


// ----------------------------------------------------------------------------
// DETERMINE NEXT POSITION for a train
// ----------------------------------------------------------------------------
// Compute next position/direction from current tile and rules.
// ----------------------------------------------------------------------------
bool determineNextPosition(int trainID) {
if (trainID < 0 || trainID >= activeTrainCount) return false;
    Train& train = trains[trainID];
    
    if (train.state != MOVING) return false;
    
    // Skip movement if delayed by safety buffer
    if (train.waitTicks > 0) {
        train.waitTicks--;
        return false;
    }
      // Calculate next position based on current direction
    int nextX = train.x;
    int nextY = train.y;
    
    switch (train.direction) {
        case UP:    nextY--; break;
        case RIGHT: nextX++; break;
        case DOWN:  nextY++; break;
        case LEFT:  nextX--; break;
    }
    // Validate the move
    if (!isValidPosition(nextX, nextY)) return false;
    if (isTileBlocked(nextX, nextY)) return false;
    
    char nextTile = getTile(nextX, nextY);
    if (nextTile == '.') return false; // Cannot move to empty space
    
      // Get new direction based on track type
    Direction newDirection = getNextDirection(trainID, nextX, nextY);
    
    // Store planned move with distance-based priority info
    plannedMoves[plannedMoveCount] = {
        trainID,
        train.x, train.y,
        nextX, nextY,
        newDirection,
        calculateManhattanDistance(nextX, nextY, train.destinationX, train.destinationY)
    };
    plannedMoveCount++;
    
    return true;
}


// ----------------------------------------------------------------------------
// GET NEXT DIRECTION based on current tile and direction
// ----------------------------------------------------------------------------
// Return new direction after entering the tile.
// ----------------------------------------------------------------------------
int getNextDirection(int trainID, int nextX, int nextY
) {
Train& train = trains[trainID];
    char nextTile = getTile(nextX, nextY);
    Direction currentDir = train.direction;
    
    // Handle different track types
    switch (nextTile) {
        case '-': // Horizontal track
            return (currentDir == LEFT || currentDir == RIGHT) ? currentDir : currentDir;
            
        case '|': // Vertical track
            return (currentDir == UP || currentDir == DOWN) ? currentDir : currentDir;
            case '/': // Curve
            if (currentDir == RIGHT) return UP;
            if (currentDir == DOWN) return LEFT;
            if (currentDir == LEFT) return DOWN;
            if (currentDir == UP) return RIGHT;
            break;
            
        case '\\': // Curve
            if (currentDir == RIGHT) return DOWN;
            if (currentDir == UP) return LEFT;
            if (currentDir == LEFT) return UP;
            if (currentDir == DOWN) return RIGHT;
            break;
            
        case '+': // Crossing - use smart routing
            return getSmartDirectionAtCrossing(trainID);
            
             default:
            // For switches and other tiles, maintain current direction
            if (nextTile >= 'A' && nextTile <= 'Z') {
                return currentDir;
            }
            break;
    }
    
    return currentDir; // Default: maintain direction
}

            
            


// ----------------------------------------------------------------------------
// SMART ROUTING AT CROSSING - Route train to its matched destination
// ----------------------------------------------------------------------------
// Choose best direction at '+' toward destination.
// ----------------------------------------------------------------------------
int getSmartDirectionAtCrossing(int trainID) {
Train& train = trains[trainID];
    Direction bestDirection = train.direction;
    int bestDistance = calculateManhattanDistance(train.x, train.y, train.destinationX, train.destinationY);
    
    // Test all possible directions from crossing
    Direction testDirections[] = {UP, RIGHT, DOWN, LEFT};
    
    for (Direction dir : testDirections) {
        int testX = train.x;
        int testY = train.y;
        
         // Calculate test position
        switch (dir) {
            case UP:    testY--; break;
            case RIGHT: testX++; break;
            case DOWN:  testY++; break;
            case LEFT:  testX--; break;
        }
        
        // Check if this direction is valid and better
        if (isValidPosition(testX, testY) && !isTileBlocked(testX, testY)) {
            char testTile = getTile(testX, testY);
            if (testTile != '.') {
                int distance = calculateManhattanDistance(testX, testY, train.destinationX, train.destinationY);
                 if (distance < bestDistance) {
                    bestDistance = distance;
                    bestDirection = dir;
                }
            }
        }
    }
    
    return bestDirection;
}



// ----------------------------------------------------------------------------
// DETERMINE ALL ROUTES (PHASE 2)
// ----------------------------------------------------------------------------
// Fill next positions/directions for all trains.
// ----------------------------------------------------------------------------
void determineAllRoutes() {
    plannedMoveCount = 0;
    
    for (int i = 0; i < activeTrainCount; i++) {
        if (trains[i].state == MOVING) {
            // Store current position for switch entry detection
            prevTrainX[i] = trains[i].x;
            prevTrainY[i] = trains[i].y;
            
            // Determine next move for this train
            determineNextPosition(i);
        }
    }
}



// ----------------------------------------------------------------------------
// MOVE ALL TRAINS (PHASE 5)
// ----------------------------------------------------------------------------
// Move trains; resolve collisions and apply effects.
// ----------------------------------------------------------------------------
void moveAllTrains() {
// Resolve collisions first (distance-based priority system)
    detectCollisions();
    
    // Execute all valid moves
    for (int i = 0; i < plannedMoveCount; i++) {
        PlannedMove& move = plannedMoves[i];
        Train& train = trains[move.trainID];
        
        if (train.state != CRASHED) {
            // Update switch counters if entering a switch
            char currentTile = getTile(move.toX, move.toY);
            if (currentTile >= 'A' && currentTile <= 'Z') {
                updateSwitchCounters(move.trainID, prevTrainX[move.trainID], prevTrainY[move.trainID]);
            }
              // Apply movement
            train.x = move.toX;
            train.y = move.toY;
            train.direction = move.newDirection;
            
            // Handle safety buffer delay
            if (currentTile == '=') {
                train.onSafetyBuffer = true;
                train.waitTicks = 1; // One tick delay
            }
        }
    }
    
     // Apply emergency halt if active
    if (emergencyHaltActive) {
        applyEmergencyHalt();
    }
}

    


// ----------------------------------------------------------------------------
// DETECT COLLISIONS WITH PRIORITY SYSTEM
// ----------------------------------------------------------------------------
// Resolve same-tile, swap, and crossing conflicts.
// ----------------------------------------------------------------------------
void detectCollisions() {
// Check for same-tile collisions
    for (int i = 0; i < plannedMoveCount; i++) {
        for (int j = i + 1; j < plannedMoveCount; j++) {
            PlannedMove& move1 = plannedMoves[i];
            PlannedMove& move2 = plannedMoves[j];
            
            // Same destination collision
            if (move1.toX == move2.toX && move1.toY == move2.toY) {
                resolveSameTileCollision(move1, move2);
            }
            
            // Head-on swap collision
            if (move1.fromX == move2.toX && move1.fromY == move2.toY &&
                move1.toX == move2.fromX && move1.toY == move2.fromY) {
                resolveSwapCollision(move1, move2);
            }
        }
    }
}


// ----------------------------------------------------------------------------
// CHECK ARRIVALS
// ----------------------------------------------------------------------------
// Mark trains that reached destinations.
// ----------------------------------------------------------------------------
void checkArrivals() {
for (int i = 0; i < activeTrainCount; i++) {
        Train& train = trains[i];
        if (train.state == MOVING && 
            train.x == train.destinationX && 
            train.y == train.destinationY) {
            train.state = ARRIVED;
        }
    }
}


// ----------------------------------------------------------------------------
// APPLY EMERGENCY HALT
// ----------------------------------------------------------------------------
// Apply halt to trains in the active zone.
// ----------------------------------------------------------------------------
void applyEmergencyHalt() {
 if (!emergencyHaltActive) return;
    
    for (int i = 0; i < activeTrainCount; i++) {
        Train& train = trains[i];
        if (train.state == MOVING) {
            // Check if train is in 3x3 halt zone
            if (abs(train.x - emergencyHaltX) <= 1 && 
                abs(train.y - emergencyHaltY) <= 1) {
                train.waitTicks = 3; // Halt for 3 ticks
            }
        }
    }
}


// ----------------------------------------------------------------------------
// UPDATE EMERGENCY HALT
// ----------------------------------------------------------------------------
// Decrement timer and disable when done.
// ----------------------------------------------------------------------------
void updateEmergencyHalt() {
 if (emergencyHaltActive) {
        emergencyHaltTimer--;
        if (emergencyHaltTimer <= 0) {
            emergencyHaltActive = false;
            emergencyHaltX = -1;
            emergencyHaltY = -1;
        }
    }
}
