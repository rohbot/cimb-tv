/*
  bb_Generator library is generic library to
*/
#ifndef bb_Generator_h
#define bb_Generator_h


#include "Arduino.h"

#define SCALING_FACTOR 0.06
#define LOW_THRESHOLD 0
#define ENERGY_DECR 0

class bb_Generator
{
public:
	bb_Generator(int pin);
	
	void setNumReadings(int num);
	void setDiff(int diff);
	void setScalingFactor(float scaling);
	void setEnergyDecrement(float ergDecr);
	void setLowCutoff(int cutoff);

	void begin();
	
	int update();
	void resetEnergy();

	int getReading();
	float getVoltage();
	float getEnergy();
	bool hasChanged();


	unsigned long getRunTime();

private:
	int _pin;
	
	int m_interval;     // number of data points for the moving average
	int m_nbrReadings;  // number of readings
	long m_sum;         // sum of the m_readings array
	int m_next;         // index to the next reading
	int *m_readings;    // pointer to the dynamically allocated interval array

	int _diff;

	float _energy;
	int _average;
	int _prevAverage;

	bool _changed;

	unsigned long _lastReading;
	
	bool _running;
	unsigned long _runTime;

	bool debug;

	float _scalingFactor;

	int _lowThreshold;

	float _energyDecr;

};
#endif