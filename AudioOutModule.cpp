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

AudioOutModule::AudioOutModule(QGraphicsItem *parent)
	: AudioModule("Audio-Out", 1, 0, parent)
{}

AudioOutModule::~AudioOutModule()
{
	m_model.stop();
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
	gainSpin->setValue(m_model.outputGain());
	formLayout->addRow("Gain:", gainSpin);

	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
	formLayout->addWidget(buttons);

	QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	if (dialog.exec() == QDialog::Accepted) {
		m_model.setOutputGain(gainSpin->value());

#if SOUNDPLAYGROUND_HAVE_PORTAUDIO
		if (!m_model.isRunning()) {
			m_model.start();
		}
#else
		// Status wird im Modell gepflegt
#endif
		update();
	}

	event->accept();
}

void AudioOutModule::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	AudioModule::paint(painter, option, widget);

	const qreal gain = m_model.outputGain();
	const QString status = m_model.statusText();
	const QString sourceName = m_sourceGenerator ? m_sourceGenerator->getName() : "kein Eingang";

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

	m_sourceGenerator = generator;
	if (generator) {
		m_model.setSampleProvider([generator]() {
			return static_cast<float>(generator->nextSample());
		});
	} else {
		m_model.clearSampleProvider();
	}
}
