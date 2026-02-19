#include "AudioOutModule.h"
#include "AudioConnection.h"
#include "AudioPort.h"
#include "SinusGeneratorModule.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
std::mutex AudioOutModule::s_portAudioMutex;
int AudioOutModule::s_portAudioRefCount = 0;
#endif

AudioOutModule::AudioOutModule(QGraphicsItem *parent)
	: AudioModule("Audio-Out", 1, 0, parent)
{
#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
	m_statusText = "Stopped";
#else
	m_statusText = "PortAudio fehlt";
#endif
}

AudioOutModule::~AudioOutModule()
{
	stopStream();
}

void AudioOutModule::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	refreshConnectedGenerator();

	QDialog dialog;
	dialog.setWindowTitle("Audio-Out Parameter");

	QFormLayout *formLayout = new QFormLayout(&dialog);

	QDoubleSpinBox *gainSpin = new QDoubleSpinBox(&dialog);
	gainSpin->setRange(0.0, 2.0);
	gainSpin->setSingleStep(0.01);
	gainSpin->setDecimals(2);
	gainSpin->setValue(m_outputGain);
	formLayout->addRow("Gain:", gainSpin);

	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
	formLayout->addWidget(buttons);

	QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	if (dialog.exec() == QDialog::Accepted) {
		{
			std::lock_guard<std::mutex> lock(m_stateMutex);
			m_outputGain = gainSpin->value();
		}

#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
		if (!m_isRunning) {
			startStream();
		}
#else
		m_statusText = "PortAudio fehlt";
#endif
		update();
	}

	event->accept();
}

void AudioOutModule::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	AudioModule::paint(painter, option, widget);

	qreal gain = 0.0;
	QString status;
	QString sourceName;
	{
		std::lock_guard<std::mutex> lock(m_stateMutex);
		gain = m_outputGain;
		status = m_statusText;
		sourceName = m_sourceGenerator ? m_sourceGenerator->getName() : "kein Eingang";
	}

	QFont font;
	font.setPointSize(8);
	painter->setFont(font);

	const QString infoText = QString("Gain: %1\nQuelle: %2\nStatus: %3")
		.arg(gain, 0, 'f', 2)
		.arg(sourceName)
		.arg(status);

	painter->drawText(rect().adjusted(8, 32, -8, -8), Qt::AlignLeft | Qt::AlignTop, infoText);
}

void AudioOutModule::refreshConnectedGenerator()
{
	SinusGeneratorModule *generator = nullptr;

	if (scene() && !getInputPorts().isEmpty()) {
		AudioPort *inputPort = getInputPorts().first();
		const QList<QGraphicsItem*> allItems = scene()->items();
		for (QGraphicsItem *item : allItems) {
			AudioConnection *connection = dynamic_cast<AudioConnection*>(item);
			if (!connection) {
				continue;
			}
			if (connection->getToPort() == inputPort) {
				AudioPort *fromPort = connection->getFromPort();
				if (fromPort) {
					generator = dynamic_cast<SinusGeneratorModule*>(fromPort->parentItem());
					if (generator) {
						break;
					}
				}
			}
		}
	}

	std::lock_guard<std::mutex> lock(m_stateMutex);
	m_sourceGenerator = generator;
	if (!generator && m_isRunning) {
		m_statusText = "Running (silence)";
	} else if (m_isRunning) {
		m_statusText = "Running";
	}
}

bool AudioOutModule::startStream()
{
#if !SOUNDPLAYGROUND_HAVE_PORTAUDIO
	m_isRunning = false;
	m_statusText = "PortAudio fehlt";
	return false;
#else
	if (m_isRunning) {
		return true;
	}

	if (!ensurePortAudioInitialized()) {
		m_statusText = "Init failed";
		return false;
	}

	refreshConnectedGenerator();

	PaError openError = Pa_OpenDefaultStream(
		&m_stream,
		0,
		2,
		paFloat32,
		44100,
		256,
		&AudioOutModule::renderCallback,
		this);

	if (openError != paNoError) {
		m_statusText = "Open failed";
		releasePortAudio();
		return false;
	}

	PaError startError = Pa_StartStream(m_stream);
	if (startError != paNoError) {
		Pa_CloseStream(m_stream);
		m_stream = nullptr;
		m_statusText = "Start failed";
		releasePortAudio();
		return false;
	}

	m_isRunning = true;
	m_statusText = m_sourceGenerator ? "Running" : "Running (silence)";
	return true;
#endif
}

void AudioOutModule::stopStream()
{
#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
	if (m_stream) {
		Pa_StopStream(m_stream);
		Pa_CloseStream(m_stream);
		m_stream = nullptr;
		releasePortAudio();
	}
#endif
	m_isRunning = false;
#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
	m_statusText = "Stopped";
#else
	m_statusText = "PortAudio fehlt";
#endif
}

#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
int AudioOutModule::renderCallback(const void *,
	void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo *,
	PaStreamCallbackFlags,
	void *userData)
{
	auto *self = static_cast<AudioOutModule*>(userData);
	auto *out = static_cast<float*>(output);

	SinusGeneratorModule *generator = nullptr;
	qreal gain = 0.0;
	{
		std::lock_guard<std::mutex> lock(self->m_stateMutex);
		generator = self->m_sourceGenerator;
		gain = self->m_outputGain;
	}

	for (unsigned long i = 0; i < frameCount; ++i) {
		float sample = 0.0f;
		if (generator) {
			sample = static_cast<float>(generator->nextSample() * gain);
		}
		out[2 * i] = sample;
		out[2 * i + 1] = sample;
	}

	return paContinue;
}

bool AudioOutModule::ensurePortAudioInitialized()
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

void AudioOutModule::releasePortAudio()
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
