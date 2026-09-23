#ifndef ALTITUDE_MONITOR_H
#define ALTITUDE_MONITOR_H

#include <stdbool.h>

#define MAX_ALTITUDE 50000
#define MIN_ALTITUDE 0
#define SENSOR_TOLERANCE 50

typedef struct {
    int sensor1;
    int sensor2;
    int sensor3;
} AltitudeReadings;

typedef struct {
    int agreed_altitude;
    bool alarm_active;
} MonitorOutput;

/*@ 
  requires (readings.sensor1 >= MIN_ALTITUDE && readings.sensor1 <= MAX_ALTITUDE);
  requires (readings.sensor2 >= MIN_ALTITUDE && readings.sensor2 <= MAX_ALTITUDE);
  requires (readings.sensor3 >= MIN_ALTITUDE && readings.sensor3 <= MAX_ALTITUDE);
  
  assigns \nothing;
  
  ensures (\result.alarm_active == false) ==> 
          (\result.agreed_altitude >= MIN_ALTITUDE && \result.agreed_altitude <= MAX_ALTITUDE);
          
  ensures (\result.alarm_active == false) ==> 
          (
            (\result.agreed_altitude == readings.sensor1) || 
            (\result.agreed_altitude == readings.sensor2) || 
            (\result.agreed_altitude == readings.sensor3)
          );
@*/
MonitorOutput check_altitude(AltitudeReadings readings);

#endif // ALTITUDE_MONITOR_H
