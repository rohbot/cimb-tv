#include "Arduino.h"
#include "bb_Generator.h"

bb_Generator::bb_Generator(int pin) {
	_pin = pin;
	m_interval = 10;
	_scalingFactor = SCALING_FACTOR;
	_lowThreshold = LOW_THRESHOLD;
	_energyDecr = ENERGY_DECR;
	_diff = 5;

}


void bb_Generator::begin() {

	_average = 0;
	_energy = 0;
	_prevAverage = 0;
	_changed = false;
	_running = false;
	m_readings = new int[m_interval];

}


int bb_Generator::update() {

	int newReading = analogRead(_pin);

	if (m_nbrReadings < m_interval) {
		++m_nbrReadings;
		m_sum = m_sum + newReading;
	}
	// once the array is filled, subtract the oldest data point and add the new one
	else {
		m_sum = m_sum - m_readings[m_next] + newReading;
	}

	m_readings[m_next] = newReading;
	if (++m_next >= m_interval)
		m_next = 0;

	_average = (m_sum + m_nbrReadings / 2) / m_nbrReadings;

	if ( abs( _average - _prevAverage) > _diff) {
		_changed = true;
		_prevAverage = _average;
	} else {
		_changed = false;
	}



	// Calculate Energy generated between last reading
	unsigned long dt = millis() - _lastReading;


	// Check if running
	if (_average > _lowThreshold) {
		if (!_running) {
			_running = true;
			_runTime = millis();
		}
		_energy += (_average * _scalingFactor * dt * 0.001);  // Energy = _average * dt in secs

	} else {
		if (_running) {
			_running = false;
		}
		
		_average = 0.0; 

		 // Decrease Energy when not running
		if (_energy > 0) {
			_energy -= _energyDecr;
		}
		if (_energy < 0) {
			_energy = 0;
		}

	}


	_lastReading = millis();

	return _average;
}


unsigned long bb_Generator::getRunTime() {
	if (_running) {
		return millis() - _runTime;
	} else {
		return 0;
	}
}

void bb_Generator::resetEnergy() {
	_energy = 0.0;
}

float bb_Generator::getVoltage() {
	return _average * _scalingFactor;
}

void bb_Generator::setNumReadings(int num) {
	m_interval = num;
}

void bb_Generator::setEnergyDecrement(float ergDecr) {
	_energyDecr = ergDecr;
}

void bb_Generator::setDiff(int diff) {
	_diff = diff;
}

void bb_Generator::setScalingFactor(float scaling) {
	_scalingFactor = scaling;
}

void bb_Generator::setLowCutoff(int cutoff){
	_lowThreshold = cutoff;
}

bool bb_Generator::hasChanged() {
	return _changed;
}

int bb_Generator::getReading() {
	return _average;
}

float bb_Generator::getEnergy() {
	return _energy;
}

