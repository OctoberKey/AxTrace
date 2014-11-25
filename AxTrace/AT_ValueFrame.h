/***************************************************

                     AXIA|Trace3		
				
                           (C) Copyright  Jean. 2013
***************************************************/
#pragma once
#include "Resource.h"
#include "AT_ListCtrlEx.h"
#include "AT_ChildFrame.h"

#include <ATD_Interface.h>

namespace AT3
{
/** ChildFrame - Value watcher
*/
class ValueFrameWnd : public CMDIChildWindowImpl<ValueFrameWnd>, public IChildFrame
{
public:
	void watchValue(unsigned int styleID, const AXIATRACE_TIME* tTime, const wchar_t* valueName, const wchar_t* value);
	/** get child type */
	virtual CHILD_STYLE getChildType(void) { return CS_VALUE_FRAME; }
	/** get window id */
	virtual int getWindowID(void) { return m_windowID; }
	/** redraw */
	virtual void redraw(void);
	/** get native wnd handle */
	virtual HWND getNativeWnd(void) { return m_hWnd; }

	/*************************************************************************
			Inherit Methods
	*************************************************************************/
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_TRACE_FRAME_TYPE)

	virtual void OnFinalMessage(HWND /*hWnd*/)
	{
		delete this;
	}

	BEGIN_MSG_MAP(TraceFrameWnd)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(ID_EDIT_CLEAR, OnEditClear)
		COMMAND_ID_HANDLER(ID_EDIT_CLEARALL, OnEditClear)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnEditCopy)
		COMMAND_ID_HANDLER(ID_EDIT_SELECTALL, OnEditSelectAll)
		COMMAND_ID_HANDLER(ID_FILE_SAVEAS, OnFileSaveAs)
		CHAIN_CLIENT_COMMANDS()
		CHAIN_MSG_MAP(CMDIChildWindowImpl<ValueFrameWnd>)
		CHAIN_MSG_MAP_MEMBER(m_wndListView)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEditClear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnEditSelectAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnFileSaveAs(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	/*************************************************************************
		Implementation Data
	*************************************************************************/
private:
	CListCtrlEx m_wndListView;
	CUpdateUIBase* m_pUpdateUI;
	int m_windowID;

	//value map
	typedef std::hash_map< std::wstring, int > ValueHashMap;
	ValueHashMap m_valueHashMap;

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
public:
	ValueFrameWnd(CUpdateUIBase* pUpdateUI, int m_windowID);
	~ValueFrameWnd();
};

}