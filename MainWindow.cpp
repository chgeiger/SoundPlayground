#include "MainWindow.h"
#include "AudioModule.h"
#include "AudioConnection.h"
#include <QGraphicsTextItem>
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

	// Setup audio modules
	setupModules();
	
	// Setup test connections
	setupConnections();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupModules()
{
	// Create test modules
	AudioModule *oscModule = new AudioModule("Oscillator", 0, 2);
	oscModule->setPos(50, 50);
	scene->addItem(oscModule);

	AudioModule *filterModule = new AudioModule("Filter", 2, 1);
	filterModule->setPos(250, 50);
	scene->addItem(filterModule);

	AudioModule *ampModule = new AudioModule("Amplifier", 1, 1);
	ampModule->setPos(450, 50);
	scene->addItem(ampModule);
}

void MainWindow::setupConnections()
{
	// This will be expanded later for dynamic connections
	// For now, we'll add test connections when modules are clicked
}