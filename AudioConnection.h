#ifndef AUDIOCONNECTION_H
#define AUDIOCONNECTION_H

#include <QGraphicsLineItem>
#include <QPen>

class AudioPort;

class AudioConnection : public QGraphicsLineItem {
public:
	AudioConnection(AudioPort *fromPort, AudioPort *toPort, QGraphicsItem *parent = nullptr);
	~AudioConnection();

	AudioPort* getFromPort() const { return m_fromPort; }
	AudioPort* getToPort() const { return m_toPort; }

	// Update the line position based on port positions
	void updateLine();

	// Style constants
	static constexpr qreal LINE_WIDTH = 2.0;

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	AudioPort *m_fromPort;
	AudioPort *m_toPort;
};

#endif // AUDIOCONNECTION_H
