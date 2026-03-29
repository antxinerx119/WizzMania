#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtGlobal>

const quint32 MAGIC_NUMBER = 0x57495A5A; // 'WIZZ' in big-endian
const quint8 PROTOCOL_VERSION = 1;

enum class MessageType : quint8 {
    LoginRequest       = 1,     // client → serveur : pseudo + (optionnel mot de passe)
    LoginResponse      = 2,     // serveur → client : succès + liste users ou erreur
    ChatMessage        = 10,    // client → serveur : message texte à broadcaster
    ChatBroadcast      = 11,    // serveur → tous : message + pseudo expéditeur
    Wizz               = 20,    // client → serveur : envoie un wizz à tout le monde ou à quelqu'un
    UserJoin           = 30,    // serveur → tous : nouveau user connecté
    UserLeave          = 31,    // serveur → tous : user déconnecté
    // Ajouter plus tard : PrivateMessage, ChangeStatus, etc.
};

#endif // PROTOCOL_H
