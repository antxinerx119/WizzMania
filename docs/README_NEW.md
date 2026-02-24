# WIZZ Mania - Application de Chat en Temps Réel

**MSN Messenger est de retour !**

## 📖 Introduction

**WIZZ Mania** est une application de chat en temps réel inspirée du mythique **MSN Messenger** avec son célèbre effet "Wizz". 

La communication réseau utilise le protocole **TCP/IP** (fiable, connexion persistante) pour un chat en temps réel entre plusieurs clients connectés à un serveur centralisé.

## 🏗️ Architecture Modulaire

Le projet est séparé en **3 parties indépendantes** :

1. **Server** (`server/`) - Serveur centralisé TCP/IP
2. **Back** (`client/src/back/`) - Logique réseau client (pas d'interface)
3. **Front** (`client/src/front/`) - Interface graphique Qt6

## 🛠️ Stack Technologique

| Composant | Technologie |
|-----------|-------------|
| Langage | C++17 |
| Framework GUI | Qt6 (Widgets) |
| Réseau | Winsock2 (abstrait par Qt Network) |
| Base de données | SQLite |
| Build | CMake 3.16+ |
| Tests | Google Test (GTest) |
| Versioning | Git + Conventional Commits |

## 📋 Prérequis

- **C++17** compiler (MSVC 2019+, MinGW, Clang)
- **Qt 6.0+** avec modules : Core, Gui, Widgets, Network, Sql
- **CMake 3.16+**

## 🚀 Quick Start

### 1. Compiler le projet
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### 2. Lancer le serveur (Terminal 1)
```bash
./Mess --server
# Console: Server started on port 12345
```

### 3. Lancer le (ou les) client(s) (Terminal 2+)
```bash
./Mess
# Interface de login → entrez pseudo et mot de passe
```

## 📁 Organisation des Dossiers

```
WizzMania/
├── server/                # Serveur TCP/IP centralisé
│   ├── Server.h/cpp       # Classe Server
│   └── database.hpp       # SQLite (users, messages)
├── client/src/back/       # Logique réseau client
│   └── ClientNetwork.h/cpp
├── client/src/front/      # Interface Qt6
│   ├── LoginWindow.h/cpp
│   ├── MainWindow.h/cpp
│   └── MessageWindow.h/cpp
├── src/common/            # Code partagé
│   ├── Message.h/cpp
│   └── Protocol.h
└── tests/                 # Tests unitaires
```

## 📡 Protocole Réseau

### Types de Messages
- `LoginRequest (1)` : username + password
- `LoginResponse (2)` : succès ou liste utilisateurs
- `ChatMessage (10)` : message texte
- `ChatBroadcast (11)` : reception message
- `Wizz (20)` : effet wizz
- `UserJoin (30)` : nouvel utilisateur
- `UserLeave (31)` : utilisateur parti

## 🧪 Tests

```bash
cmake --build . --config Debug
./TestMessage              # Test sérialisation
```

## 🎮 Convention Git

### Branches
- `main` - Version stable
- `develop` - Branche intégration
- `server` / `back` / `front` - Développement spécialisé

### Commits
```
feat(server): nouvelle fonctionnalité
fix(back): correction bug
test(front): ajout tests
docs: mise à jour documentation
```

## 🚧 Fonctionnalités

- [x] Chat multi-utilisateurs
- [x] Authentification (login/password)
- [x] Effet Wizz (animation)
- [x] Liste utilisateurs connectés
- [ ] Salons privés (rooms)
- [ ] Messages privés (1-to-1)
- [ ] Jeux intégrés
- [ ] Chiffrement SSL/TLS

## 📚 Documentation Complète

Voir [README_COMPLETE.md](README_COMPLETE.md) pour l'architecture détaillée, conventions de code, et ressources.
