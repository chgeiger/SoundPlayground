#include "SinusGeneratorModule.h"
#include <QtGlobal>
#include <cmath>

namespace {
	constexpr qreal PI = 3.14159265358979323846;
}

SinusGeneratorModule::SinusGeneratorModule(qreal sampleRate, QGraphicsItem *parent)
	: AudioModule("Sinus-Generator", 0, 1, parent)
	, m_sampleRate(sampleRate)
{
	if (m_sampleRate <= 0.0) {
		m_sampleRate = 44100.0;
	}
}

void SinusGeneratorModule::setFrequency(qreal frequencyHz)
{
	m_frequencyHz = qBound(0.0, frequencyHz, 22000.0);
}

void SinusGeneratorModule::setAmplitude(qreal amplitude)
{
	m_amplitude = qBound(0.0, amplitude, 1.0);
}

void SinusGeneratorModule::setSampleRate(qreal sampleRate)
{
	if (sampleRate > 0.0) {
		m_sampleRate = sampleRate;
	}
}

qreal SinusGeneratorModule::nextSample()
{
	const qreal sample = m_amplitude * std::sin(m_phase);
	const qreal phaseIncrement = (2.0 * PI * m_frequencyHz) / m_sampleRate;
	m_phase += phaseIncrement;

	if (m_phase >= 2.0 * PI) {
		m_phase = std::fmod(m_phase, 2.0 * PI);
	}

	return sample;
}

void SinusGeneratorModule::resetPhase()
{
	m_phase = 0.0;
}
