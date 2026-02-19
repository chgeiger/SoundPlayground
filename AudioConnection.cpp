#include "AudioConnection.h"
#include "AudioPort.h"
#include <QPainter>

AudioConnection::AudioConnection(AudioPort *fromPort, AudioPort *toPort, QGraphicsItem *parent)
	: QGraphicsLineItem(parent)
	, m_fromPort(fromPort)
	, m_toPort(toPort)
{
	// Styling
	setPen(QPen(Qt::blue, LINE_WIDTH));
	setAcceptHoverEvents(true);

	// Draw the line with some curvature effect (we'll use a line for now)
	updateLine();
}

AudioConnection::~AudioConnection()
{
}

void AudioConnection::updateLine()
{
	if (m_fromPort && m_toPort) {
		QPointF fromPoint = m_fromPort->getConnectionPoint();
		QPointF toPoint = m_toPort->getConnectionPoint();
		setLine(QLineF(fromPoint, toPoint));
	}
}

void AudioConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	// Update line positions before painting (in case ports moved)
	updateLine();
	QGraphicsLineItem::paint(painter, option, widget);
}
