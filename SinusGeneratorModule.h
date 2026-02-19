#ifndef SINUSGENERATORMODULE_H
#define SINUSGENERATORMODULE_H

#include "AudioModule.h"

class SinusGeneratorModule : public AudioModule {
public:
	SinusGeneratorModule(qreal sampleRate = 44100.0, QGraphicsItem *parent = nullptr);

	void setFrequency(qreal frequencyHz);
	qreal frequency() const { return m_frequencyHz; }

	void setAmplitude(qreal amplitude);
	qreal amplitude() const { return m_amplitude; }

	void setSampleRate(qreal sampleRate);
	qreal sampleRate() const { return m_sampleRate; }

	qreal nextSample();
	void resetPhase();

private:
	qreal m_frequencyHz = 440.0;
	qreal m_amplitude = 0.8;
	qreal m_sampleRate = 44100.0;
	qreal m_phase = 0.0;
};

#endif // SINUSGENERATORMODULE_H
