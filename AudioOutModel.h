#ifndef AUDIOOUTMODEL_H
#define AUDIOOUTMODEL_H

#include "config.h"
#include <QString>
#include <functional>
#include <mutex>

#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
#include <portaudio.h>
#endif

class AudioOutModel {
public:
	using SampleProvider = std::function<float()>;

	AudioOutModel();
	~AudioOutModel();

	void setOutputGain(qreal gain);
	qreal outputGain() const;

	void setSampleProvider(SampleProvider provider);
	void clearSampleProvider();

	QString statusText() const;
	bool isRunning() const;

	bool start();
	void stop();

private:
#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
	static int renderCallback(const void *input,
		void *output,
		unsigned long frameCount,
		const PaStreamCallbackTimeInfo *timeInfo,
		PaStreamCallbackFlags statusFlags,
		void *userData);

	static bool ensurePortAudioInitialized();
	static void releasePortAudio();

	PaStream *m_stream = nullptr;
	static std::mutex s_portAudioMutex;
	static int s_portAudioRefCount;
#endif

	mutable std::mutex m_stateMutex;
	qreal m_outputGain = 0.5;
	bool m_isRunning = false;
	QString m_statusText;
	SampleProvider m_sampleProvider;
};

#endif // AUDIOOUTMODEL_H
