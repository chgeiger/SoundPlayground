#ifndef AUDIOPORT_H
#define AUDIOPORT_H

#include <QGraphicsEllipseItem>
#include <QString>

enum class PortType {
	Input,
	Output
};

class AudioPort : public QGraphicsEllipseItem {
public:
	AudioPort(const QString &name, PortType type, QGraphicsItem *parent = nullptr);

	QString getName() const { return m_name; }
	PortType getType() const { return m_type; }
	QPointF getConnectionPoint() const;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
	QString m_name;
	PortType m_type;
	static constexpr qreal PORT_RADIUS = 6.0;
};

#endif // AUDIOPORT_H
