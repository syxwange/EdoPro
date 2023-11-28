#include "COpenAI.h"
#include <QUrl>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>

COpenAI::COpenAI(const QString& baseUrl,const QString& apiKey, QObject *parent): 
	QObject(parent), networkManager_(new QNetworkAccessManager(this)),baseUrl_(baseUrl),apiKey_(apiKey)
{
	connect(networkManager_, &QNetworkAccessManager::finished, this, &COpenAI::slotResponse);
}


void COpenAI::slotResponse(QNetworkReply *reply)
{
	
	QByteArray responseData = reply->readAll();
	QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	int httpStatusCode = statusCode.toInt();
	QNetworkRequest request = reply->request();
	QUrl requestUrl = request.url();
	reply->deleteLater(); 
	if(httpStatusCode != 200){
		responseData.append("\n");
		responseData.append(QString::number(httpStatusCode).toUtf8());
		emit sigOaiReply(responseData);
		return;
	}

	auto ret  = requestUrl.toString();
	if (ret.contains("youdao"))
	{
		QString text;
		auto root =  QJsonDocument::fromJson(responseData).object();
		auto jsons = root["data"].toObject()["entries"].toArray();		
		for (auto json : jsons)
		{
			auto json1 = json.toObject();
			text +=json1["entry"].toString()+" : "+json1["explain"].toString()+"\n";
		}	
		QString url = baseUrl_ + "/word";
		send(responseData,url);
		emit sigOaiReply(text);
		   
		return;
	}
	auto root =  QJsonDocument::fromJson(responseData).object();
	auto res = root["choices"][0]["message"]["content"].toString();	
	 	
       	
	emit sigOaiReply(res);
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
	request.setRawHeader("Authorization", apiKey_.toLocal8Bit());
    QNetworkReply *reply = networkManager_->post(request, jsonBody);    
	
}

void COpenAI::youdao(const QString &word) const & noexcept(false)
{
	QString url ="https://dict.youdao.com/suggest?num=5&ver=3.0&doctype=json&cache=false&le=en&q="+word;
	QNetworkRequest request((QUrl(url)));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = networkManager_->get(request);
}