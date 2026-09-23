#include "altitude_monitor.h"
#include <klee/klee.h>

int main() {
    AltitudeReadings readings;
    
    // Make the input readings symbolic
    klee_make_symbolic(&readings, sizeof(readings), "readings");
    
    // Constrain the inputs to valid ranges to avoid generating tests for impossible scenarios
    klee_assume(readings.sensor1 >= MIN_ALTITUDE && readings.sensor1 <= MAX_ALTITUDE);
    klee_assume(readings.sensor2 >= MIN_ALTITUDE && readings.sensor2 <= MAX_ALTITUDE);
    klee_assume(readings.sensor3 >= MIN_ALTITUDE && readings.sensor3 <= MAX_ALTITUDE);

    // Call the function under test
    MonitorOutput out = check_altitude(readings);
    
    return 0;
}
