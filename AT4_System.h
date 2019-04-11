#pragma once

class MainWindow;
class Incoming;
class Config;
class MessageQueue;
class Filter;

class System
{
public:
	bool init(int argc, char *argv[]);
	int run(void);

public:
	Config* getConfig(void) { return m_config; }
	MainWindow* getMainWindow(void) { return m_mainWindow; }
	MessageQueue* getMessageQueue(void) { return m_messageQueue; }
	Filter*	getFilter(void) { return m_filter; }

private:
	QApplication*	m_theApplication;
	Config*			m_config;
	Incoming*		m_incoming;
	MainWindow*		m_mainWindow;
	MessageQueue*	m_messageQueue;
	Filter*			m_filter;

public:
	System();
	~System();

	static System* getSingleton(void) { return s_singleton; };

private:
	static System*  s_singleton;
};
