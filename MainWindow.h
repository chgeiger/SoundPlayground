#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVector>

class CustomGraphicsScene;
class AudioConnection;
class AudioOutModule;
class QAction;
class QLabel;
class QTimer;

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	static constexpr int PREMIUM_LIMIT_SECONDS = 3600;

	QGraphicsView *graphicsView;
	CustomGraphicsScene *scene;
	QVector<AudioConnection*> m_connections;
	AudioOutModule *m_audioOutModule = nullptr;
	QAction *m_audioEngineAction = nullptr;
	QLabel *m_cpuUsageLabel = nullptr;
	QTimer *m_cpuUsageTimer = nullptr;
	quint64 m_prevCpuIdle = 0;
	quint64 m_prevCpuTotal = 0;
	bool m_hasCpuSample = false;
	QLabel *m_premiumLabel = nullptr;
	QTimer *m_premiumTimer = nullptr;
	int m_premiumSecondsUsed = 0;

	void setupModules();
	void setupConnections();
	void updateCpuUsage();
	bool readCpuStats(quint64 &idle, quint64 &total) const;
	void updatePremiumLabel();
	void loadPremiumUsage();
	void savePremiumUsage() const;
};

#endif // MAINWINDOW_H
