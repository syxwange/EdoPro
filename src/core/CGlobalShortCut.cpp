#include "CGlobalShortCut.h"
#include <Windows.h>
#include <qapplication.h>


CGlobalShortCut::CGlobalShortCut(QObject*parent)
{		
	registerHotKey();
}

bool CGlobalShortCut::nativeEventFilter(const QByteArray& eventType, void* message, qintptr*)
{
	if (eventType == "windows_generic_MSG")
	{
		MSG* msg = static_cast<MSG*>(message);
		if (msg->message == WM_HOTKEY)
		{
			if (msg->wParam == 1)
            {								
				emit signActiveWin();
				return true;
			}
			if (msg->wParam == 2)
            {
                emit signActiveScreen(); 				
				return true;
            }
		}
	}
	return false;
}

bool CGlobalShortCut::registerHotKey()
{
	RegisterHotKey(NULL, 1, MOD_ALT, 'S');
	RegisterHotKey(NULL, 2, MOD_ALT, 'W');
	return true;
}

bool CGlobalShortCut::unregisterHotKey()
{
	return UnregisterHotKey(NULL, 1) && UnregisterHotKey(NULL, 2);
}
