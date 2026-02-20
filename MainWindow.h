#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVector>

class CustomGraphicsScene;
class AudioConnection;
class AudioOutModule;
class QAction;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	QGraphicsView *graphicsView;
	CustomGraphicsScene *scene;
	QVector<AudioConnection*> m_connections;
	AudioOutModule *m_audioOutModule = nullptr;
	QAction *m_audioEngineAction = nullptr;
	
	void setupModules();
	void setupConnections();
};

#endif // MAINWINDOW_H
