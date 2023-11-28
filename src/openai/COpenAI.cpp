#include "COpenAI.h"
#include <QUrl>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>


COpenAI::COpenAI(const QString& baseUrl,const QString& apiKey, QObject *parent): 
	QObject(parent), networkManager_(new QNetworkAccessManager(this)),baseUrl_(baseUrl),apiKey_(apiKey)
{
	connect(networkManager_, &QNetworkAccessManager::finished, this, &COpenAI::slotResponse);
}

void COpenAI::slotResponse(QNetworkReply *reply)
{	
	QByteArray responseData = reply->readAll();
	QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	reply->deleteLater();    	
	emit sigOaiReply(responseData,statusCode.toInt());
}

void COpenAI::chat(const QByteArray &jsonBody) const & noexcept(false)
{
	QString url = baseUrl_ + "/chat/completions";
	send(jsonBody,url);
}

void COpenAI::send(const QByteArray& jsonBody,const QString& url) const & noexcept(false)
{
	QNetworkRequest request((QUrl(url)));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	request.setRawHeader("Authorization", ("Bearer "+apiKey_).toLocal8Bit());
    QNetworkReply *reply = networkManager_->post(request, jsonBody);    
	
}


