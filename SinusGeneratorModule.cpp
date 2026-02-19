#include "SinusGeneratorModule.h"
#include <QtGlobal>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
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

void SinusGeneratorModule::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QDialog dialog;
	dialog.setWindowTitle("Sinus-Generator Parameter");

	QFormLayout *formLayout = new QFormLayout(&dialog);

	QDoubleSpinBox *frequencySpin = new QDoubleSpinBox(&dialog);
	frequencySpin->setRange(0.0, 22000.0);
	frequencySpin->setDecimals(2);
	frequencySpin->setSuffix(" Hz");
	frequencySpin->setValue(m_frequencyHz);

	QDoubleSpinBox *amplitudeSpin = new QDoubleSpinBox(&dialog);
	amplitudeSpin->setRange(0.0, 1.0);
	amplitudeSpin->setSingleStep(0.01);
	amplitudeSpin->setDecimals(3);
	amplitudeSpin->setValue(m_amplitude);

	QDoubleSpinBox *sampleRateSpin = new QDoubleSpinBox(&dialog);
	sampleRateSpin->setRange(1000.0, 384000.0);
	sampleRateSpin->setDecimals(0);
	sampleRateSpin->setSuffix(" Hz");
	sampleRateSpin->setValue(m_sampleRate);

	formLayout->addRow("Frequenz:", frequencySpin);
	formLayout->addRow("Amplitude:", amplitudeSpin);
	formLayout->addRow("Sample-Rate:", sampleRateSpin);

	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
	formLayout->addWidget(buttons);

	QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	if (dialog.exec() == QDialog::Accepted) {
		setFrequency(frequencySpin->value());
		setAmplitude(amplitudeSpin->value());
		setSampleRate(sampleRateSpin->value());
		update();
	}

	event->accept();
}

void SinusGeneratorModule::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	AudioModule::paint(painter, option, widget);

	QFont font;
	font.setPointSize(8);
	painter->setFont(font);

	const QString infoText = QString("f: %1 Hz\nA: %2\nFs: %3 Hz")
		.arg(m_frequencyHz, 0, 'f', 1)
		.arg(m_amplitude, 0, 'f', 2)
		.arg(m_sampleRate, 0, 'f', 0);

	painter->drawText(rect().adjusted(8, 32, -8, -8), Qt::AlignLeft | Qt::AlignTop, infoText);
}
