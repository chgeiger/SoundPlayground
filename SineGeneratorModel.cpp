#include "SineGeneratorModel.h"
#include <QtGlobal>
#include <cmath>

namespace {
	constexpr qreal PI = 3.14159265358979323846;
}

SineGeneratorModel::SineGeneratorModel(qreal sampleRate)
	: m_sampleRate(sampleRate)
{
	if (m_sampleRate <= 0.0) {
		m_sampleRate = 44100.0;
	}
}

void SineGeneratorModel::setFrequency(qreal frequencyHz)
{
	std::lock_guard<std::mutex> lock(m_audioMutex);
	m_frequencyHz = qBound(0.0, frequencyHz, 22000.0);
}

qreal SineGeneratorModel::frequency() const
{
	std::lock_guard<std::mutex> lock(m_audioMutex);
	return m_frequencyHz;
}

void SineGeneratorModel::setAmplitude(qreal amplitude)
{
	std::lock_guard<std::mutex> lock(m_audioMutex);
	m_amplitude = qBound(0.0, amplitude, 1.0);
}

qreal SineGeneratorModel::amplitude() const
{
	std::lock_guard<std::mutex> lock(m_audioMutex);
	return m_amplitude;
}

void SineGeneratorModel::setSampleRate(qreal sampleRate)
{
	std::lock_guard<std::mutex> lock(m_audioMutex);
	if (sampleRate > 0.0) {
		m_sampleRate = sampleRate;
	}
}

qreal SineGeneratorModel::sampleRate() const
{
	std::lock_guard<std::mutex> lock(m_audioMutex);
	return m_sampleRate;
}

qreal SineGeneratorModel::nextSample()
{
	std::lock_guard<std::mutex> lock(m_audioMutex);
	const qreal sample = m_amplitude * std::sin(m_phase);
	const qreal phaseIncrement = (2.0 * PI * m_frequencyHz) / m_sampleRate;
	m_phase += phaseIncrement;

	if (m_phase >= 2.0 * PI) {
		m_phase = std::fmod(m_phase, 2.0 * PI);
	}

	return sample;
}

void SineGeneratorModel::resetPhase()
{
	std::lock_guard<std::mutex> lock(m_audioMutex);
	m_phase = 0.0;
}
