#pragma once

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <qhash.h>
#include <QKeySequence>


class CGlobalShortCut : public QObject,public QAbstractNativeEventFilter
{
	Q_OBJECT
public:
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

	CGlobalShortCut(QObject*parent=nullptr);
	~CGlobalShortCut(){ unregisterHotKey(); }
	bool registerHotKey();
	bool unregisterHotKey();
signals:
	void signActiveWin();
	void signActiveScreen();
};