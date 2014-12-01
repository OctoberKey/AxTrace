/***************************************************

                     AXIA|Trace3		
				
                           (C) Copyright  Jean. 2013
***************************************************/

#include "StdAfx.h"
#include "AT_Incoming.h"
#include "AT_System.h"
#include "AT_MessageQueue.h"
#include "AT_Config.h"

namespace AT3
{

//--------------------------------------------------------------------------------------------
Incoming::Incoming()
	: m_nListenPort(DEFAULT_PORT)	
	, m_hReceiveThread(0)
	, m_opPull(0)
	, m_hQuitSignal(0)
{
}

//--------------------------------------------------------------------------------------------
Incoming::~Incoming()
{
}

//--------------------------------------------------------------------------------------------
bool Incoming::init(void)
{
	assert(m_hReceiveThread==0);

	//create pull port
	if(!_createPullPort()) return false;
	//write port number to global memory file
	System::getSingleton()->getCommonCookie()->nListenPort = m_nListenPort;

	//create quit signal
	m_hQuitSignal = CreateEventW(0, TRUE, FALSE, 0);

	//begin listen thread
	unsigned int threadID;
	m_hReceiveThread = (HANDLE)::_beginthreadex(0, 0, 
		__threadEntry, this, THREAD_QUERY_INFORMATION, &threadID);
	if(m_hReceiveThread == 0) 
	{
		zmq_close(m_opPull);
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------------
unsigned int Incoming::__threadEntry(void* param)
{
	return ((Incoming*)param)->_threadEntry();
}

//--------------------------------------------------------------------------------------------
bool Incoming::_createPullPort(void)
{
	void* ctx = System::getSingleton()->getZeroMQ();

	//try create zmp port
	do
	{
		void* port = zmq_socket(System::getSingleton()->getZeroMQ(), ZMQ_PULL);

		char temp[MAX_PATH]={0};
		StringCchPrintfA(temp, MAX_PATH, "tcp://*:%d", m_nListenPort);
		if(0==zmq_bind(port, temp))
		{
			m_opPull = port;
			m_strListenPort = temp;
			break;
		}
		m_nListenPort++;
		zmq_close(port);
	}while(m_nListenPort-DEFAULT_PORT<MAX_TRY_COUNTS);
	if(m_opPull==0) return false;

	return true;
}

//--------------------------------------------------------------------------------------------
bool Incoming::_connectToBridge(const std::string bridgeServer, int bridgePort)
{
	void* ctx = System::getSingleton()->getZeroMQ();

	char temp[MAX_PATH] = { 0 };
	StringCchPrintfA(temp, MAX_PATH, "tcp://%s:%d", bridgeServer.c_str(), bridgePort);

	void* port = zmq_socket(System::getSingleton()->getZeroMQ(), ZMQ_PULL);
	if (0 != zmq_connect(port, temp))
	{
		zmq_close(port);
		return false;
	}

	m_opPull = port;
	return true;
}

//--------------------------------------------------------------------------------------------
unsigned int Incoming::_threadEntry(void)
{
	//work loop...
	while(true)
	{
		zmq_msg_t recv_command;
		zmq_msg_init(&recv_command);
		//receive
		if(zmq_msg_recv(&recv_command, m_opPull, 0)>0)
		{
			//quit?
			if(WAIT_OBJECT_0 == WaitForSingleObject(m_hQuitSignal, 0)) break;
			if(System::getSingleton()->getConfig()->getCapture())
			{
				SYSTEMTIME tTime;
				GetLocalTime(&tTime);

				System::getSingleton()->getMessageQueue()->insertMessage((const char*)zmq_msg_data(&recv_command), zmq_msg_size(&recv_command), &tTime);
			}
		}
		zmq_msg_close(&recv_command);
	}

	_endthread();
	return 0;
}

//--------------------------------------------------------------------------------------------
void Incoming::closeListen(void)
{
	//light quit signal
	SetEvent(m_hQuitSignal);

	//push a dummy msg to let incoming loop quit
	char temp[MAX_PATH] = { 0 };
	StringCchPrintfA(temp, MAX_PATH, "tcp://127.0.0.1:%d", m_nListenPort);
	void* s = zmq_socket(System::getSingleton()->getZeroMQ(), ZMQ_PUSH);
	zmq_connect(s, temp);
	zmq_send(s, " ", 1, 0);
	zmq_close(s);

	//wait thred quit(max 1sec)
	if(WAIT_TIMEOUT == WaitForSingleObject(m_hReceiveThread, 1000))
	{
		//destroy thread
		::TerminateThread(m_hReceiveThread, 0);
	}
	CloseHandle(m_hReceiveThread); m_hReceiveThread=0;

	//close zmq bind
	zmq_unbind(m_opPull, m_strListenPort.c_str());
	zmq_close(m_opPull);
}

}

