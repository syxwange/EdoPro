#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "CMysql.h"

#include <jdbc/cppconn/prepared_statement.h>

CMysql::CMysql(QObject *parent):QObject(parent)
{
    // qDebug() <<QSqlDatabase::drivers();
}

bool CMysql::open(const QString& hostName,const QString& dbName,const QString& userName,const QString& password)
{
    // db_.setHostName(hostName);
    // db_.setDatabaseName(dbName);
    // db_.setUserName(userName);
    // db_.setPassword(password);   

    // if (!db_.open()) {
    //     qDebug() << "Failed to connect to root mysql admin";
    //     qDebug() << db_.lastError().text();
    //     return false;
    // } else {
    //     qDebug() << "connected to database.";
    //     return true;
    // }
    sql::mysql::MySQL_Driver *driver{};    

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con_ = driver->connect(hostName.toStdString(), userName.toStdString(), password.toStdString());
        con_->setSchema(dbName.toStdString());
    } catch (sql::SQLException &e) {
        std::cerr << "SQLException: " << e.what();
        std::cerr << " (MySQL error code: " << e.getErrorCode();
        std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
        return false;
    }



    if (con_->isValid()) {
        std::cout << "connected to database." << std::endl;
        return true;
    } else {
        std::cout << "Failed to connect to root mysql admin" << std::endl;
        delete con_;
        return false;
    }
}

void CMysql::insert(const UWord &word)
{



    auto stmt = con_->prepareStatement("INSERT INTO Words (word) VALUES (?)");   
    stmt->setString(1, word.word.toStdString());
    stmt->execute();
    delete stmt;

    stmt = con_->prepareStatement("SELECT LAST_INSERT_ID()");
    auto res = stmt->executeQuery();
    int wordId = 0;
    if (res->next()) {
        wordId = res->getInt(1);
    }
    delete res;
    delete stmt;

    if (wordId <= 0) {
        std::cout << "Failed to get last insert id." << std::endl;
        return;
    }

    stmt = con_->prepareStatement("INSERT INTO RootAffixes (word_id, root, illustrate) "
                                 "VALUES (?, ?, ?)");
    stmt->setInt(1, wordId);
    stmt->setString(2, word.root.toStdString());
    stmt->setString(3, word.illustrate.toStdString());
    stmt->execute();
    delete stmt;

    stmt = con_->prepareStatement("INSERT INTO WordChange (word_id, word_er, word_est, word_pl, word_third, word_done, word_ing) "
                                 "VALUES (?, ?, ?, ?, ?, ?, ?)");
    stmt->setInt(1, wordId);
    stmt->setString(2, word.word_er.toStdString());
    stmt->setString(3, word.word_est.toStdString());
    stmt->setString(4, word.word_pl.toStdString());
    stmt->setString(5, word.word_third.toStdString());
    stmt->setString(6, word.word_done.toStdString());
    stmt->setString(7, word.word_ing.toStdString());
    stmt->execute();
    delete stmt;

    for (int i = 0; i < word.meaning.size(); ++i) {
        stmt = con_->prepareStatement("INSERT INTO ChineseMeaning (word_id, meaning, word_type) "
                                     "VALUES (?, ?, ?)");
        stmt->setInt(1, wordId);
        stmt->setString(2, word.meaning.at(i).toStdString());
        stmt->setString(3, word.word_type.at(i).toStdString());
        stmt->execute();
        delete stmt;
    }
}

UWord CMysql::queryWord(const QString &word)
{
    auto pstmt = con_->prepareStatement("SELECT * FROM Words WHERE word = ?");
    pstmt->setString(1,word.toStdString());
    auto res = pstmt->executeQuery();
    if (res->next()) {
        int wordId = res->getInt("id");
        if (wordId > 0) {
            UWord wordTemp{};
            wordTemp.id = wordId;
            wordTemp.word = word;

            delete pstmt;
            pstmt = con_->prepareStatement(R"(SELECT * FROM Words 
            LEFT JOIN RootAffixes ON Words.id = RootAffixes.word_id 
            LEFT JOIN WordChange ON Words.id = WordChange.word_id 
            WHERE Words.id = ?)");
          
            pstmt->setInt(1, wordId);
            delete res;
            res = pstmt->executeQuery();
            while (res->next()) {
                wordTemp.root =  QString::fromStdString(res->getString("root").asStdString());
                wordTemp.illustrate = QString::fromStdString(res->getString("illustrate").asStdString());
                wordTemp.word_adj =  QString::fromStdString(res->getString("word_adj").asStdString());
                wordTemp.word_er = QString::fromStdString(res->getString("word_er").asStdString());
                wordTemp.word_est = QString::fromStdString(res->getString("word_est").asStdString());
                wordTemp.word_pl = QString::fromStdString(res->getString("word_pl").asStdString());
                wordTemp.word_third = QString::fromStdString(res->getString("word_third").asStdString());
                wordTemp.word_done = QString::fromStdString(res->getString("word_done").asStdString());
                wordTemp.word_ing = QString::fromStdString(res->getString("word_ing").asStdString());

            }
            delete pstmt;
            delete res;
            pstmt = con_->prepareStatement("SELECT * FROM ChineseMeaning WHERE word_id = id?");
            pstmt->setInt(1, wordId);
            res = pstmt->executeQuery();
            while (res->next()) {
                wordTemp.meaning.append(QString::fromStdString(res->getString("meaning").asStdString()));
                wordTemp.word_type.append(QString::fromStdString(res->getString("word_type").asStdString()));
            }

            delete pstmt;
            delete res;
            return wordTemp;
        }
    } else {
        qDebug() << "Failed to execute query: " ;
    }
    return {};
}

