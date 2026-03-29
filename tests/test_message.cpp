#include <QCoreApplication>
#include <QDebug>
#include "../src/common/Message.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    bool passed = false;

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
            passed = true;
        } else {
            qDebug() << "Test FAILED: Data mismatch";
        }
    } else {
        qDebug() << "Test FAILED: Deserialization failed";
    }

    return passed ? 0 : 1;
}
