#ifndef AUDIOOUTMODULE_H
#define AUDIOOUTMODULE_H

#include "AudioModule.h"
#include "AudioOutModel.h"
#include <QString>

class SinusGeneratorModule;
class QGraphicsSceneMouseEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

// GUI-/Graph-Knoten für den Audioausgang.
// Verknüpft einen Eingangsknoten (z.B. SinusGeneratorModule) mit AudioOutModel,
// zeigt aktuelle Parameter/Status an und reagiert auf Benutzerinteraktion.
class AudioOutModule : public AudioModule {
public:
	AudioOutModule(QGraphicsItem *parent = nullptr);
	~AudioOutModule() override;

protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	void refreshConnectedGenerator();

	SinusGeneratorModule *m_sourceGenerator = nullptr;
	AudioOutModel m_model;
};

#endif // AUDIOOUTMODULE_H
