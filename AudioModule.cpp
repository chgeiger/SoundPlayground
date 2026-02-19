#include "AudioModule.h"
#include "AudioPort.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QFont>

// Initialize static variable
qreal AudioModule::s_maxZValue = 0.0;

AudioModule::AudioModule(const QString &name, int numInputs, int numOutputs, QGraphicsItem *parent)
	: QGraphicsRectItem(0, 0, MODULE_WIDTH, MODULE_HEIGHT, parent)
	, m_name(name)
{
	// Styling
	setBrush(Qt::lightGray);
	setPen(QPen(Qt::black, 2));
	setAcceptHoverEvents(true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);

	// Create input ports
	for (int i = 0; i < numInputs; ++i) {
		AudioPort *port = new AudioPort("In " + QString::number(i), PortType::Input, this);
		m_inputs.append(port);
	}

	// Create output ports
	for (int i = 0; i < numOutputs; ++i) {
		AudioPort *port = new AudioPort("Out " + QString::number(i), PortType::Output, this);
		m_outputs.append(port);
	}

	layoutPorts();
}

void AudioModule::layoutPorts()
{
	qreal totalHeight = MODULE_HEIGHT - 2 * MODULE_MARGIN;
	qreal maxPorts = qMax(m_inputs.size(), m_outputs.size());
	qreal portSpacing = maxPorts > 1 ? totalHeight / (maxPorts + 1) : totalHeight / 2;

	// Layout input ports on the left
	for (int i = 0; i < m_inputs.size(); ++i) {
		qreal y = MODULE_MARGIN + (i + 1) * portSpacing;
		m_inputs[i]->setPos(0, y);
	}

	// Layout output ports on the right
	for (int i = 0; i < m_outputs.size(); ++i) {
		qreal y = MODULE_MARGIN + (i + 1) * portSpacing;
		m_outputs[i]->setPos(MODULE_WIDTH, y);
	}
}

void AudioModule::raiseToTop()
{
	s_maxZValue += 1.0;
	setZValue(s_maxZValue);
}

void AudioModule::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	m_isDragging = true;
	// Bring to top of Z-order stack
	raiseToTop();
	QGraphicsRectItem::mousePressEvent(event);
}

void AudioModule::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_isDragging) {
		QGraphicsRectItem::mouseMoveEvent(event);
	}
}

void AudioModule::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	m_isDragging = false;
	QGraphicsRectItem::mouseReleaseEvent(event);
}

void AudioModule::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	// Draw the module rectangle
	QGraphicsRectItem::paint(painter, option, widget);

	// Draw the module name
	QFont font;
	font.setPointSize(10);
	font.setBold(true);
	painter->setFont(font);
	painter->drawText(rect(), Qt::AlignCenter | Qt::AlignTop, m_name);
}