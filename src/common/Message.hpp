#ifndef MESSAGE_H
#define MESSAGE_H

#include <QtCore>
#include <optional>
#include "Protocol.hpp"

class Message {
public:
    Message();
    Message(MessageType type, const QString &content = {});

    // Sérialisation (pour envoi)
    QByteArray serialize() const;

    // Désérialisation (pour réception)
    static std::optional<Message> deserialize(const QByteArray &data);

    MessageType type() const;
    QString content() const;           // texte du message
    QString sender() const;            // pseudo (rempli par le serveur pour broadcast)

    void setSender(const QString &sender);

private:
    MessageType m_type;
    QString m_content;
    QString m_sender;
};

#endif // MESSAGE_H
