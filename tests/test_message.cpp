#include <QCoreApplication>
#include <QDebug>
#include "../src/common/Message.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Create a test message
    Message original(MessageType::ChatMessage, "Hello, world!");
    original.setSender("Alice");

    // Serialize
    QByteArray serialized = original.serialize();
    qDebug() << "Serialized size:" << serialized.size();

    // Deserialize
    auto deserialized = Message::deserialize(serialized);
    if (deserialized) {
        qDebug() << "Deserialized type:" << static_cast<int>(deserialized->type());
        qDebug() << "Deserialized content:" << deserialized->content();
        qDebug() << "Deserialized sender:" << deserialized->sender();

        // Check if identical
        if (deserialized->type() == original.type() &&
            deserialized->content() == original.content() &&
            deserialized->sender() == original.sender()) {
            qDebug() << "Test PASSED: Serialization/Deserialization successful";
        } else {
            qDebug() << "Test FAILED: Data mismatch";
        }
    } else {
        qDebug() << "Test FAILED: Deserialization failed";
    }

    return 0;
}
