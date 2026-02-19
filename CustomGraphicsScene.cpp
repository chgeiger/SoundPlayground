#include "CustomGraphicsScene.h"
#include "AudioPort.h"
#include "AudioConnection.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>

CustomGraphicsScene::CustomGraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
	, m_draggingPort(nullptr)
	, m_dragLine(nullptr)
{
}

CustomGraphicsScene::~CustomGraphicsScene()
{
	if (m_dragLine) {
		removeItem(m_dragLine);
		delete m_dragLine;
	}
}

void CustomGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	// Update the drag line position if we're dragging from a port
	if (m_draggingPort) {
		updateDragLine(event->scenePos());
	}
	QGraphicsScene::mouseMoveEvent(event);
}

void CustomGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (m_draggingPort) {
		// Check if we're releasing over another port
		AudioPort *targetPort = getPortAtPos(event->scenePos());
		if (targetPort && targetPort != m_draggingPort) {
			// Validate: Output to Input connections only
			if (m_draggingPort->getType() == PortType::Output &&
				targetPort->getType() == PortType::Input) {
				createConnection(m_draggingPort, targetPort);
			}
		}
		endDragConnection(event->scenePos());
	}
	QGraphicsScene::mouseReleaseEvent(event);
}

void CustomGraphicsScene::startDragConnection(AudioPort *fromPort, const QPointF &startPos)
{
	m_draggingPort = fromPort;

	// Create temporary drag line
	m_dragLine = new QGraphicsLineItem();
	m_dragLine->setPen(QPen(Qt::yellow, 2, Qt::DashLine));
	addItem(m_dragLine);

	updateDragLine(startPos);
}

void CustomGraphicsScene::updateDragLine(const QPointF &pos)
{
	if (m_dragLine && m_draggingPort) {
		QPointF fromPos = m_draggingPort->getConnectionPoint();
		m_dragLine->setLine(QLineF(fromPos, pos));
	}
}

void CustomGraphicsScene::endDragConnection(const QPointF &endPos)
{
	if (m_dragLine) {
		removeItem(m_dragLine);
		delete m_dragLine;
		m_dragLine = nullptr;
	}
	m_draggingPort = nullptr;
}

void CustomGraphicsScene::createConnection(AudioPort *fromPort, AudioPort *toPort)
{
	if (fromPort && toPort) {
		AudioConnection *connection = new AudioConnection(fromPort, toPort);
		addItem(connection);
	}
}

AudioPort* CustomGraphicsScene::getPortAtPos(const QPointF &pos)
{
	// Get all items at position using itemAt or iterate through items
	QList<QGraphicsItem*> items = this->items(pos);

	// Find the first AudioPort in the list
	for (QGraphicsItem *item : items) {
		AudioPort *port = dynamic_cast<AudioPort*>(item);
		if (port) {
			return port;
		}
	}
	return nullptr;
}

void CustomGraphicsScene::onPortPressed(AudioPort *port)
{
	// This slot would be called when a port is pressed
	// For now, we handle it in AudioPort's mousePressEvent
}
