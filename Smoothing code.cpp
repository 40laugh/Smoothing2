// this lets you use std::uint8_t, std::uint16_t, etc
// below I use std::uint16_t because 0-1024 is too big for 8bits (0-255) and we don't want
// to handle negative numbers. The "u" before int means unsigned (only positive).
#include <cstdint>

#define MOV_AVG_SIZE  50                    // keep this many readings
#define THRESHOLD     0.97                  // reading must be X% lower than avg
#define LED_PIN       LED_BUILTIN           //defining light pin
#define LIGHT_DURATION 5                   //tenths of a second 1/10th second




struct MovingAverage {
    std::uint16_t data[MOV_AVG_SIZE] = {0}; // hold the values here, all values default to 0
    std::uint8_t  cursor{0};                // {0} just means defaults to 0
    std::uint8_t  valid_num{0};             // number of entries we have filled
    std::uint16_t sum{0};                   // start with a sum of 0

    std::uint16_t get() {
        if (valid_num == 0) { return 0; }   // don't divide by 0
        return sum / valid_num;
    }

    void insert(std::uint16_t val) {
        sum -= data[cursor];                // remove this reading from the sum
        data[cursor] = val;                 // replace old value with new
        sum += val;                         // adjust the new sum
        cursor = (cursor+1) % MOV_AVG_SIZE; // % is the "remainder" so this "wraps" the list for us
        if (valid_num < MOV_AVG_SIZE) {
            ++valid_num;                    // short hand for "add 1" but doesn't faster for reasons you shouldn't care about
        }
    }
};                                          //End of running average
MovingAverage avg;

inline std::uint16_t get_targets_pressure() {
    return analogRead(A0);
}

void set_lights(bool on) {
    if(on) {
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);
    }
}
//This is for setting up the loop of checking target, turning lights on, etc...
void setup() {
    Serial.begin(9600);
    pinMode(LED_PIN,OUTPUT);

    // "prime" the avg
    for (int i = 0; i < 100; ++i) {         // do 100 readings to get a "reliable" starting avg
        std::uint16_t r = get_targets_pressure();    // read the pin value
        avg.insert(r);                      // insert reading
    }
}


void loop() {
    std::uint16_t r = get_targets_pressure();    // read the pin value
    Serial.println(r);
    // check if we are over the threshold
    // only add to the average if _not_ over the threshold otherwise the
    // average will potentially get skewed... probably not that big of a deal
    // but what I would suggest
    if (r < avg.get()*THRESHOLD) {
        set_lights(true);
        delay(5*LIGHT_DURATION);
        set_lights(false);
    } else {
        avg.insert(r);                  // only insert reading if not across the threshold
    }
}
