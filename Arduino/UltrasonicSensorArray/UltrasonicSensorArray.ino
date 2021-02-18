// This example code was used to successfully communicate with 15 ultrasonic sensors. You can adjust
// the number of sensors in your project by changing SONAR_NUM and the number of NewPing objects in the
// "sonar" array. You also need to change the pins for each sensor for the NewPing objects. Each sensor
// is pinged at 33ms intervals. So, one cycle of all sensors takes 495ms (33 * 15 = 495ms). The results
// are sent to the "oneSensorCycle" function which currently just displays the distance data. Your project
// would normally process the sensor results in this function (for example, decide if a robot needs to
// turn and call the turn function). Keep in mind this example is event-driven. Your complete sketch needs
// to be written so there's no "delay" commands and the loop() cycles at faster than a 33ms rate. If other
// processes take longer than 33ms, you'll need to increase PING_INTERVAL so it doesn't get behind.
// ---------------------------------------------------------------------------
#include <NewPing.h>

#define SONAR_NUM     5 // Number of sensors.
#define MAX_DISTANCE 500 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.


NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(12, 12, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(11, 11, MAX_DISTANCE),
  NewPing(10, 10, MAX_DISTANCE),
  NewPing(9, 9, MAX_DISTANCE),
  NewPing(8, 8, MAX_DISTANCE)
  
//  NewPing(11, 11, MAX_DISTANCE),
//  NewPing(11, 11, MAX_DISTANCE),
//  NewPing(11, 11, MAX_DISTANCE),
//  NewPing(11, 11, MAX_DISTANCE),
//  NewPing(11, 11, MAX_DISTANCE),
//  NewPing(11, 11, MAX_DISTANCE),
//  NewPing(11, 11, MAX_DISTANCE),
//  NewPing(11, 11, MAX_DISTANCE),
//  NewPing(11, 11, MAX_DISTANCE),
//  NewPing(11, 11, MAX_DISTANCE)
  
};

void setup() {
  Serial.begin(115200);
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}

void loop() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
  // Other code that *DOESN'T* analyze ping results can go here.

}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;

    // Send package for the reading
    byte reading_high = highByte(cm[currentSensor]);
    byte reading_low = lowByte(cm[currentSensor]);
    byte packet[]={0x59,0x59,currentSensor,reading_high,reading_low};
    Serial.write(packet, sizeof(packet));
    
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  
  // for (uint8_t i = 0; i < SONAR_NUM; i++) {

    // Sending bytes
    //    byte reading_high = highByte(cm[i]);
    //    byte reading_low = lowByte(cm[i]);
    //    byte packet[]={0x59,0x59,i,reading_high,reading_low};
    //    Serial.write(packet, sizeof(packet));
    
    // Printing lines
    //    if (i!=0) Serial.print(";");
    //    Serial.print(i);
    //    Serial.print(":");
    //    Serial.print(cm[i]);
    
  // }
  
  //Serial.println();
  // delay(500);

}
