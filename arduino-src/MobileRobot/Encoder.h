#ifndef ENCODER_H
#define ENCODER_H
#include <Arduino.h>

class Encoder {
	private:
		byte sensorPin;
		byte pulsesPerRot;
		byte wheelRadius;

		byte numPulses = 0;

	public:
		Encoder(byte sensorPin, byte pulsesPerRot, byte wheelRadius) {
			this->sensorPin = sensorPin;
			this->pulsesPerRot = pulsesPerRot;
			this->wheelRadius = wheelRadius;
		}

		void init() {
			pinMode(sensorPin, INPUT);
		}

		//Returns the number of pulses recorded as the encoder disk rotates
		byte cntPulses() {
			static bool prevReading = true;
			bool currReading = digitalRead(sensorPin);

			if (!currReading && prevReading) 
				numPulses++;
			
			prevReading = currReading;
			return numPulses;
		}

		//Converts the number of pulses detected to the distance travelled
		float pulsesToDist(byte numPulses) {
			return (2 * PI * wheelRadius) * (numPulses/pulsesPerRot);
		}

		void setNumPulses(int numPulses) {
			this->numPulses = numPulses;
		}

		//Tests the cntPulses method
		void debug() {
			static byte prevCnt = 0;
			static byte prevRots = 0;

			byte cnt = cntPulses();
			byte rots = (cnt % pulsesPerRot == 0 && cnt != prevCnt) ? ++rots : rots;

			if (cnt != prevCnt) {
				Serial.print("Pulses: ");
				Serial.println(cnt);	
			}

			if (rots != prevRots) {
				Serial.print("Rotations: ");
				Serial.println(rots);
			}

			prevCnt = cnt;
			prevRots = rots;
		}
};

#endif
