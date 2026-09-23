#include "altitude_monitor.h"

// Helper function to get absolute difference
/*@ 
  assigns \nothing;
  ensures \result >= 0;
  ensures \result == a - b || \result == b - a;
@*/
static int abs_diff(int a, int b) {
    return (a > b) ? (a - b) : (b - a);
}

MonitorOutput check_altitude(AltitudeReadings readings) {
    MonitorOutput out;
    out.alarm_active = true;
    out.agreed_altitude = 0;

    int s1 = readings.sensor1;
    int s2 = readings.sensor2;
    int s3 = readings.sensor3;

    bool s1_s2_agree = abs_diff(s1, s2) <= SENSOR_TOLERANCE;
    bool s2_s3_agree = abs_diff(s2, s3) <= SENSOR_TOLERANCE;
    bool s1_s3_agree = abs_diff(s1, s3) <= SENSOR_TOLERANCE;

    // Median voter logic or agreement logic
    if (s1_s2_agree && s2_s3_agree && s1_s3_agree) {
        // All agree, pick median (simplification: pick s2 if sorted, but we don't know sort order)
        // We'll just pick s1 as a representative since they all agree within tolerance
        out.agreed_altitude = s1;
        out.alarm_active = false;
    } else if (s1_s2_agree) {
        out.agreed_altitude = s1;
        out.alarm_active = false;
    } else if (s2_s3_agree) {
        out.agreed_altitude = s2;
        out.alarm_active = false;
    } else if (s1_s3_agree) {
        out.agreed_altitude = s1;
        out.alarm_active = false;
    } else {
        // Total disagreement
        out.alarm_active = true;
        out.agreed_altitude = 0; // safe fallback state
    }

    return out;
}
