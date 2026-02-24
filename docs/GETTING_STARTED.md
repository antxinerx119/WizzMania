# GUIDE DE DÉMARRAGE - WIZZ Mania

Bienvenue dans le projet WIZZ Mania ! Ce guide vous aidera à démarrer rapidement.

## ⏱️ 5 Minutes - Configuration de Base

### 1. Prérequis
Installer les éléments suivants :
- **Qt Creator** ou **Visual Studio Code** : [qt.io](https://www.qt.io)
- **CMake** (3.16+) : [cmake.org](https://cmake.org)
- **C++ Compiler** : MSVC 2019+ (Windows), GCC 9+ (Linux), Clang 12+ (macOS)

### 2. Cloner le Projet
```bash
git clone <repo-url>
cd WizzMania
```

### 3. Compiler
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### 4. Lancer
**Terminal 1 - Serveur** :
```bash
./Mess --server
```

**Terminal 2 - Client 1** :
```bash
./Mess
# Connexion : Alice / password
```

**Terminal 3 - Client 2** :
```bash
./Mess
# Connexion : Bob / password
```

✅ **Terminé !** Vous devriez voir Alice et Bob communiquer en temps réel.

---

## 📚 15 Minutes - Apprendre l'Architecture

### Structure du Projet

```
WizzMania/
├── server/                      # 🖥️ Serveur centralisé
│   ├── Server.h/cpp             # Logique serveur
│   └── database.hpp             # SQLite
│
├── client/src/back/             # 📡 Backend client (réseau)
│   └── ClientNetwork.h/cpp      # Connexion TCP, messages
│
├── client/src/front/            # 🎨 Frontend (interface Qt)
│   ├── LoginWindow              # Écran de login
│   ├── MessageWindow            # Écran de chat
│   └── MainWindow               # Orchestration
│
├── src/common/                  # 🔄 Code partagé
│   ├── Message.h/cpp            # Classe Message
│   └── Protocol.h               # Types de messages
│
└── tests/                       # 🧪 Tests unitaires
    ├── test_message.cpp
    ├── test_server.cpp
    └── test_clientnetwork.cpp
```

### Flux de Données

```
Client 1               Serveur                Client 2
   |                     |                       |
   |--- LoginRequest ---->|                       |
   |                     |--- Check Auth (DB)    |
   |<-- LoginResponse ---|                       |
   |                     |                       |
   |--- ChatMessage ----->|                       |
   |                     |--- ChatBroadcast --->|
   |                     |                   (Display)
   |                     |                       |
   |<-- ChatBroadcast ---|<-- ChatBroadcast ----|
   |               (Display)
```

### Protocole Réseau

Chaque message TCP commence par un header :
```
[Magic: 'WIZZ' (4B)] [Version (1B)] [Type (1B)] [PayloadSize (4B)] [Payload]
```

Types : `LoginRequest`, `ChatMessage`, `Wizz`, `UserJoin`, `UserLeave`, etc.

---

## 🔧 30 Minutes - Premier Changement

### Objectif : Ajouter un Bouton "Wizz" à l'Interface

#### 1. Éditer `MessageWindow.h`
```cpp
private slots:
    void sendMessage();
    void sendWizz();  // ← AJOUTER

private:
    QPushButton *sendButton;
    QPushButton *wizzButton;  // ← AJOUTER
```

#### 2. Éditer `MessageWindow.cpp`
```cpp
// Dans le constructeur
wizzButton = new QPushButton("✨ Wizz!");
inputLayout->addWidget(wizzButton);  // Ajouter au layout

// À la fin du fichier
void MessageWindow::sendWizz() {
    // TODO : Envoyer un Wizz via ClientNetwork
    qDebug() << "Wizz sent!";
}

// Dans le constructeur, ajouter la connexion
connect(wizzButton, &QPushButton::clicked, this, &MessageWindow::sendWizz);
```

#### 3. Compiler
```bash
cd build
cmake --build .
```

#### 4. Tester
Lancer l'app, le bouton "Wizz!" devrait apparaître !

#### 5. Commit
```bash
git add .
git commit -m "feat(front): ajouter bouton Wizz à MessageWindow"
git push origin feature/wizz-button
```

---

## 🧪 30 Minutes - Ajouter un Test

### Objectif : Tester la Sérialisation d'un Message

#### 1. Créer `tests/test_protocol.cpp`
```cpp
#include <gtest/gtest.h>
#include "src/common/Message.h"
#include "src/common/Protocol.h"

class ProtocolTest : public ::testing::Test {};

TEST_F(ProtocolTest, MessageTypeEnumExists) {
    EXPECT_EQ(static_cast<int>(MessageType::ChatMessage), 10);
    EXPECT_EQ(static_cast<int>(MessageType::Wizz), 20);
}

TEST_F(ProtocolTest, MagicNumberIsCorrect) {
    EXPECT_EQ(MAGIC_NUMBER, 0x57495A5A);  // 'WIZZ'
}

TEST_F(ProtocolTest, SerializeDeserializeRoundtrip) {
    Message original(MessageType::ChatMessage, "Hello Test");
    original.setSender("LilyPad");
    
    QByteArray serialized = original.serialize();
    auto deserialized = Message::deserialize(serialized);
    
    EXPECT_TRUE(deserialized.has_value());
    EXPECT_EQ(deserialized->type(), MessageType::ChatMessage);
    EXPECT_EQ(deserialized->content(), "Hello Test");
    EXPECT_EQ(deserialized->sender(), "LilyPad");
}
```

#### 2. Ajouter au CMakeLists.txt
```cmake
add_executable(TestProtocol tests/test_protocol.cpp
    src/common/Message.h/cpp
    src/common/Protocol.h/cpp
)
target_link_libraries(TestProtocol PRIVATE gtest gtest_main Qt6::Core)
```

#### 3. Compiler et Tester
```bash
cd build
cmake --build .
./TestProtocol
# Résultat : [  PASSED  ] 3 tests
```

---

## 🚀 1 Heure - Implémenter une Fonctionnalité

### Objectif : Afficher l'Heure d'Envoi des Messages

#### 1. Modifier `Message.h`
```cpp
#include <QDateTime>

class Message {
public:
    // ...
    QDateTime timestamp() const { return m_timestamp; }
    void setTimestamp(const QDateTime& ts) { m_timestamp = ts; }
    
private:
    QDateTime m_timestamp;
};
```

#### 2. Modifier `Message.cpp`
```cpp
Message::Message() : m_type(MessageType::ChatMessage) {
    m_timestamp = QDateTime::currentDateTime();  // ← Auto-set
}

QByteArray Message::serialize() const {
    // ...existing serialization...
    // Ajouter le timestamp
    payloadStream << m_timestamp.toString();
}

std::optional<Message> Message::deserialize(const QByteArray &data) {
    // ...existing deserialization...
    // Ajouter la désérialisation du timestamp (simule)
    return msg;
}
```

#### 3. Modifier `MessageWindow.cpp`
```cpp
void MessageWindow::displayMessage(const Message& msg) {
    QString time = msg.timestamp().toString("hh:mm:ss");
    QString display = QString("[%1] %2: %3")
        .arg(time, msg.sender(), msg.content());
    messageDisplay->append(display);
}
```

#### 4. Tester
Envoyer des messages, vérifier que l'heure s'affiche.

#### 5. Commit
```bash
git add .
git commit -m "feat(common): ajouter timestamp aux messages"
```

---

## 📖 Ressources Recommandées

### Documentation
- **Lire d'abord** : [README_NEW.md](README_NEW.md) (5 min)
- **Architecture détaillée** : [README_COMPLETE.md](README_COMPLETE.md) (20 min)
- **Conventions de code** : [CONTRIBUTING.md](CONTRIBUTING.md) (10 min)
- **Résumé des changements** : [CHANGES_SUMMARY.md](CHANGES_SUMMARY.md) (10 min)
- **Bugs et TODO** : [BUGFIX_AND_TODO.md](BUGFIX_AND_TODO.md) (5 min)

### Liens Utiles
- [Qt Documentation - Widgets](https://doc.qt.io/qt-6/qtwidgets-index.html)
- [Qt Documentation - Network](https://doc.qt.io/qt-6/qtnetwork-index.html)
- [Google Test Primer](https://github.com/google/googletest/blob/master/docs/primer.md)
- [Conventional Commits](https://www.conventionalcommits.org/)

---

## ⚠️ Troubleshooting

### Le build échoue : "Qt6 not found"
```bash
# Windows
cmake -DQt6_DIR="C:/Qt/6.0.0/msvc2019_64/lib/cmake/Qt6" ..

# Linux (Ubuntu)
sudo apt-get install qt6-base-dev

# macOS
brew install qt6
```

### erreur "server.hpp not found"
✅ Ce fichier a été supprimé. Le projet utilise `Server.h` maintenant.

### Le serveur ne démarre pas
```bash
# Vérifier que le port 12345 est libre
netstat -ano | grep :12345  # Windows
lsof -i :12345             # macOS/Linux

# Si occupé, modifier server/Server.cpp
bool Server::start(quint16 port) {
    // Changer le port ici, ex: 12346
}
```

### Les tests ne compilent pas
```bash
# Installer Google Test
cmake -DENABLE_TESTING=ON ..
cmake --build .

# Si toujours en erreur, vérifier CMakeLists.txt
find_package(GTest REQUIRED)
```

---

## ✅ Checklist de Démarrage

- [ ] Avoir Qt6, CMake, et un compilateur C++17
- [ ] Cloner le projet
- [ ] Compiler avec succès
- [ ] Lancer serveur + 2 clients
- [ ] Envoyer un message et voir la réception
- [ ] Lancer les tests unitaires
- [ ] Lire la documentation (README_COMPLETE.md)
- [ ] Explorer le code :
  - [ ] `server/Server.cpp` - logique serveur
  - [ ] `client/src/back/ClientNetwork.cpp` - logique client
  - [ ] `client/src/front/MainWindow.cpp` - interface
  - [ ] `src/common/Message.cpp` - sérialisation

---

## 🎯 Prochaines Étapes

### Court terme (cette semaine)
1. ✅ Comprendre l'architecture
2. [ ] Implémenter une petite fonctionnalité (timestamp, icon, etc.)
3. [ ] Ajouter des tests pour cette fonctionnalité
4. [ ] Créer une Pull Request

### Moyen terme (ce mois)
1. [ ] Implémenter les **Rooms** (salons privés)
2. [ ] Ajouter persistance des messages (SQLite)
3. [ ] Améliorer l'authentication (hachage des passwords)

### Long terme
1. [ ] Ajouter des jeux intégrés
2. [ ] Support SSL/TLS
3. [ ] Optimiser la performance (scaling)

---

## 💬 Besoin d'Aide ?

1. **Consulter la documentation** dans le répertoire : README.md, CONTRIBUTING.md
2. **Vérifier les logs** : Les messages `qDebug()` s'affichent en console
3. **Lire le code** : Commenter est encouragé, demander c'est mieux que deviner
4. **Ouvrir une issue** : Décrire le problème avec détails

---

## 🎉 Bienvenue dans WIZZ Mania !

Le projet n'attend que vos contributions. N'hésitez pas à explorer le code, à poser des questions, et à proposer des améliorations.

**Happy coding! 🚀**
