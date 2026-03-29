#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtGlobal>

const quint32 MAGIC_NUMBER = 0x57495A5A; // 'WIZZ' in big-endian
const quint8 PROTOCOL_VERSION = 1;

enum class MessageType : quint8 {
    LoginRequest  = 1,
    LoginResponse = 2,
    ChatMessage   = 10,
    ChatBroadcast = 11,
    UserJoin      = 30,
    UserLeave     = 31,
    // Ajouter plus tard : PrivateMessage, ChangeStatus, etc.
};

#endif // PROTOCOL_H
