#ifndef AUDIOMODULE_H
#define AUDIOMODULE_H

#include <QGraphicsRectItem>
#include <QString>
#include <QVector>
#include <memory>

class AudioPort;

class AudioModule : public QGraphicsRectItem {
public:
	AudioModule(const QString &name, int numInputs, int numOutputs, QGraphicsItem *parent = nullptr);

	QString getName() const { return m_name; }
	const QVector<AudioPort*>& getInputPorts() const { return m_inputs; }
	const QVector<AudioPort*>& getOutputPorts() const { return m_outputs; }

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	void layoutPorts();

	QString m_name;
	QVector<AudioPort*> m_inputs;
	QVector<AudioPort*> m_outputs;
	bool m_isDragging = false;

	static constexpr qreal MODULE_WIDTH = 120.0;
	static constexpr qreal MODULE_HEIGHT = 200.0;
	static constexpr qreal MODULE_MARGIN = 10.0;
	static constexpr qreal PORT_SPACING = 30.0;
};

#endif // AUDIOMODULE_H
