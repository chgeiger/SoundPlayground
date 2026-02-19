#include "MainWindow.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle("SoundPlayground");
	setGeometry(100, 100, 800, 600);

	// Create graphics scene and view
	scene = new QGraphicsScene(this);
	graphicsView = new QGraphicsView(scene, this);
	setCentralWidget(graphicsView);

	// Set scene dimensions
	scene->setSceneRect(0, 0, 800, 600);
	scene->setBackgroundBrush(Qt::white);

	// Add a sample ellipse
	QGraphicsEllipseItem *ellipse = scene->addEllipse(100, 100, 200, 200);
	ellipse->setBrush(Qt::blue);
	ellipse->setPen(QPen(Qt::darkBlue, 2));

	// Add a text item
	QGraphicsTextItem *text = scene->addText("SoundPlayground");
	text->setPos(250, 50);
	QFont font = text->font();
	font.setPointSize(20);
	text->setFont(font);
}

MainWindow::~MainWindow()
{
}
