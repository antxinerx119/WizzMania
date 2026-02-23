#include "Message.h"
#include <QDataStream>

Message::Message() : m_type(MessageType::ChatMessage) {}

Message::Message(MessageType type, const QString &content)
    : m_type(type), m_content(content) {}

QByteArray Message::serialize() const {
    QByteArray payload;
    QDataStream payloadStream(&payload, QIODevice::WriteOnly);
    payloadStream.setVersion(QDataStream::Qt_6_0); // or Qt_5_15
    payloadStream << m_content;
    payloadStream << m_sender;

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_0);
    stream << MAGIC_NUMBER;
    stream << PROTOCOL_VERSION;
    stream << static_cast<quint8>(m_type);
    stream << static_cast<quint32>(payload.size());
    data.append(payload);

    return data;
}

std::optional<Message> Message::deserialize(const QByteArray &data) {
    if (data.size() < 10) { // magic 4 + version 1 + type 1 + length 4
        return std::nullopt;
    }

    QDataStream stream(data);
    stream.setVersion(QDataStream::Qt_6_0);

    quint32 magic;
    stream >> magic;
    if (magic != MAGIC_NUMBER) {
        return std::nullopt;
    }

    quint8 version;
    stream >> version;
    if (version != PROTOCOL_VERSION) {
        return std::nullopt;
    }

    quint8 typeRaw;
    stream >> typeRaw;
    MessageType type = static_cast<MessageType>(typeRaw);

    quint32 length;
    stream >> length;

    if (data.size() < 10 + length) {
        return std::nullopt;
    }

    QByteArray payload = data.mid(10, length);
    QDataStream payloadStream(payload);
    payloadStream.setVersion(QDataStream::Qt_6_0);

    QString content;
    payloadStream >> content;
    QString sender;
    payloadStream >> sender;

    Message msg(type, content);
    msg.setSender(sender);
    return msg;
}

MessageType Message::type() const {
    return m_type;
}

QString Message::content() const {
    return m_content;
}

QString Message::sender() const {
    return m_sender;
}

void Message::setSender(const QString &sender) {
    m_sender = sender;
}
