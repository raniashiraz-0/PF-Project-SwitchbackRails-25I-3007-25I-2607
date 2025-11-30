#include "io.h"
#include "simulation_state.h"
#include "grid.h"
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

// ============================================================================
// IO.CPP - Level I/O and logging
// ============================================================================

// ----------------------------------------------------------------------------
// LOAD LEVEL FILE
// ----------------------------------------------------------------------------
// Load a .lvl file into global state.
// ----------------------------------------------------------------------------
bool loadLevelFile()const string& filename {
 ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open level file " << filename << endl;
        return false;
}
   string line;
    string section;
    
    while (getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        // Check for section headers
        if (line.back() == ':') {
            section = line.substr(0, line.length() - 1);
            continue;
        }
        
        // Parse based on current section
        if (section == "NAME") {
            levelName = line;
        }
        else if (section == "ROWS") {
            levelRows = stoi(line);
        }
        else if (section == "COLS") {
            levelCols = stoi(line);
        }
        else if (section == "SEED") {
            levelSeed = stoi(line);
            srand(levelSeed); // Seed the random number generator
        }
        else if (section == "WEATHER") {
            weatherType = line;
        }
        else if (section == "MAP") {
            // Parse grid layout
            // This would populate your grid data structure
            // For now, we'll assume grid is loaded elsewhere
        }
         else if (section == "SWITCHES") {
            // Parse switch definitions
            // Format: <Letter> <Mode> <Init> <K_UP> <K_RIGHT> <K_DOWN> <K_LEFT> <State0> <State1>
            istringstream iss(line);
            string token;
            vector<string> tokens;
            
            while (iss >> token) {
                tokens.push_back(token);
            }
            
              if (tokens.size() >= 9) {
                Switch sw;
                sw.letter = tokens[0][0];
                sw.mode = (tokens[1] == "PER_DIR") ? PER_DIR : GLOBAL;
                sw.initialState = stoi(tokens[2]);
                sw.currentState = sw.initialState;
                
                // Parse K-values
                sw.kValues[UP] = stoi(tokens[3]);
                sw.kValues[RIGHT] = stoi(tokens[4]);
                sw.kValues[DOWN] = stoi(tokens[5]);
                sw.kValues[LEFT] = stoi(tokens[6]);
                
                sw.stateNames[0] = tokens[7];
                sw.stateNames[1] = tokens[8];
                
                 // Initialize counters
                sw.globalCounter = 0;
                for (int i = 0; i < 4; i++) {
                    sw.counters[i] = 0;
                }
                sw.queuedForFlip = false;
                
                // Add to switches array
                if (switchCount < MAX_SWITCHES) {
                    switches[switchCount++] = sw;
                }
            }
        }
        
        else if (section == "TRAINS") {
            // Parse train schedule
            // Format: <tick> <x> <y> <direction> <color>
            istringstream iss(line);
            int tick, x, y, direction, color;
            
            if (iss >> tick >> x >> y >> direction >> color) {
                if (activeTrainCount < MAX_TRAINS) {
                    trains[activeTrainCount] = {
                        activeTrainCount,
                        x, y,
                        static_cast<Direction>(direction),
                        WAITING,
                        tick,
                        color,
                        -1, -1, // destination will be set based on track
                        
            0,
                        false
                    };
                    activeTrainCount++;
                }
            }
        }
    }
         file.close();
    cout << "Loaded level: " << levelName << " (" << levelRows << "x" << levelCols << ")" << endl;
    return true;
}

// ----------------------------------------------------------------------------
// INITIALIZE LOG FILES
// ----------------------------------------------------------------------------
// Create/clear CSV logs with headers.
// ----------------------------------------------------------------------------
void initializeLogFiles() {

 // Open trace.csv
    traceFile.open("out/trace.csv");
    traceFile << "Tick,TrainID,X,Y,Direction,State\n";
    
    // Open switches.csv  
    switchesFile.open("out/switches.csv");
    switchesFile << "Tick,Switch,Mode,State\n";
    
    // Open signals.csv
    signalsFile.open("out/signals.csv");
    signalsFile << "Tick,Switch,Signal\n";

 // Reset metrics
    trainsDelivered = 0;
    trainsCrashed = 0;
    totalWaitTicks = 0;
    totalTicks = 0;
    switchFlipsCount = 0;
    signalViolations = 0;
    safetyBuffersUsed = 0;
    
    cout << "Log files initialized in out/ directory" << endl;
}




// ----------------------------------------------------------------------------
// LOG TRAIN TRACE
// ----------------------------------------------------------------------------
// Append tick, train id, position, direction, state to trace.csv.
// ----------------------------------------------------------------------------
void logTrainTrace() {
 for (int i = 0; i < activeTrainCount; i++) {
        Train& train = trains[i];
        
        string stateStr;
        switch (train.state) {
            case WAITING: stateStr = "WAITING"; break;
            case MOVING: stateStr = "MOVING"; break;
            case ARRIVED: stateStr = "ARRIVED"; break;
            case CRASHED: stateStr = "CRASHED"; break;
            case DELAYED: stateStr = "DELAYED"; break;
        }
        
        
}  string dirStr;
        switch (train.direction) {
            case UP: dirStr = "UP"; break;
            case RIGHT: dirStr = "RIGHT"; break;
            case DOWN: dirStr = "DOWN"; break;
            case LEFT: dirStr = "LEFT"; break;
        }
        
         traceFile << currentTick << ","
                  << train.id << ","
                  << train.x << ","
                  << train.y << ","
                  << dirStr << ","
                  << stateStr << "\n";
    }
    traceFile.flush();
}

// ----------------------------------------------------------------------------
// LOG SWITCH STATE
// ----------------------------------------------------------------------------
// Append tick, switch id/mode/state to switches.csv.
// ----------------------------------------------------------------------------
void logSwitchState() {
for (int i = 0; i < switchCount; i++) {
        Switch& sw = switches[i];
        
        string modeStr = (sw.mode == PER_DIR) ? "PER_DIR" : "GLOBAL";
        string stateStr = sw.stateNames[sw.currentState];
        
        switchesFile << currentTick << ","
                     << sw.letter << ","
                     << modeStr << ","
                     << stateStr << "\n";
        
        // Count flips if switch was queued for flip
        if (sw.queuedForFlip) {
            switchFlipsCount++;
        }
    }
switchesFile.flush();
}



// ----------------------------------------------------------------------------
// LOG SIGNAL STATE
// ----------------------------------------------------------------------------
// Append tick, switch id, signal color to signals.csv.
// ----------------------------------------------------------------------------
void logSignalState() {

for (int i = 0; i < switchCount; i++) {
        Switch& sw = switches[i];
        
        string signalStr;
        switch (signalStates[i].color) {
            case RED: signalStr = "RED"; break;
            case YELLOW: signalStr = "YELLOW"; break;
            case GREEN: signalStr = "GREEN"; break;
        }
        
        signalsFile << currentTick << ","
                    << sw.letter << ","
                    << signalStr << "\n";
    }
    signalsFile.flush();
}

// ----------------------------------------------------------------------------
// WRITE FINAL METRICS
// ----------------------------------------------------------------------------
// Write summary metrics to metrics.txt.
// ----------------------------------------------------------------------------
void writeMetrics() {
 metricsFile.open("out/metrics.txt");

    metricsFile << "Level: " << levelName << "\n";
    metricsFile << "Grid Size: " << levelRows << " x " << levelCols << "\n";
    metricsFile << "Weather: " << weatherType << "\n";
    metricsFile << "Total Ticks: " << totalTicks << "\n\n";
    
    metricsFile << "TRAIN STATISTICS:\n";
    metricsFile << "Trains Delivered: " << trainsDelivered << "\n";
    metricsFile << "Trains Crashed: " << trainsCrashed << "\n";
    metricsFile << "Total Trains: " << activeTrainCount << "\n\n";
    metricsFile << "PERFORMANCE METRICS:\n";
    metricsFile << "Throughput (trains per 100 ticks): " 
                << (trainsDelivered * 100.0 / max(totalTicks, 1)) << "\n";
    metricsFile << "Average Wait Time: " 
                << (totalWaitTicks * 1.0 / max(activeTrainCount, 1)) << " ticks\n";
    metricsFile << "Signal Violations: " << signalViolations << "\n";
    metricsFile << "Safety Buffers Used: " << safetyBuffersUsed << "\n\n";
    
    metricsFile << "SWITCH STATISTICS:\n";
    metricsFile << "Total Switch Flips: " << switchFlipsCount << "\n";
    metricsFile << "Energy Efficiency (trains×ticks/buffers): " 
                << (activeTrainCount * totalTicks * 1.0 / max(safetyBuffersUsed, 1)) << "\n";
                
                metricsFile.close();
    
    // Close all log files
    traceFile.close();
    switchesFile.close();
    signalsFile.close();
    
    cout << "Metrics written to out/metrics.txt" << endl;
}

    

