#ifndef SINEGENERATORMODEL_H
#define SINEGENERATORMODEL_H

#include <QtGlobal>
#include <mutex>

class SineGeneratorModel {
public:
	explicit SineGeneratorModel(qreal sampleRate = 44100.0);

	void setFrequency(qreal frequencyHz);
	qreal frequency() const;

	void setAmplitude(qreal amplitude);
	qreal amplitude() const;

	void setSampleRate(qreal sampleRate);
	qreal sampleRate() const;

	qreal nextSample();
	void resetPhase();

private:
	qreal m_frequencyHz = 440.0;
	qreal m_amplitude = 0.8;
	qreal m_sampleRate = 44100.0;
	qreal m_phase = 0.0;
	mutable std::mutex m_audioMutex;
};

#endif // SINEGENERATORMODEL_H
