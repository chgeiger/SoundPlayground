#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QPointF>

class AudioPort;

class CustomGraphicsScene : public QGraphicsScene {
	Q_OBJECT

public:
	CustomGraphicsScene(QObject *parent = nullptr);
	~CustomGraphicsScene();

	// Start drag connection from a port
	void startDragConnection(AudioPort *fromPort, const QPointF &startPos);

protected:
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private slots:
	void onPortPressed(AudioPort *port);

private:
	void updateDragLine(const QPointF &pos);
	void endDragConnection(const QPointF &endPos);
	void createConnection(AudioPort *fromPort, AudioPort *toPort);
	AudioPort* getPortAtPos(const QPointF &pos);

	AudioPort *m_draggingPort = nullptr;
	QGraphicsLineItem *m_dragLine = nullptr;
};

#endif // CUSTOMGRAPHICSSCENE_H
