#ifndef SINUSGENERATORMODULE_H
#define SINUSGENERATORMODULE_H

#include "AudioModule.h"
#include "SineGeneratorModel.h"

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
	SineGeneratorModel m_model;
};

#endif // SINUSGENERATORMODULE_H
