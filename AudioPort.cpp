#include "AudioPort.h"
#include "CustomGraphicsScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>

AudioPort::AudioPort(const QString &name, PortType type, QGraphicsItem *parent)
	: QGraphicsEllipseItem(-PORT_RADIUS, -PORT_RADIUS, PORT_RADIUS * 2, PORT_RADIUS * 2, parent)
	, m_name(name)
	, m_type(type)
{
	// Styling
	setBrush(QBrush(Qt::green));
	setPen(QPen(Qt::darkGreen, 1));
	setAcceptHoverEvents(true);

	// Make it interactive
	setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QPointF AudioPort::getConnectionPoint() const
{
	// Return the center point of the port in scene coordinates
	return mapToScene(0, 0);
}

void AudioPort::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	m_isDragging = true;
	setSelected(true);
	
	// Tell CustomGraphicsScene to start drag connection
	CustomGraphicsScene *customScene = dynamic_cast<CustomGraphicsScene*>(scene());
	if (customScene) {
		customScene->startDragConnection(this, event->scenePos());
	}
	
	QGraphicsEllipseItem::mousePressEvent(event);
}

void AudioPort::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_isDragging) {
		// Visual feedback: change color while dragging
		setBrush(QBrush(Qt::yellow));
		QGraphicsEllipseItem::mouseMoveEvent(event);
	}
}

void AudioPort::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	m_isDragging = false;
	// Reset color
	setBrush(QBrush(Qt::green));
	QGraphicsEllipseItem::mouseReleaseEvent(event);
}
