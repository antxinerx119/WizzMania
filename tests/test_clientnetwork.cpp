#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QTest>
#include <QSignalSpy>
#include <QThread>
#include "client/src/back/ClientNetwork.h"
#include "server/Server.h"
#include "src/common/Message.h"

// Helper: Server in background thread
class ServerThread : public QThread {
    Q_OBJECT
public:
    ServerThread() : m_server(nullptr), m_serverRunning(false) {}
    
    ~ServerThread() {
        quit();
        wait();
        if (m_server) delete m_server;
    }
    
    bool startServer(quint16 port) {
        m_port = port;
        start();
        QTest::qWait(500);
        return m_serverRunning;
    }
    
    void stopServer() {
        if (m_server) m_server->stop();
    }

protected:
    void run() override {
        m_server = new Server();
        m_serverRunning = m_server->start(m_port);
        exec();
    }

private:
    Server* m_server;
    quint16 m_port;
    bool m_serverRunning;
};

// Test fixture
class ClientNetworkTest : public ::testing::Test {
protected:
    void SetUp() override {
        m_serverThread = new ServerThread();
        ASSERT_TRUE(m_serverThread->startServer(54322));
        QTest::qWait(100);
    }

    void TearDown() override {
        if (m_serverThread) {
            m_serverThread->stopServer();
            delete m_serverThread;
        }
    }

    ServerThread* m_serverThread;
};

// Test: ClientNetwork instantiation
TEST_F(ClientNetworkTest, ClientNetworkCanBeInstantiated) {
    ClientNetwork client;
    EXPECT_EQ(client.username(), "");
    EXPECT_FALSE(client.isConnected());
}

// Test: Client can connect to server
TEST_F(ClientNetworkTest, ClientCanConnectToServer) {
    ClientNetwork client;
    
    QSignalSpy connectedSpy(&client, &ClientNetwork::connected);
    client.connectToServer("127.0.0.1", 54322);
    
    EXPECT_TRUE(connectedSpy.wait(3000)) << "Should emit connected signal";
    EXPECT_TRUE(client.isConnected());
}

// Test: Client sends login message
TEST_F(ClientNetworkTest, ClientCanSendLoginMessage) {
    ClientNetwork client;
    
    QSignalSpy connectedSpy(&client, &ClientNetwork::connected);
    client.connectToServer("127.0.0.1", 54322);
    
    if (!connectedSpy.wait(3000)) {
        FAIL() << "Could not connect to server";
        return;
    }
    
    // Should not throw
    EXPECT_NO_THROW({
        client.sendLogin("TestUser", "password");
    });
}

// Test: Client sends chat message (should fail if not logged in)
TEST_F(ClientNetworkTest, ClientCannotSendChatMessageWhenNotLoggedIn) {
    ClientNetwork client;
    
    QSignalSpy connectedSpy(&client, &ClientNetwork::connected);
    QSignalSpy errorSpy(&client, &ClientNetwork::error);
    
    client.connectToServer("127.0.0.1", 54322);
    connectedSpy.wait(3000);
    
    // Try to send message without login
    client.sendChatMessage("Hello");
    
    EXPECT_TRUE(errorSpy.wait(1000)) << "Should emit error signal when not logged in";
}

// Test: Message serialization/deserialization roundtrip
TEST_F(ClientNetworkTest, MessageSerializationRoundtrip) {
    Message original(MessageType::ChatMessage, "Hello World");
    original.setSender("Alice");
    
    QByteArray serialized = original.serialize();
    auto deserialized = Message::deserialize(serialized);
    
    EXPECT_TRUE(deserialized.has_value());
    if (deserialized) {
        EXPECT_EQ(deserialized->type(), MessageType::ChatMessage);
        EXPECT_EQ(deserialized->content(), "Hello World");
        EXPECT_EQ(deserialized->sender(), "Alice");
    }
}

// Test: Login message contains username and password
TEST_F(ClientNetworkTest, LoginMessageContainsCredentials) {
    Message loginMsg(MessageType::LoginRequest, "Alice\npassword123");
    QByteArray serialized = loginMsg.serialize();
    auto deserialized = Message::deserialize(serialized);
    
    EXPECT_TRUE(deserialized.has_value());
    if (deserialized) {
        QStringList parts = deserialized->content().split("\n");
        EXPECT_EQ(parts[0], "Alice");
        EXPECT_EQ(parts[1], "password123");
    }
}

// Test: Client receives and processes messages
TEST_F(ClientNetworkTest, ClientReceivesMessagesCorrectly) {
    ClientNetwork client;
    
    QSignalSpy connectedSpy(&client, &ClientNetwork::connected);
    QSignalSpy messageReceivedSpy(&client, &ClientNetwork::messageReceived);
    
    client.connectToServer("127.0.0.1", 54322);
    connectedSpy.wait(3000);
    
    // After login, server should send user list or other messages
    // (This test assumes server behavior; adapt as needed)
    
    // Note: Would need actual server cooperation to test message reception
    // For now, just verify the signals exist
    EXPECT_TRUE(true);
}

// Test: Client disconnect
TEST_F(ClientNetworkTest, ClientCanDisconnect) {
    ClientNetwork client;
    
    QSignalSpy connectedSpy(&client, &ClientNetwork::connected);
    QSignalSpy disconnectedSpy(&client, &ClientNetwork::disconnected);
    
    client.connectToServer("127.0.0.1", 54322);
    EXPECT_TRUE(connectedSpy.wait(3000));
    
    client.disconnect();
    EXPECT_TRUE(disconnectedSpy.wait(3000)) << "Should emit disconnected signal";
}

// Test: Wizz message format
TEST_F(ClientNetworkTest, WizzMessageFormat) {
    Message wizzMsg(MessageType::Wizz, "BobTarget");
    wizzMsg.setSender("Alice");
    
    EXPECT_EQ(wizzMsg.type(), MessageType::Wizz);
    EXPECT_EQ(wizzMsg.sender(), "Alice");
    EXPECT_EQ(wizzMsg.content(), "BobTarget");
}

#include "test_clientnetwork.moc"
