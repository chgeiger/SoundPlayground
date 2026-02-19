#include "AudioOutModel.h"

AudioOutModel::AudioOutModel()
{
#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
	m_statusText = "Stopped";
#else
	m_statusText = "PortAudio fehlt";
#endif
}

AudioOutModel::~AudioOutModel()
{
	stop();
}

void AudioOutModel::setOutputGain(qreal gain)
{
	std::lock_guard<std::mutex> lock(m_stateMutex);
	m_outputGain = std::clamp(gain, 0.0, 2.0);
}

qreal AudioOutModel::outputGain() const
{
	std::lock_guard<std::mutex> lock(m_stateMutex);
	return m_outputGain;
}

void AudioOutModel::setSampleProvider(SampleProvider provider)
{
	std::lock_guard<std::mutex> lock(m_stateMutex);
	m_sampleProvider = std::move(provider);
	if (m_isRunning) {
		m_statusText = m_sampleProvider ? "Running" : "Running (silence)";
	}
}

void AudioOutModel::clearSampleProvider()
{
	std::lock_guard<std::mutex> lock(m_stateMutex);
	m_sampleProvider = nullptr;
	if (m_isRunning) {
		m_statusText = "Running (silence)";
	}
}

QString AudioOutModel::statusText() const
{
	std::lock_guard<std::mutex> lock(m_stateMutex);
	return m_statusText;
}

bool AudioOutModel::isRunning() const
{
	std::lock_guard<std::mutex> lock(m_stateMutex);
	return m_isRunning;
}

bool AudioOutModel::start()
{
#if !SOUNDPLAYGROUND_HAVE_PORTAUDIO
	std::lock_guard<std::mutex> lock(m_stateMutex);
	m_isRunning = false;
	m_statusText = "PortAudio fehlt";
	return false;
#else
	{
		std::lock_guard<std::mutex> lock(m_stateMutex);
		if (m_isRunning) {
			return true;
		}
	}

	if (!ensurePortAudioInitialized()) {
		std::lock_guard<std::mutex> lock(m_stateMutex);
		m_statusText = "Init failed";
		return false;
	}

	PaError openError = Pa_OpenDefaultStream(
		&m_stream,
		0,
		2,
		paFloat32,
		44100,
		256,
		&AudioOutModel::renderCallback,
		this);

	if (openError != paNoError) {
		{
			std::lock_guard<std::mutex> lock(m_stateMutex);
			m_statusText = "Open failed";
		}
		releasePortAudio();
		return false;
	}

	PaError startError = Pa_StartStream(m_stream);
	if (startError != paNoError) {
		Pa_CloseStream(m_stream);
		m_stream = nullptr;
		{
			std::lock_guard<std::mutex> lock(m_stateMutex);
			m_statusText = "Start failed";
		}
		releasePortAudio();
		return false;
	}

	{
		std::lock_guard<std::mutex> lock(m_stateMutex);
		m_isRunning = true;
		m_statusText = m_sampleProvider ? "Running" : "Running (silence)";
	}

	return true;
#endif
}

void AudioOutModel::stop()
{
#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
	if (m_stream) {
		Pa_StopStream(m_stream);
		Pa_CloseStream(m_stream);
		m_stream = nullptr;
		releasePortAudio();
	}
#endif
	std::lock_guard<std::mutex> lock(m_stateMutex);
	m_isRunning = false;
#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
	m_statusText = "Stopped";
#else
	m_statusText = "PortAudio fehlt";
#endif
}

#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
#include <algorithm>

std::mutex AudioOutModel::s_portAudioMutex;
int AudioOutModel::s_portAudioRefCount = 0;

int AudioOutModel::renderCallback(const void *,
	void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo *,
	PaStreamCallbackFlags,
	void *userData)
{
	auto *self = static_cast<AudioOutModel*>(userData);
	auto *out = static_cast<float*>(output);

	SampleProvider provider;
	qreal gain = 0.0;
	{
		std::lock_guard<std::mutex> lock(self->m_stateMutex);
		provider = self->m_sampleProvider;
		gain = self->m_outputGain;
	}

	for (unsigned long i = 0; i < frameCount; ++i) {
		float sample = 0.0f;
		if (provider) {
			sample = provider() * static_cast<float>(gain);
		}
		out[2 * i] = sample;
		out[2 * i + 1] = sample;
	}

	return paContinue;
}

bool AudioOutModel::ensurePortAudioInitialized()
{
	std::lock_guard<std::mutex> lock(s_portAudioMutex);
	if (s_portAudioRefCount == 0) {
		PaError err = Pa_Initialize();
		if (err != paNoError) {
			return false;
		}
	}
	++s_portAudioRefCount;
	return true;
}

void AudioOutModel::releasePortAudio()
{
	std::lock_guard<std::mutex> lock(s_portAudioMutex);
	if (s_portAudioRefCount > 0) {
		--s_portAudioRefCount;
		if (s_portAudioRefCount == 0) {
			Pa_Terminate();
		}
	}
}
#endif
