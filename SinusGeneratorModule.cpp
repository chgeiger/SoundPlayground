#include "SinusGeneratorModule.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

SinusGeneratorModule::SinusGeneratorModule(qreal sampleRate, QGraphicsItem *parent)
	: AudioModule("Sinus-Generator", 0, 1, parent)
{}

void SinusGeneratorModule::setFrequency(qreal frequencyHz)
{
	m_model.setFrequency(frequencyHz);
}

qreal SinusGeneratorModule::frequency() const
{
	return m_model.frequency();
}

void SinusGeneratorModule::setAmplitude(qreal amplitude)
{
	m_model.setAmplitude(amplitude);
}

qreal SinusGeneratorModule::amplitude() const
{
	return m_model.amplitude();
}

void SinusGeneratorModule::setSampleRate(qreal sampleRate)
{
	m_model.setSampleRate(sampleRate);
}

qreal SinusGeneratorModule::sampleRate() const
{
	return m_model.sampleRate();
}

qreal SinusGeneratorModule::nextSample()
{
	return m_model.nextSample();
}

void SinusGeneratorModule::resetPhase()
{
	m_model.resetPhase();
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
	frequencySpin->setValue(frequency());

	QDoubleSpinBox *amplitudeSpin = new QDoubleSpinBox(&dialog);
	amplitudeSpin->setRange(0.0, 1.0);
	amplitudeSpin->setSingleStep(0.01);
	amplitudeSpin->setDecimals(3);
	amplitudeSpin->setValue(amplitude());

	QDoubleSpinBox *sampleRateSpin = new QDoubleSpinBox(&dialog);
	sampleRateSpin->setRange(1000.0, 384000.0);
	sampleRateSpin->setDecimals(0);
	sampleRateSpin->setSuffix(" Hz");
	sampleRateSpin->setValue(sampleRate());

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

	const qreal frequencyHz = frequency();
	const qreal amplitudeValue = amplitude();
	const qreal sampleRateHz = sampleRate();

	QFont font;
	font.setPointSize(8);
	painter->setFont(font);

	const QString infoText = QString("f: %1 Hz\nA: %2\nFs: %3 Hz")
		.arg(frequencyHz, 0, 'f', 1)
		.arg(amplitudeValue, 0, 'f', 2)
		.arg(sampleRateHz, 0, 'f', 0);

	painter->drawText(rect().adjusted(8, 32, -8, -8), Qt::AlignLeft | Qt::AlignTop, infoText);
}
