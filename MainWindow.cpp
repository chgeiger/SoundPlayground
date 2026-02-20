#include "MainWindow.h"
#include "CustomGraphicsScene.h"
#include "AudioModule.h"
#include "SinusGeneratorModule.h"
#include "AudioOutModule.h"
#include "AudioConnection.h"
#include <QGraphicsTextItem>
#include <QFont>
#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setWindowTitle("SoundPlayground");
	setGeometry(100, 100, 800, 600);

	// Create graphics scene and view
	scene = new CustomGraphicsScene(this);
	graphicsView = new QGraphicsView(scene, this);
	setCentralWidget(graphicsView);

	// Toolbar oberhalb des Views
	QToolBar *toolbar = addToolBar("Main Toolbar");
	toolbar->setMovable(false);

	QAction *resetViewAction = toolbar->addAction("Ansicht zurücksetzen");
	connect(resetViewAction, &QAction::triggered, this, [this]() {
		graphicsView->resetTransform();
		graphicsView->centerOn(0, 0);
	});

	m_audioEngineAction = toolbar->addAction("AudioEngine: AUS");
	m_audioEngineAction->setCheckable(true);
	m_audioEngineAction->setChecked(false);
	connect(m_audioEngineAction, &QAction::toggled, this, [this](bool enabled) {
		if (m_audioOutModule) {
			m_audioOutModule->setAudioEngineEnabled(enabled);
		}
		m_audioEngineAction->setText(enabled ? "AudioEngine: AN" : "AudioEngine: AUS");
	});

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
	SinusGeneratorModule *sinusModule = new SinusGeneratorModule();
	sinusModule->setPos(50, 50);
	scene->addItem(sinusModule);

	AudioModule *filterModule = new AudioModule("Filter", 2, 1);
	filterModule->setPos(250, 50);
	scene->addItem(filterModule);

	AudioModule *ampModule = new AudioModule("Amplifier", 1, 1);
	ampModule->setPos(450, 50);
	scene->addItem(ampModule);

	m_audioOutModule = new AudioOutModule();
	m_audioOutModule->setPos(650, 50);
	scene->addItem(m_audioOutModule);
	m_audioOutModule->setAudioEngineEnabled(false);
}

void MainWindow::setupConnections()
{
	// This will be expanded later for dynamic connections
	// For now, we'll add test connections when modules are clicked
}