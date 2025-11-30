#include "switches.h"
#include "simulation_state.h"
#include "grid.h"
#include "io.h"

// ============================================================================
// SWITCHES.CPP - Switch management
// ============================================================================

// ----------------------------------------------------------------------------
// UPDATE SWITCH COUNTERS
// ----------------------------------------------------------------------------
// Increment counters for trains entering switches.
// ----------------------------------------------------------------------------
void updateSwitchCounters(int trainID, int entryX, int entryY) {
if (trainID < 0 || trainID >= activeTrainCount) return;
    
    char switchTile = getTile(entryX, entryY);
    if (switchTile < 'A' || switchTile > 'Z') return;
    
    // Find the switch
    int switchIndex = -1;
    for (int i = 0; i < switchCount; i++) {
        if (switches[i].letter == switchTile) {
            switchIndex = i;
            break;
        }
}
 if (switchIndex == -1) return;
    
    Switch& sw = switches[switchIndex];
    Train& train = trains[trainID];
    
    if (sw.mode == GLOBAL) {
        // Global mode - use single counter for all directions
        sw.globalCounter++;
        if (sw.globalCounter >= sw.kValues[0]) {
            sw.queuedForFlip = true;
        }
    } else if (sw.mode == PER_DIR) {
        // Per-direction mode - increment counter for specific direction
        
        int dirIndex = static_cast<int>(train.direction);
        sw.counters[dirIndex]++;
        if (sw.counters[dirIndex] >= sw.kValues[dirIndex]) {
            sw.queuedForFlip = true;
        }
    }
}

// ----------------------------------------------------------------------------
// QUEUE SWITCH FLIPS
// ----------------------------------------------------------------------------
// Queue flips when counters hit K.
// ----------------------------------------------------------------------------
void queueSwitchFlips() {
for (int i = 0; i < switchCount; i++) {
        Switch& sw = switches[i];
        
        if (sw.mode == GLOBAL) {
            if (sw.globalCounter >= sw.kValues[0]) {
                sw.queuedForFlip = true;
            }
        } else if (sw.mode == PER_DIR) {
            // Check if any direction counter reached K-value
            for (int dir = 0; dir < 4; dir++) {
                if (sw.counters[dir] >= sw.kValues[dir]) {
                    sw.queuedForFlip = true;
                    break;
                }
            }
        }
    }
}


// ----------------------------------------------------------------------------
// APPLY DEFERRED FLIPS
// ----------------------------------------------------------------------------
// Apply queued flips after movement.
// ----------------------------------------------------------------------------
void applyDeferredFlips() {
     for (int i = 0; i < switchCount; i++) {
        Switch& sw = switches[i];
        
        if (sw.queuedForFlip) {
            // Flip the switch state
            sw.currentState = 1 - sw.currentState; // Toggle between 0 and 1
            
            // Reset counters after flip (as recommended in spec)
            if (sw.mode == GLOBAL) {
                sw.globalCounter = 0;
            } else if (sw.mode == PER_DIR) {
                for (int dir = 0; dir < 4; dir++) {
                    sw.counters[dir] = 0;
                }
            }
             sw.queuedForFlip = false;
            
            // Log the switch flip
            cout << "Tick " << currentTick << ": Switch " << sw.letter 
                 << " flipped to " << sw.stateNames[sw.currentState] << endl;
        }
    }
}



// ----------------------------------------------------------------------------
// UPDATE SIGNAL LIGHTS
// ----------------------------------------------------------------------------
// Update signal colors for switches.
// ----------------------------------------------------------------------------
void updateSignalLights() {

 for (int i = 0; i < switchCount; i++) {
        Switch& sw = switches[i];
        
        // Check all switch tiles with this letter
        for (int y = 0; y < gridRows; y++) {
            for (int x = 0; x < gridCols; x++) {
                if (getTile(x, y) == sw.letter) {
                    // Determine signal color based on track conditions
                    SignalColor color = determineSignalColor(sw, x, y);
                    signalStates[i].x = x;
                    signalStates[i].y = y;
                    signalStates[i].color = color;
                    }
                }
        }
    }
}

// ----------------------------------------------------------------------------
// TOGGLE SWITCH STATE (Manual)
// ----------------------------------------------------------------------------
// Manually toggle a switch state.
// ----------------------------------------------------------------------------
void toggleSwitchState(char switchLetter) {

for (int i = 0; i < switchCount; i++) {
        if (switches[i].letter == switchLetter) {
            switches[i].currentState = 1 - switches[i].currentState;
            
            // Reset counters when manually toggled
            if (switches[i].mode == GLOBAL) {
                switches[i].globalCounter = 0;
            } else if (switches[i].mode == PER_DIR) {
                for (int dir = 0; dir < 4; dir++) {
                    switches[i].counters[dir] = 0;
                }
            }
            
              cout << "Switch " << switchLetter << " manually toggled to " 
                 << switches[i].stateNames[switches[i].currentState] << endl;
            return;
        }
    }
}


// ----------------------------------------------------------------------------
// GET SWITCH STATE FOR DIRECTION
// ----------------------------------------------------------------------------
// Return the state for a given direction.
// ----------------------------------------------------------------------------
int getSwitchStateForDirection(char switchLetter, Direction direction) {
for (int i = 0; i < switchCount; i++) {
        if (switches[i].letter == switchLetter) {
            return switches[i].currentState;
        }
    }
    return 0; // Default state
}


