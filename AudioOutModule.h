#ifndef AUDIOOUTMODULE_H
#define AUDIOOUTMODULE_H

#include "AudioModule.h"
#include "config.h"
#include <QString>
#include <mutex>

#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
#include <portaudio.h>
#endif

class SinusGeneratorModule;
class QGraphicsSceneMouseEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class AudioOutModule : public AudioModule {
public:
	AudioOutModule(QGraphicsItem *parent = nullptr);
	~AudioOutModule() override;

protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	void refreshConnectedGenerator();
	bool startStream();
	void stopStream();

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

	SinusGeneratorModule *m_sourceGenerator = nullptr;
	qreal m_outputGain = 0.5;
	bool m_isRunning = false;
	QString m_statusText;
	std::mutex m_stateMutex;
};

#endif // AUDIOOUTMODULE_H
