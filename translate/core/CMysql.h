#pragma once

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>

struct UWord
{
    int id;
    QString word;
    QStringList word_type;
    QStringList meaning;
    QString root;    
    QString illustrate;  
    QString word_er;
    QString word_est;
    QString word_pl;
    QString word_third;
    QString word_past;
    QString word_done;
    QString word_ing;
    QString word_adj; 
    QString remark;

    bool isEmpty() const {
        return word.isEmpty() && word_type.isEmpty() && meaning.isEmpty() && root.isEmpty() &&
               illustrate.isEmpty() && word_er.isEmpty() && word_est.isEmpty() && word_pl.isEmpty() &&
               word_third.isEmpty() && word_past.isEmpty() && word_done.isEmpty() && word_ing.isEmpty() &&
               word_adj.isEmpty() && remark.isEmpty();
    }
};

inline void  printUWord(UWord word)
{
    qDebug() << "UWord("
                  << "id: " << word.id
                  << ", word: " << word.word
                  << ", word_type: " << word.word_type.join(", ")
                  << ", meaning: " << word.meaning.join(", ")
                  << ", root: " << word.root
                  << ", illustrate: " << word.illustrate
                  << ", word_er: " << word.word_er
                  << ", word_est: " << word.word_est
                  << ", word_pl: " << word.word_pl
                  << ", word_third: " << word.word_third
                  << ", word_past: " << word.word_past
                  << ", word_done: " << word.word_done
                  << ", word_ing: " << word.word_ing
                  << ", word_adj: " << word.word_adj
                  << ", remark: " << word.remark
                  << ")"; 
}

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>

class CMysql:public QObject
{
    Q_OBJECT
public:
    CMysql(QObject *parent = Q_NULLPTR);
    ~CMysql(){ if (con_) delete con_;}
    bool open(const QString& hostName="116.62.164.209", const QString& dbName="englishDb",
        const QString& userName="syxwange", const QString& password="Wg123456");
    void insert(const UWord& word);
    // void update(const QString& word,const QString& text);
    UWord queryWord(const QString& word);
    void close(){if (con_) delete con_;con_=nullptr;}
private:

private:
    sql::Connection *con_{};
};