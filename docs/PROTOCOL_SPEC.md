# SPÉCIFICATION DU PROTOCOLE RÉSEAU - WIZZ Mania

## Vue d'Ensemble

Le protocole WIZZ Mania est un protocole de communication binaire conçu pour TCP/IP, permettant l'échange de messages entre clients et serveur de manière fiable et structurée.

---

## Format de Message

### Structure Générale

```
┌─────────────┬──────────┬───────┬─────────────┬─────────┐
│   Magic     │ Version  │ Type  │ PayloadLen  │ Payload │
├─────────────┼──────────┼───────┼─────────────┼─────────┤
│  4 bytes    │ 1 byte   │ 1 byte│  4 bytes    │ Variable│
└─────────────┴──────────┴───────┴─────────────┴─────────┘

Magic Number:     0x57495A5A  (ASCII: 'WIZZ')
Version:          1
Type:             MessageType enum value (0-31)
PayloadLength:    Taille du payload en bytes
Payload:          Données sérializées (QString + QString + ...)
```

### Exemple Visuel (Chat Message)

```
Alice envoie "Hello Bob"

Sérialisation Qt QDataStream:
┌─ Message Object
│  ├─ type: ChatMessage (10)
│  ├─ content: "Hello Bob"
│  └─ sender: "Alice"
│
└─ Binary (wireformat)
   ┌──────────────────────────────────────────────────┐
   │ Header (10 bytes)                                │
   │ ┌──────────┬──────┬──────┬──────────────────────┐│
   │ │ WIZZ     │ v:1  │ 10   │ PayloadLen: 42       ││
   │ │ 57495A5A │ 01   │ 0A   │ 0000002A             ││
   │ └──────────┴──────┴──────┴──────────────────────┘│
   └──────────────────────────────────────────────────┘
   ┌──────────────────────────────────────────────────┐
   │ Payload (42 bytes)                               │
   │ ["Hello Bob"][                                   │
   │  QByteArray de "Hello Bob" en UTF-8             │
   │                                               ][ │
   │ ["Alice"][                                       │
   │  QByteArray de "Alice"                           │
   │                                                ]│
   └──────────────────────────────────────────────────┘
```

---

## Types de Messages

### 1. **LoginRequest** (Type: 1)

**Direction** : Client → Serveur  
**Objectif** : Demander l'authentification

```
Payload: "username\npassword"

Exemple:
┌─ LoginRequest
│  ├─ content: "Alice\nmysecretpass"
│  └─ sender: ""

Binary:
┌────────────────────────────────────────┐
│ WIZZ (v1) │ Type: 1 │ PayloadLen: 25  │
│ 57495A5A  │ 01      │ 00000019        │
├────────────────────────────────────────┤
│ "Alice\nmysecretpass"                  │
│ (Sérialisé en UTF-8 + length prefixes) │
└────────────────────────────────────────┘
```

### 2. **LoginResponse** (Type: 2)

**Direction** : Serveur → Client  
**Objectif** : Répondre à la demande d'authentification

```
Payload (Success):   "success"
Payload (Failure):   "Invalid username or password"
Payload (UserList):  "Alice,Bob,Charlie"

Exemple (Success):
┌─ LoginResponse
│  ├─ content: "success"
│  └─ sender: "SERVER"
```

### 3. **ChatMessage** (Type: 10)

**Direction** : Client → Serveur  
**Objectif** : Envoyer un message texte

```
Payload: "message texte"

Exemple:
┌─ ChatMessage
│  ├─ content: "Hey everyone!"
│  └─ sender: "Alice"
```

### 4. **ChatBroadcast** (Type: 11)

**Direction** : Serveur → Tous les Clients  
**Objectif** : Diffuser un message reçu

```
Payload: "message texte"

Exemple:
┌─ ChatBroadcast (reçu par Bob et Charlie)
│  ├─ content: "Hey everyone!"
│  └─ sender: "Alice"
```

### 5. **Wizz** (Type: 20)

**Direction** : Client → Serveur ou Serveur → Client  
**Objectif** : Envoyer/recevoir l'effet wizz

```
Payload (à un utilisateur spécifique): "TargetUsername"
Payload (à tous):                      "" (vide)

Exemple (Envoi de Alice à Bob):
┌─ Wizz
│  ├─ content: "Bob"
│  └─ sender: "Alice"

Exemple (Réception par Bob):
┌─ Wizz
│  ├─ content: ""
│  └─ sender: "Alice"
```

### 6. **UserJoin** (Type: 30)

**Direction** : Serveur → Tous les Clients  
**Objectif** : Notifier qu'un utilisateur s'est connecté

```
Payload: "nom d'utilisateur"

Exemple:
┌─ UserJoin
│  ├─ content: "Alice"
│  └─ sender: "SERVER"
```

### 7. **UserLeave** (Type: 31)

**Direction** : Serveur → Tous les Clients  
**Objectif** : Notifier qu'un utilisateur s'est déconnecté

```
Payload: "nom d'utilisateur"

Exemple:
┌─ UserLeave
│  ├─ content: "Alice"
│  └─ sender: "SERVER"
```

---

## Énumération des Types

```cpp
enum class MessageType : quint8 {
    LoginRequest       = 1,   // Authentification
    LoginResponse      = 2,   // Réponse d'authentification
    
    ChatMessage        = 10,  // Message texte (sent)
    ChatBroadcast      = 11,  // Message texte (broadcasted)
    
    Wizz               = 20,  // Effet Wizz
    
    UserJoin           = 30,  // Notification entrée
    UserLeave          = 31,  // Notification sortie
};
```

---

## Flux Réseau - Scénario Complet

```
┌─────────┐                    ┌──────────┐                    ┌─────────┐
│ Alice   │                    │ Serveur  │                    │   Bob   │
└────┬────┘                    └────┬─────┘                    └────┬────┘
     │                              │                              │
     │── 1. LoginRequest ─────────>│                              │
     │    (Alice / password)        │                              │
     │                              │ Check auth (DB)              │
     │<─ 2. LoginResponse ────────│                              │
     │     (success + user list)    │                              │
     │                              │                              │
     │                    ┌─────────┴─────────┐                   │
     │                    │ Mark Alice online │                   │
     │                    └─────────┬─────────┘                   │
     │                              │                              │
     │                              │── 3. UserJoin ────────────>│
     │                              │     (Alice joined)           │
     │                              │                              │
     │                          [CLIENT 2 CONNECTS]               │
     │                              │<── 4. LoginRequest ────────│
     │                              │    (Bob / password)         │
     │                              │                              │
     │<─ 5. LoginResponse ────────│                              │
     │     (success + Alice, Bob)   │ 6. LoginResponse ────────>│
     │                              │     (success + Alice, Bob)   │
     │                              │                              │
     │                              │── 7. UserJoin ────────────>│
     │                              │     (Bob joined)             │
     │                              │                              │
     │── 8. ChatMessage ─────────>│                              │
     │    (Hello Bob!)              │                              │
     │                              │── 9. ChatBroadcast ───────>│
     │                              │    (from Alice)              │
     │                              │                              │
     │<─ 9. ChatBroadcast ─────────│                              │
     │    (from Alice)              │                              │
     │                              │                              │
     │                              │── 10. Wizz ───────────────>│
     │                              │     (from Alice)             │
     │<─ 10. Wizz ────────────────│                              │
     │    (animate shake)           │                              │
     │                              │                              │
     │── 11. Disconnect ─────────>│                              │
     │                              │ Remove from online          │
     │                              │── 12. UserLeave ──────────>│
     │                              │     (Alice left)             │
     │                              │                              │
```

---

## États de Connexion

```
┌─────────────────────────────────────┐
│ Client State Machine                │
├─────────────────────────────────────┤
│                                     │
│  ┌─────────────┐                    │
│  │   OFFLINE   │                    │
│  └──────┬──────┘                    │
│         │ connectToServer()          │
│         v                            │
│  ┌─────────────────────┐             │
│  │  CONNECTING/TCP     │             │
│  └──────┬──────────────┘             │
│         │ [connected signal]         │
│         v                            │
│  ┌─────────────────────┐             │
│  │  AUTHENTICATING     │             │
│  │  (waiting for       │             │
│  │   LoginResponse)    │             │
│  └──────┬──────────────┘             │
│         │ [loginSuccess signal]      │
│         v                            │
│  ┌─────────────────────┐             │
│  │  AUTHENTICATED      │ ◄──────┐   │
│  │  (ready to chat)    │        │   │
│  └──────┬──────────────┘        │   │
│         │ (send/receive msgs)    │   │
│         └────────────────────────┘   │
│                                     │
```

---

## Sérialisation avec QDataStream

### Procédure Qt

```cpp
// ENVOI
Message msg(MessageType::ChatMessage, "Hello");
msg.setSender("Alice");

QByteArray payload;
QDataStream payloadStream(&payload, QIODevice::WriteOnly);
payloadStream.setVersion(QDataStream::Qt_6_0);
payloadStream << msg.content();  // Écrit taille + données
payloadStream << msg.sender();

QByteArray data;
QDataStream stream(&data, QIODevice::WriteOnly);
stream.setVersion(QDataStream::Qt_6_0);
stream << MAGIC_NUMBER;           // 0x57495A5A
stream << PROTOCOL_VERSION;       // 1
stream << static_cast<quint8>(msg.type());  // 10
stream << static_cast<quint32>(payload.size());
data.append(payload);

socket->write(data);  // Envoi TCP

// RÉCEPTION
QByteArray received = socket->readAll();

QDataStream stream(received);
stream.setVersion(QDataStream::Qt_6_0);

quint32 magic;
quint8 version, type;
quint32 payloadLen;

stream >> magic >> version >> type >> payloadLen;

if (magic != MAGIC_NUMBER) {
    qDebug() << "Invalid magic number";
    return;
}

QByteArray payload = received.mid(10, payloadLen);
QDataStream payloadStream(payload);
payloadStream.setVersion(QDataStream::Qt_6_0);

QString content, sender;
payloadStream >> content >> sender;

Message msg(static_cast<MessageType>(type), content);
msg.setSender(sender);
```

---

## Robustesse & Edge Cases

### 1. Message Fragmenrié (TCP)
```
TCP n'a pas limite de packet size. Un message peut arriver en 3 readyRead() calls:

Envoyé:  [Magic...Ver...Type...Len...Payload...]  (50 bytes)
Reçu:
  readyRead #1: [Magic...Ver...Type...L]           (10 bytes)
  readyRead #2: [en...Payload...]                   (40 bytes)

Solution: Buffer + vérification taille complète avant désérialisation
```

### 2. Corruption de Données
```
Bytes perdus/modifiés → Magic number ne correspond pas
Solution: Rejeter le message et ignorer le reste du buffer (ou fermer la connexion)
```

### 3. Timeout de Connexion
```
Client envoie LoginRequest mais pas de réponse
Solution: Implémenter QTimer qui ferme après 5 sec d'inactivité
```

### 4. Double Login
```
Client A se reconnecte avec même username avant que Server remarque la déco
Solution: Rejeter ou remplacer la session ancienne
```

---

## Performance & Optimisations

### Throughput
```
- Message typique : ~100 bytes
- Overhead header : 10 bytes
- Latence TCP : ~1ms (local)
- Max messages/sec (single client) : ~1000

Mesure: ./Mess --benchmark
```

### Bufferisation
```cpp
// Côté serveur, attendre jusqu'à 10 messages avant processing
// Réduit les context switches
```

### Compression
```
Pas implementée actuellement (futur: gzip pour payloads > 1000 bytes)
```

---

## Évolution Possible du Protocole (v2)

```cpp
enum class MessageTypeV2 : quint8 {
    // ... types existants ...
    RoomCreate    = 40,      // Créer un salon
    RoomJoin      = 41,      // Rejoindre un salon
    RoomLeave     = 42,      // Quitter un salon
    RoomBroadcast = 43,      // Message limité à une room
    
    PrivateMsg    = 50,      // Message privé 1-to-1
    UserStatus    = 60,      // Statut (online/away/busy)
};

struct HeaderV2 {
    uint32_t magic;          // 0x574Z0002 (version 2)
    uint8_t version;         // 2
    uint8_t type;
    uint32_t payloadLen;
    uint32_t roomId;         // NEW: Pour le multi-rooms
    uint64_t timestamp;      // NEW: Horodatage
    uint16_t checksum;       // NEW: CRC pour intégrité
};
```

---

**Spécification Protocole v1**  
Released: 2026-02-23  
Status: ✅ Stable
