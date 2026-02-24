# BUGS CORRIGÉS et TODO TRACKER

## ✅ BUGS CORRIGÉS (Étape 2)

### Incohérence de l'API Serveur
- ✅ **Suppression** de l'ancien header `server.hpp` (version incohérente)
- ✅ **Unification** vers `Server.h/Server.cpp` (version cohérente et avancée)
- ✅ **Nettoyage** du CMakeLists.txt et main.cpp

### Gestion du Mot de Passe
- ✅ Ajout du champ mot de passe dans `LoginWindow` (Qt)
- ✅ Transmission du mot de passe du client au serveur
- ✅ Implémentation de l'authentification côté serveur (vérification SQLite)
- ✅ Extraction et parsing du mot de passe dans `handleLogin()`

### Bufferisation des Messages Réseau
- ✅ Ajout d'un buffer de lecture par client (`m_clientReadBuffers`)
- ✅ Implémentation de `readMessageFromBuffer()` côté serveur
- ✅ Modification de `onReadyRead()` pour utiliser le buffer
- ✅ Extraction correcte des messages (vérification de la taille totale)
- ✅ Nettoyage du buffer lors de la déconnexion

### Effet Wizz Visuel
- ✅ Ajout de `QPropertyAnimation` et d'une Q_PROPERTY dans `MainWindow`
- ✅ Implémentation de `performWizzAnimation()` avec shake effect
- ✅ Animation fluide (5 mouvements latéraux + retour à l'origine)
- ✅ Support de plusieurs wizz consécutifs

### Affichage de la Liste des Utilisateurs
- ✅ Ajout de `QListWidget` dans `MessageWindow`
- ✅ Création de `updateUserList()` pour mettre à jour la liste en temps réel
- ✅ Layout avec splitter (chat / liste utilisateurs)
- ✅ Intégration avec les notifications `UserJoin` et `UserLeave`

### Documentation
- ✅ Création du `README_COMPLETE.md` avec architecture complète
- ✅ Création du `README_NEW.md` avec quick start et guide rapide
- ✅ Description du protocole réseau
- ✅ Convention Git (Conventional Commits)
- ✅ Instructions d'installation et de compilation

---

## 📋 TODO - ÉTAPE 3 : Tests Unitaires

### Serveur (`server/`)
- [ ] `TestServerStart` - Démarrage serveur sur port 12345
- [ ] `TestServerAcceptClient` - Acceptation d'une connexion client
- [ ] `TestServerMultipleClients` - Gestion de 3+ clients simultanément
- [ ] `TestBroadcast` - Envoi d'un message à tous les clients
- [ ] `TestLogin` - Authentification réussie/échouée
- [ ] `TestLoginDuplicateUsername` - Rejet d'un username déjà pris
- [ ] `TestRoomBroadcast` - Broadcast dans une room spécifique
- [ ] `TestDisconnection` - Notification UserLeave correcte
- [ ] `TestWizzBroadcast` - Envoi d'un Wizz à tous

### Backend Client (`client/src/back/`)
- [ ] `TestClientConnect` - Connexion au serveur
- [ ] `TestClientDisconnect` - Déconnexion propre
- [ ] `TestClientSendMessage` - Envoi d'un message
- [ ] `TestClientReceiveMessage` - Réception et parsing d'un message
- [ ] `TestLoginRequestMessage` - Sérialisation du login
- [ ] `TestMessageSerialization` - Sérialisation/désérialisation
- [ ] `TestWizzMessage` - Envoi et réception d'un Wizz
- [ ] `TestClientBuffer` - Bufferisation des données incomplètes

### Frontend (`client/src/front/`)
- [ ] `TestLoginWindowInput` - Validation des champs login/password
- [ ] `TestMessageWindowDisplay` - Affichage des messages
- [ ] `TestWizzAnimation` - Animation shake effect
- [ ] `TestUserListWidget` - Mise à jour de la liste utilisateurs
- [ ] `TestMainWindowNavigation` - Transition login → chat

---

## 📋 TODO - ÉTAPE 4 : Fonctionnalités Manquantes

### Rooms/Salons (Priorité Haute)
- [ ] Table `rooms` dans SQLite
- [ ] Message type `CreateRoom` / `JoinRoom` / `LeaveRoom`
- [ ] Gestion des rooms côté serveur
- [ ] UI pour créer/rejoindre une room
- [ ] Broadcast limité à une room spécifique
- [ ] Affichage de la room actuelle dans le chat

### Messages Privés (Priorité Moyenne)
- [ ] Message type `PrivateMessage`
- [ ] Routing du serveur (socket spécifique)
- [ ] Onglet "Messages privés" dans le front
- [ ] Notification de message privé reçu

### Persistance des Messages (Priorité Moyenne)
- [ ] Historique SQLite (id, sender, content, timestamp, room_id)
- [ ] Chargement de l'historique à la connexion
- [ ] Affichage des messages précédents

### Statuts Utilisateurs (Priorité Basse)
- [ ] Enum : Online / Away / Busy / Offline
- [ ] Message type `SetStatus`
- [ ] Indicator visuel dans la liste utilisateurs
- [ ] Persistance du statut

---

## 📋 TODO - ÉTAPE 5 : Améliorations / Nice-to-Have

### Sécurité
- [ ] Hachage des passwords (bcrypt/PBKDF2 au lieu de stockage brut)
- [ ] Support SSL/TLS pour les connexions réseau
- [ ] Token JWT ou sessions
- [ ] Validation/sanitization des inputs

### Expérience Utilisateur
- [ ] Son de notification à la réception d'un message
- [ ] Timestamps sur les messages
- [ ] Styles CSS / thème clair/sombre
- [ ] Emojis support
- [ ] Indicateur "utilisateur en train d'écrire"

### Jeux Intégrés
- [ ] Tic-Tac-Toe
- [ ] Morpion
- [ ] Pendu
- [ ] Système de scoring

### Performance & Scalabilité
- [ ] Utilisation de `std::thread` pour multi-threading serveur
- [ ] `std::mutex` pour synchronisation des ressources partagées
- [ ] Thread pool pour gestion des connexions
- [ ] Bench performance (peak clients, messages/sec)

### Logs & Debugging
- [ ] Serilog ou semblable pour logging
- [ ] Niveau de log configurable (DEBUG, INFO, WARN, ERROR)
- [ ] Logs d'audit (qui s'est connecté, quand, etc.)

---

## 🔄 État du Projet

| Phase | Statut | Pourcentage |
|-------|--------|------------|
| Analyse complète | ✅ Done | 100% |
| Corrections bugs | ✅ Done | 100% |
| Tests unitaires | ⏳ TODO | 0% |
| Fonctionnalités manquantes | ⏳ TODO | 0% |
| **TOTAL** | 🔄 In Progress | **50%** |

---

## 📌 Prochaines Étapes

1. **Compiler le projet** avec les fixes appliquées
2. **Lancer les tests existants** (test_message)
3. **Créer les tests unitaires** (Google Test framework)
4. **Implémenter les rooms** (fonctionnalité prioritaire)
5. **Ajouter la persistance des messages**
6. **Optimiser la sécurité** (passwords hachés, SSL/TLS)

---

Generated: 2026-02-23
