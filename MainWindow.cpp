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
#include <QLabel>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QSettings>

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
		if (enabled) {
			m_premiumTimer->start();
		} else {
			m_premiumTimer->stop();
		}
	});

	toolbar->addSeparator();
	m_cpuUsageLabel = new QLabel("CPU: -- %", this);
	toolbar->addWidget(m_cpuUsageLabel);

	m_cpuUsageTimer = new QTimer(this);
	m_cpuUsageTimer->setInterval(1000);
	connect(m_cpuUsageTimer, &QTimer::timeout, this, &MainWindow::updateCpuUsage);
	m_cpuUsageTimer->start();
	updateCpuUsage();

	// Premium quota label
	toolbar->addSeparator();
	loadPremiumUsage();
	m_premiumLabel = new QLabel(this);

	m_premiumTimer = new QTimer(this);
	m_premiumTimer->setInterval(1000);
	connect(m_premiumTimer, &QTimer::timeout, this, [this]() {
		if (m_premiumSecondsUsed < PREMIUM_LIMIT_SECONDS) {
			++m_premiumSecondsUsed;
			if (m_premiumSecondsUsed % 60 == 0) {
				savePremiumUsage();
			}
		}
		updatePremiumLabel();
	});

	updatePremiumLabel();
	toolbar->addWidget(m_premiumLabel);

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
	savePremiumUsage();
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

void MainWindow::updateCpuUsage()
{
	if (!m_cpuUsageLabel) {
		return;
	}

	quint64 idle = 0;
	quint64 total = 0;
	if (!readCpuStats(idle, total)) {
		m_cpuUsageLabel->setText("CPU: n/a");
		return;
	}

	if (!m_hasCpuSample) {
		m_prevCpuIdle = idle;
		m_prevCpuTotal = total;
		m_hasCpuSample = true;
		m_cpuUsageLabel->setText("CPU: -- %");
		return;
	}

	const quint64 totalDelta = total - m_prevCpuTotal;
	const quint64 idleDelta = idle - m_prevCpuIdle;

	m_prevCpuIdle = idle;
	m_prevCpuTotal = total;

	if (totalDelta == 0) {
		m_cpuUsageLabel->setText("CPU: -- %");
		return;
	}

	const double usage = (1.0 - (static_cast<double>(idleDelta) / static_cast<double>(totalDelta))) * 100.0;
	m_cpuUsageLabel->setText(QString("CPU: %1 %").arg(usage, 0, 'f', 1));
}

bool MainWindow::readCpuStats(quint64 &idle, quint64 &total) const
{
	QFile statFile("/proc/stat");
	if (!statFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}

	QTextStream stream(&statFile);
	const QString firstLine = stream.readLine();
	if (!firstLine.startsWith("cpu ")) {
		return false;
	}

	const QStringList fields = firstLine.simplified().split(' ');
	if (fields.size() < 5) {
		return false;
	}

	total = 0;
	for (int i = 1; i < fields.size(); ++i) {
		bool ok = false;
		const quint64 value = fields[i].toULongLong(&ok);
		if (!ok) {
			return false;
		}
		total += value;
	}

	bool idleOk = false;
	const quint64 idleValue = fields[4].toULongLong(&idleOk);
	if (!idleOk) {
		return false;
	}

	quint64 iowaitValue = 0;
	if (fields.size() > 5) {
		bool iowaitOk = false;
		iowaitValue = fields[5].toULongLong(&iowaitOk);
		if (!iowaitOk) {
			return false;
		}
	}

	idle = idleValue + iowaitValue;
	return true;
}

void MainWindow::updatePremiumLabel()
{
	const int used = qMin(m_premiumSecondsUsed, PREMIUM_LIMIT_SECONDS);
	const int usedMinutes = used / 60;
	const int limitMinutes = PREMIUM_LIMIT_SECONDS / 60;
	const double percent = (static_cast<double>(used) / PREMIUM_LIMIT_SECONDS) * 100.0;
	m_premiumLabel->setText(
		QString("Premium: %1% (%2/%3 min)")
			.arg(percent, 0, 'f', 1)
			.arg(usedMinutes)
			.arg(limitMinutes));
}

void MainWindow::loadPremiumUsage()
{
	QSettings settings("SoundPlayground", "SoundPlayground");
	m_premiumSecondsUsed = settings.value("premiumSecondsUsed", 0).toInt();
}

void MainWindow::savePremiumUsage() const
{
	QSettings settings("SoundPlayground", "SoundPlayground");
	settings.setValue("premiumSecondsUsed", m_premiumSecondsUsed);
}