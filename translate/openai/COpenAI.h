

#pragma once

#include <map>
#include <QString>
#include <QObject>
#include <QJsonObject>
#include "../core/CMysql.h"


class QNetworkReply;
class QNetworkAccessManager;


struct UOpenaiArgs
{
	QString apikey{};

	QString systemPrompt{};
	QString max_tokens{};
	QString temperature{};
	QString top_p{};
	QString frequency_penalty{};
	QString presence_penalty{};

};

class COpenAI final :public QObject
{
	Q_OBJECT

public:
	COpenAI(const QString& baseUrl="http://47.252.69.252:8866/v1", const QString&  apiKey="sk-test", QObject *parent = nullptr);
	~COpenAI(){}
	COpenAI(const COpenAI&,QObject *parent = nullptr) = delete;
	COpenAI(COpenAI&&,QObject *parent = nullptr) = delete;
	COpenAI& operator=(const COpenAI&) = delete;
	COpenAI& operator=(COpenAI&&) = delete;

	void  send(const QByteArray& jsonBody,const QString& url) const & noexcept(false);
	void slotResponse(QNetworkReply *reply);
	void chat(const QByteArray& jsonBody) const & noexcept(false);
	void youdao(const QString& word)  noexcept(false);

signals:
	void sigOaiReply(const QString& res);
private:
	UWord getWord(const QString& htmltext);
	void setWordChange(UWord& wordTemp,const QString& text);

private:
	QNetworkAccessManager * networkManager_ =  nullptr;	
	QString baseUrl_;	
	QString apiKey_;
	QJsonObject jsonBody_;
	QString word_{};

};
