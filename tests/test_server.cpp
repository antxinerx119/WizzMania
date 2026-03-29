#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QTest>
#include <QTcpSocket>
#include <QHostAddress>
#include <QThread>
#include "server/Server.hpp"
#include "src/common/Message.hpp"

// Helper class to run Server in a background thread
class ServerThread : public QThread {
    Q_OBJECT
public:
    ServerThread() : m_server(nullptr) {}
    
    ~ServerThread() {
        quit();
        wait();
        if (m_server) delete m_server;
    }
    
    bool startServer(quint16 port) {
        m_port = port;
        start();
        // Wait for server to start
        QTest::qWait(500);
        return m_server && m_serverRunning;
    }
    
    void stopServer() {
        if (m_server) {
            m_server->stop();
        }
    }

protected:
    void run() override {
        QCoreApplication* app = QCoreApplication::instance();
        m_server = new Server();
        m_serverRunning = m_server->start(m_port);
        exec();
    }

private:
    Server* m_server;
    quint16 m_port;
    bool m_serverRunning = false;
};

// Test fixture
class ServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Server starts on port 54321 to avoid conflicts
        m_serverThread = new ServerThread();
        ASSERT_TRUE(m_serverThread->startServer(54321));
    }

    void TearDown() override {
        if (m_serverThread) {
            m_serverThread->stopServer();
            delete m_serverThread;
        }
    }

    ServerThread* m_serverThread;
};

// Test: Server starts and listens
TEST_F(ServerTest, ServerStartsSuccessfully) {
    EXPECT_TRUE(m_serverThread != nullptr);
}

// Test: Client can connect to server
TEST_F(ServerTest, ClientCanConnectToServer) {
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 54321);
    
    bool connected = socket.waitForConnected(2000);
    EXPECT_TRUE(connected) << "Client should connect to server";
    
    socket.disconnectFromHost();
}

// Test: Client can send login message
TEST_F(ServerTest, ClientCanSendLoginMessage) {
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 54321);
    
    if (!socket.waitForConnected(2000)) {
        FAIL() << "Could not connect to server";
        return;
    }

    // Create and send login message
    Message loginMsg(MessageType::LoginRequest, "Alice\npassword");
    QByteArray data = loginMsg.serialize();
    
    qint64 bytesWritten = socket.write(data);
    EXPECT_GT(bytesWritten, 0) << "Should write data to socket";
    
    socket.disconnectFromHost();
}

// Test: Multiple clients
TEST_F(ServerTest, MultipleClientsCanConnect) {
    QTcpSocket socket1, socket2, socket3;
    
    socket1.connectToHost("127.0.0.1", 54321);
    socket2.connectToHost("127.0.0.1", 54321);
    socket3.connectToHost("127.0.0.1", 54321);
    
    EXPECT_TRUE(socket1.waitForConnected(2000));
    EXPECT_TRUE(socket2.waitForConnected(2000));
    EXPECT_TRUE(socket3.waitForConnected(2000));
    
    socket1.disconnectFromHost();
    socket2.disconnectFromHost();
    socket3.disconnectFromHost();
}

// Test: Client receives response on valid login
TEST_F(ServerTest, ClientReceivesLoginResponseOnValidLogin) {
    QTcpSocket socket;
    socket.connectToHost("127.0.0.1", 54321);
    
    if (!socket.waitForConnected(2000)) {
        FAIL() << "Could not connect to server";
        return;
    }

    // Send login message
    Message loginMsg(MessageType::LoginRequest, "TestUser\npass");
    QByteArray data = loginMsg.serialize();
    socket.write(data);
    socket.flush();
    
    // Wait for response
    bool readyRead = socket.waitForReadyRead(2000);
    EXPECT_TRUE(readyRead) << "Server should send response";
    
    if (readyRead) {
        QByteArray response = socket.readAll();
        auto msgOpt = Message::deserialize(response);
        EXPECT_TRUE(msgOpt.has_value()) << "Response should be valid message";
        
        if (msgOpt) {
            // Could be LoginResponse (success) or LoginResponse (error if user exists)
            EXPECT_EQ(msgOpt->type(), MessageType::LoginResponse);
        }
    }
    
    socket.disconnectFromHost();
}

// Note: This test requires a main() function with QCoreApplication
// Typically provided by Google Test with Qt integration

#include "test_server.moc"  // For Q_OBJECT meta object compilation
