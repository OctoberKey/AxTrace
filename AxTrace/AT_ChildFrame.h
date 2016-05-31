/***************************************************

                     AXIA|Trace3		
				
                           (C) Copyright  Jean. 2013
***************************************************/
#pragma once


namespace AT3
{
/** ChildFrame base interface
*/
class IChildFrame
{
public:
	enum CHILD_STYLE
	{
		CS_LOG_FRAME,
		CS_VALUE_FRAME,
	};

	/** get child type */
	virtual CHILD_STYLE getChildType(void) = 0;
	/** get window title*/
	virtual const std::string& getWindowTitle(void) = 0;
	/** get native wnd handle */
	virtual HWND getNativeWnd(void) = 0;
	/** redraw */
	virtual void redraw(void) = 0;
};

}
