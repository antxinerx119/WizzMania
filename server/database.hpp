#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

class Database {
public:
    Database(const QString& dbName = "wizzmania.db") {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(dbName);

        if (!m_db.open()) {
            qDebug() << "Erreur d'ouverture de la base de donnees :" << m_db.lastError().text();
        } else {
            createTables();
        }
    }

    void saveMessage(const QString& sender, const QString& message) {
        QSqlQuery query;
        query.prepare("INSERT INTO messages (sender, content, timestamp) VALUES (:sender, :content, datetime('now'))");
        query.bindValue(":sender", sender);
        query.bindValue(":content", message);

        if (!query.exec()) {
            qDebug() << "Erreur lors de la sauvegarde du message :" << query.lastError().text();
        }
    }

    bool registerUser(const QString& username, const QString& password) {
        if (userExists(username)) {
            qDebug() << "Utilisateur existe deja :" << username;
            return false;
        }

        QSqlQuery query;
        query.prepare("INSERT INTO users (username, password) VALUES (:username, :password)");
        query.bindValue(":username", username);
        query.bindValue(":password", password);

        if (!query.exec()) {
            qDebug() << "Erreur lors de l'enregistrement de l'utilisateur :" << query.lastError().text();
            return false;
        }
        return true;
    }

    bool authenticateUser(const QString& username, const QString& password) {
        QSqlQuery query;
        query.prepare("SELECT id FROM users WHERE username = :username AND password = :password");
        query.bindValue(":username", username);
        query.bindValue(":password", password);

        if (query.exec() && query.next()) {
            return true;
        }
        return false;
    }

    bool userExists(const QString& username) {
        QSqlQuery query;
        query.prepare("SELECT id FROM users WHERE username = :username");
        query.bindValue(":username", username);

        if (query.exec() && query.next()) {
            return true;
        }
        return false;
    }

    bool setUserOnline(const QString& username, bool online) {
        QSqlQuery query;
        query.prepare("UPDATE users SET is_online = :online WHERE username = :username");
        query.bindValue(":online", online ? 1 : 0);
        query.bindValue(":username", username);

        if (!query.exec()) {
            qDebug() << "Erreur lors de la mise a jour du statut en ligne :" << query.lastError().text();
            return false;
        }
        return true;
    }

private:
    void createTables() {
        QSqlQuery query;

        // Table users
        QString usersTable = "CREATE TABLE IF NOT EXISTS users ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "username TEXT UNIQUE NOT NULL, "
                             "password TEXT NOT NULL, "
                             "is_online BOOLEAN DEFAULT 0)";
        if (!query.exec(usersTable)) {
            qDebug() << "Erreur lors de la creation de la table users :" << query.lastError().text();
        }

        // Table messages
        QString messagesTable = "CREATE TABLE IF NOT EXISTS messages ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "sender TEXT, "
                                "content TEXT, "
                                "timestamp DATETIME)";
        if (!query.exec(messagesTable)) {
            qDebug() << "Erreur lors de la creation de la table messages :" << query.lastError().text();
        }
    }

    QSqlDatabase m_db;
};

#endif // DATABASE_HPP
