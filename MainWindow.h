#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector>

class AudioConnection;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	QGraphicsView *graphicsView;
	QGraphicsScene *scene;
	QVector<AudioConnection*> m_connections;
	
	void setupModules();
	void setupConnections();
};

#endif // MAINWINDOW_H
