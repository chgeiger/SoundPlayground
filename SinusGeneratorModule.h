#ifndef SINUSGENERATORMODULE_H
#define SINUSGENERATORMODULE_H

#include "AudioModule.h"
#include <mutex>

class SinusGeneratorModule : public AudioModule {
public:
	SinusGeneratorModule(qreal sampleRate = 44100.0, QGraphicsItem *parent = nullptr);

	void setFrequency(qreal frequencyHz);
	qreal frequency() const;

	void setAmplitude(qreal amplitude);
	qreal amplitude() const;

	void setSampleRate(qreal sampleRate);
	qreal sampleRate() const;

	qreal nextSample();
	void resetPhase();

protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	qreal m_frequencyHz = 440.0;
	qreal m_amplitude = 0.8;
	qreal m_sampleRate = 44100.0;
	qreal m_phase = 0.0;
	mutable std::mutex m_audioMutex;
};

#endif // SINUSGENERATORMODULE_H
