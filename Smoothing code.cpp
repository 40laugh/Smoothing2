// this lets you use std::uint8_t, std::uint16_t, etc
// below I use std::uint16_t because 0-1024 is too big for 8bits (0-255) and we don't want
// to handle negative numbers. The "u" before int means unsigned (only positive).
#include <cstdint>

#define MOV_AVG_SIZE  20                    // keep this many readings
#define THRESHOLD     1.5                   // reading must be 1.5x higher than avg
int sensorPin = A0;
int ledPin = LED_BUILTIN;
struct MovingAverage {
    std::uint16_t data[MOV_AVG_SIZE] = {0}; // hold the values here, all values default to 0
    std::uint8_t  cursor{0};                // {0} just means defaults to 0
    std::uint8_t  valid_num{0};             // number of entries we have filled
    std::uint16_t sum{0};                   // start with a sum of 0

    std::uint16_t get() {
        if (valid_num == 0) { return 0; }   // dopn't divide by 0
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
};
void setup()
void loop()    
std::uint16_t get_reading() {
    // this always returns 7 just because.... put your "read the pin" code here
    return 7;
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode (sensorPin, INPUT);
  pinMode(ledPin,OUTPUT);
}

  // put your main code here, to run repeatedly:

}  
}

void do_lights() {
    // turn on your lights, play music, etc
 Serial.println(sensorValue);
  if (sensorValue>864)
  {
    digitalWrite(ledPin,HIGH);
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }
}

int main() {
    MovingAverage avg;

    // "prime" the avg
    for (int i = 0; i < 100; ++i) {         // do 100 readings to get a "reliable" starting avg
        std::uint16_t r = get_reading();    // read the pin value
        avg.insert(r);                      // insert reading
    }

    // off to the races
    for (;;) {                              // infinite loop
        std::uint16_t r = get_reading();    // read the pin value

        // check if we are over the threshold
        // only add to the average if _not_ over the threshold otherwise the
        // average will potentially get skewed... probably not that big of a deal
        // but what I would suggest
        if (r > avg.get()*THRESHOLD) {
            do_lights();                    // we crossed the threshold, do stuff
        } else {
            avg.insert(r);                  // only insert reading if not across the threshold
        }
    }
}
