#include "COpenAI.h"
#include <QUrl>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDomDocument>

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
	if (ret.contains("iciba"))
	{
		auto word = getWord(responseData);
		CMysql sql;
		if(sql.open())
		{
			auto wordSql = sql.queryWord(word_);
			if (wordSql.isEmpty())
			{
				sql.insert(wordSql);
			}
		}
		
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
	request.setRawHeader("Authorization", ("Bearer "+apiKey_).toLocal8Bit());
    QNetworkReply *reply = networkManager_->post(request, jsonBody);    
	
}

void COpenAI::youdao(const QString &word)  noexcept(false)
{
	word_ = word;
	QString url ="https://www.iciba.com/word?w="+word;
	QNetworkRequest request((QUrl(url)));
	QNetworkReply *reply = networkManager_->get(request);
}

UWord COpenAI::getWord(const QString &htmltext)
{
	UWord word{};
	QString wordText{};
	word.word = word_;
	QDomDocument doc;
	doc.setContent(htmltext);
	QDomNodeList uls = doc.elementsByTagName("ul");

	for (int i = 0; i < uls.count(); ++i) {
		QDomElement ul = uls.at(i).toElement();
		if (ul.attribute("class") == "Mean_part__UI9M6" ) {
			QDomNodeList lis = ul.elementsByTagName("li");
			for (int j = 0; j < lis.count(); ++j) {
				QDomElement li = lis.at(j).toElement();
				auto textList = li.text().split(".");
				word.word_type.append(textList[0]);
				word.meaning.append(textList[1]);
				wordText += li.text()+"\n";
				// qDebug() << li.text();
			}
		}else if(ul.attribute("class") == "Morphology_morphology__vNvkI"){
			QDomNodeList lis = ul.elementsByTagName("li");
			wordText += "\n";
			for (int j = 0; j < lis.count(); ++j) {
				QDomElement li = lis.at(j).toElement();
				setWordChange(word,li.text());
				wordText += li.text()+"\n";
			}
		}	
	}
	QDomNodeList divs = doc.elementsByTagName("div");

	for (int i = 0; i < divs.count(); ++i) {
		QDomElement div = divs.at(i).toElement();
		if (div.attribute("class") == "Affix_affix__iiL_9" ) {
			QDomNodeList ps = div.elementsByTagName("p");
			wordText += "\n";			
			for (int j = 0; j < ps.count(); ++j) {
				QDomElement p = ps.at(j).toElement();
				word.root+=p.text()+"\n";
				wordText += p.text()+"\n";
			}
			wordText += "\n";
			QDomNodeList lis = div.elementsByTagName("li");
			for (int j = 0; j < lis.count(); ++j) {
				QDomElement li = lis.at(j).toElement();
				word.illustrate+=li.text()+"\n";				
				wordText += li.text()+"\n";
			}
		}
	}
	printUWord(word);	 
	emit sigOaiReply(wordText);  
    return UWord();
}

void COpenAI::setWordChange(UWord &wordTemp, const QString &text)
{
	QStringList textList = text.split(":");
	if (textList[0]=="复数")
	{
		wordTemp.word_pl = textList[1];
	}else if(textList[0]=="第三人称单数"){
		wordTemp.word_third = textList[1];
	}else if(textList[0]=="过去式"){
		wordTemp.word_past = textList[1];
	}else if(textList[0]=="过去分词"){
		wordTemp.word_done = textList[1];
	}else if(textList[0]=="现在分词"){
		wordTemp.word_ing = textList[1];
	}else if(textList[0]=="比较级"){
		wordTemp.word_er = textList[1];
	}else if(textList[0]=="最高级"){
		wordTemp.word_est = textList[1];
	}else if(textList[0]=="形容词"){
		wordTemp.word_adj = textList[1];
	}else{
		wordTemp.remark=text;
	}
}
