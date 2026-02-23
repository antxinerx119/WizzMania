# WIZZ Mania - Application de Chat en Temps Réel

## 📖 Introduction

**WIZZ Mania** est une application de chat en temps réel inspirée du mythique **MSN Messenger** avec son célèbre effet "Wizz". Le projet met en œuvre une architecture client-serveur utilisant le protocole TCP/IP pour la communication réseau en C++17 avec Qt6.

## 🎯 Objectif

Réaliser une application de chat complète permettant :
- ✅ Connexion de plusieurs clients simultanément à un serveur centralisé
- ✅ Envoi et réception de messages en temps réel
- ✅ Authentification utilisateur (login/password)
- ✅ Effet visuel Wizz (animation de tremblement de la fenêtre)
- ✅ Liste des utilisateurs connectés
- ✅ Notification des connexions/déconnexions

## 🏗️ Architecture

Le projet est organisé en **3 parties principales** :

### **1. Serveur (`server/`)**
Serveur centralisé TCP/IP qui :
- Écoute sur le port **12345** (configurable)
- Accepte et gère plusieurs connexions clients
- Route les messages entre clients
- Gère l'authentification via une base de données SQLite
- Broadcaste les messages et les notifications (join/leave)
- Envoie les effets Wizz à tous les clients connectés

**Technologie** : Qt6 (QTcpServer/QTcpSocket), SQLite, Winsock2 abstrait par Qt

### **2. Backend/Logique Client (`client/src/back/`)**
Logique réseau côté client qui :
- Gère la connexion TCP au serveur
- Sérialise/désérialise les messages (protocole binaire)
- Envoie les login, messages et wizz
- Reçoit et traite les messages/notifications
- Expose une API orientée signaux/slots Qt

**Classe** : `ClientNetwork` (signaux : `connected`, `messageReceived`, `wizzReceived`, `userJoined`, `userLeft`, etc.)

### **3. Frontend/Interface Qt (`client/src/front/`)**
Interface graphique Qt6 Widgets qui :
- Fenêtre de login (pseudo + mot de passe)
- Fenêtre de chat avec liste des utilisateurs
- Affichage des messages en temps réel
- Animation Wizz (shake effect sur la fenêtre)
- Navigation entre les écrans via QStackedWidget

**Composants** :
- `LoginWindow` : formulaire de connexion
- `MessageWindow` : interface de chat avec liste des utilisateurs
- `MainWindow` : orchestration et gestion des états

### **4. Code Commun (`src/common/`)**
Protocole de communication et sérialisation :
- `Protocol.h` : Énumération des types de messages
- `Message.h/cpp` : Classe Message avec sérialisation/désérialisation (QDataStream)

## 📋 Protocole Réseau

### Entête du Message
```
[Magic (4 bytes)] [Version (1 byte)] [Type (1 byte)] [Payload Length (4 bytes)] [Payload]
Magic: 0x57495A5A ('WIZZ')
Version: 1
```

### Types de Messages
```cpp
enum class MessageType : quint8 {
    LoginRequest       = 1,   // client → serveur : username + password (séparés par '\n')
    LoginResponse      = 2,   // serveur → client : success ou liste des utilisateurs
    ChatMessage        = 10,  // client → serveur : message texte
    ChatBroadcast      = 11,  // serveur → tous : message + pseudo expéditeur
    Wizz               = 20,  // client → serveur ou serveur → client : effet wizz
    UserJoin           = 30,  // serveur → tous : notification connexion
    UserLeave          = 31,  // serveur → tous : notification déconnexion
};
```

## 🛠️ Prérequis

- **OS** : Windows (Winsock2 abstrait via Qt), Linux/macOS possible avec adaptations
- **Compilateur** : Visual Studio 2019+, MinGW, Clang (C++17)
- **Qt** : Qt6 (>= 6.0) avec modules :
  - `Qt6Core`
  - `Qt6Gui`
  - `Qt6Widgets`
  - `Qt6Network`
  - `Qt6Sql`
- **CMake** : >= 3.16
- **Google Test (GTest)** : optionnel pour les tests unitaires
- **Outil de build** : Ninja ou Make

### Installation de Qt6
Télécharger depuis [qt.io](https://www.qt.io/download-qt-installer) et installer les modules nécessaires.

## 📦 Installation et Compilation

### 1. Cloner le projet
```bash
git clone <repo-url>
cd WizzMania
```

### 2. Créer un répertoire de build
```bash
mkdir build && cd build
```

### 3. Configurer avec CMake
```bash
cmake -G "Ninja" ..
# ou pour Visual Studio
cmake -G "Visual Studio 16 2019" ..
```

### 4. Compiler
```bash
cmake --build . --config Release
```

**Exécutables générés** :
- `Mess.exe` : Application client-serveur combinée
- `TestMessage.exe` : Tests unitaires (sérialisation des messages)

## 🚀 Lancement

### **Mode Serveur**
```bash
./Mess --server
# Serveur démarre sur le port 12345
```

### **Mode Client**
```bash
./Mess
# N'active que l'interface client (connexion à 127.0.0.1:12345)
```

### Plusieurs clients
Ouvrir plusieurs instances de l'application en mode client :
```bash
./Mess &  # Client 1
./Mess &  # Client 2
./Mess &  # Client 3
```

## 🧪 Tests Unitaires

### Compiler les tests
```bash
cmake --build . --config Debug
```

### Lancer les tests
```bash
./TestMessage
```

Les tests actuels couvrent :
- ✅ Sérialisation/désérialisation des messages

### Tests à ajouter
- [ ] `TestServer` : démarrage du serveur, acceptation des connexions, broadcast, gestion des rooms
- [ ] `TestClientNetwork` : connexion, envoi/réception de messages, authentification
- [ ] `TestProtocol` : validité du protocole, parsing des messages malformés
- [ ] `TestMessageWindow` : interface, affichage des messages, animation wizz

## 📁 Structure du Projet

```
WizzMania/
├── CMakeLists.txt                           # Configuration de build
├── README.md                                # Documentation du projet
├── main.cpp                                 # Point d'entrée (serveur ou client)
├── client/
│   └── src/
│       ├── back/                            # Logique réseau client
│       │   ├── ClientNetwork.h
│       │   └── ClientNetwork.cpp
│       └── front/                           # Interface Qt6
│           ├── LoginWindow.h/cpp
│           ├── MainWindow.h/cpp
│           ├── MessageWindow.h/cpp
│           └── mainwindow.ui
├── server/
│   ├── Server.h                             # Classe serveur (définition)
│   ├── Server.cpp                           # Classe serveur (implémentation)
│   ├── database.hpp                         # Gestion SQLite (utilisateurs, messages)
│   └── src/                                 # (vide, réservé pour extensions serveur)
├── src/
│   └── common/                              # Code partagé
│       ├── Protocol.h                       # Énumération des types de messages
│       ├── Protocol.cpp
│       ├── Message.h                        # Classe Message + sérialisation
│       ├── Message.cpp
│       └── CMakeLists.txt
├── tests/
│   └── test_message.cpp                     # Tests de sérialisation
├── docs/
│   └── architecture.md                      # Diagramme d'architecture
└── build/                                   # Répertoire de build (généré)
```

## 🔑 Convention de Code

### Nommage
- Classes : `CamelCase` (ex: `ClientNetwork`, `LoginWindow`)
- Méthodes/fonctions : `camelCase` (ex: `connectToServer()`, `sendMessage()`)
- Variables privées : `m_` prefix (ex: `m_socket`, `m_username`)
- Constantes : `UPPER_CASE` (ex: `MAGIC_NUMBER`, `PORT`)

### Principes
- **C++17** : utilisation des `std::optional`, `std::unique_ptr`, `std::shared_ptr`
- **POO** : une classe = une responsabilité
- **Memory** : smart pointers, pas de `delete` manuel
- **Signaux/Slots** : pour la communication Qt
- **Threading** : `std::thread` + `std::mutex` si nécessaire

## 📡 Convention Git

### Branches
- `main` : branche principale (version stable)
- `server` : développement du serveur
- `back` : développement du backend client
- `front` : développement du frontend client
- `develop` : branche de développement intégration

### Commits (Conventional Commits)
```
feat(server): ajouter gestion des rooms
fix(back): corriger la sérialisation du mot de passe
test(front): ajouter tests du wizz animation
docs: mettre à jour README
chore: cleanup du code
```

## 🎮 Utilisation

### 1. Démarrer le serveur
Terminal 1 :
```bash
./Mess --server
# Console : "Server started on port 12345"
```

### 2. Lancer le premier client
Terminal 2 :
```bash
./Mess
# Interface login
```
- Entrer un pseudo (ex: `Alice`)
- Entrer un mot de passe (ex: `password123`)
- Cliquer sur "Se connecter"

### 3. Lancer un second client
Terminal 3 :
```bash
./Mess
# Interface login
```
- Entrer un pseudo (ex: `Bob`)
- Entrer un mot de passe
- Cliquer sur "Se connecter"

### 4. Communiquer
- Alice et Bob se voient mutuellement dans la liste des utilisateurs
- Taper un message et cliquer "Envoyer" pour broadcaster à tous
- Utiliser le bouton "Wizz" (si disponible) pour envoyer l'effet Wizz

## 🚧 Modules Futurs / Améliorations

- [ ] **Rooms/Salons** : créer et rejoindre des salons privés
- [ ] **Messages privés** : communication 1-to-1
- [ ] **Statuts utilisateurs** : online/away/busy/offline
- [ ] **Jeux intégrés** : tic-tac-toe, morpion, etc.
- [ ] **Persistance des messages** : historique SQLite
- [ ] **Notifications audio** : son lors de la réception d'un message
- [ ] **Session tokens** : token JWT pour sécuriser les connexions
- [ ] **Chiffrement** : SSL/TLS pour les connexions réseau
- [ ] **Support multi-plateforme** : code adapté pour Linux/macOS

## 📚 Ressources

- [Qt6 Documentation](https://doc.qt.io/qt-6/)
- [QTcpSocket](https://doc.qt.io/qt-6/qtcpsocket.html)
- [QTcpServer](https://doc.qt.io/qt-6/qtcpserver.html)
- [Protocol Buffers](https://developers.google.com/protocol-buffers) (alternative de sérialisation)

## 📄 Licence

Projet d'école - Usage personnel et éducatif

## 👥 Contributeurs

- Équipe de développement WIZZ Mania
