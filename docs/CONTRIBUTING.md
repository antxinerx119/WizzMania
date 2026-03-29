# GUIDE DE CONTRIBUTION - WIZZ Mania

## 🎯 Avant de Commencer

1. Lire cette documentation complètement
2. Consulter `README.md` et `docs/` pour l'etat actuel du projet
3. S'assurer que l'environnement de développement est configuré (Qt6, CMake)
4. Créer une branche pour votre travail

## 🌿 Gestion des Branches

### Nomenclature
```
feature/nom-de-la-fonctionnalité    # Nouvelle fonctionnalité
bugfix/nom-du-bug                   # Correction d'un bug
hotfix/nom-urgent                   # Correction urgente en production
test/nom-du-test                    # Ajout de tests
docs/nom-de-la-doc                  # Documentation
chore/nettoyage                     # Maintenance, cleanup
```

### Branches principales
- **main** : version stable, production-ready
- **develop** : branche d'intégration, build en continu
- **server** / **back** / **front** : branchespécialisées par module

### Workflow
```bash
# 1. Créer une branche à partir de develop
git checkout -b feature/ma-fonctionnalite

# 2. Faire des commits réguliers
git commit -m "feat(server): ajouter gestion rooms"

# 3. Pousser et créer une Pull Request
git push origin feature/ma-fonctionnalite
# → Créer une PR sur GitHub/GitLab vers develop
```

## 📝 Convention des Commits

Format : `<type>(<scope>): <subject>`

### Types
- `feat` : Nouvelle fonctionnalité
- `fix` : Correction de bug
- `test` : Tests ou tests unitaires
- `docs` : Documentation
- `chore` : Maintenance, dépendances, cleanup
- `refactor` : Refactorisation sans changement fonctionnel
- `perf` : Amélioration de performance
- `ci` : Configuration CI/CD

### Scope
- `server` : Code serveur
- `back` : Backend client (ClientNetwork)
- `front` : Frontend (Qt interface)
- `common` : Code partagé
- `test` : Tests
- `build` : CMake, build system
- `docs` : Documentation

### Examples
```
feat(server): implémenter gestion des rooms
fix(back): corriger désérialisation du password
test(server): ajouter tests d'authentification
docs(readme): mettre à jour quick start
chore(build): mettre à jour CMakeLists.txt
refactor(front): simplifier MessageWindow layout
```

## 🛠️ Convention de Code

### C++17 & POO

#### Nommage des Classes
```cpp
// CamelCase, substantif
class ClientNetwork { ... };
class LoginWindow { ... };
class MessageSerializer { ... };
```

#### Nommage des Méthodes
```cpp
// camelCase, verbe pour actions
void connectToServer();
bool isConnected() const;
void sendMessage(const Message& msg);
void onReadyRead();  // Slots : "on" + événement
```

#### Nommage des Variables
```cpp
// camelCase pour locales
QString username = "Alice";
int clientCount = 0;

// m_ pour privées
class ClientNetwork {
private:
    QTcpSocket* m_socket;
    QString m_username;
    QByteArray m_buffer;
};

// Constantes : UPPER_CASE
const quint32 MAGIC_NUMBER = 0x57495A5A;
const int DEFAULT_PORT = 12345;
```

#### Pointeurs et Références
```cpp
// Préférer les smart pointers
std::unique_ptr<QTcpSocket> m_socket;
std::shared_ptr<Database> m_db;

// Au lieu de
QTcpSocket* m_socket;  // ❌ Manual deletion needed

// Références pour les paramètres (pas de copie)
void processMessage(const Message& msg);
void updateUserList(QStringList& users);
```

### Qt Specifiqué

#### Signaux/Slots
```cpp
// Signaux : PascalCase
signals:
    void connected();
    void messageReceived(const Message& msg);
    void userJoined(const QString& username);

// Slots : onEventName
private slots:
    void onConnected();
    void onMessageReceived(const Message& msg);
    void onUserJoined(const QString& username);
```

#### Connexions
```cpp
// Connecter dès que possible
connect(m_socket, &QTcpSocket::connected, this, &ClientNetwork::onConnected);
connect(m_socket, &QTcpSocket::readyRead, this, &ClientNetwork::onReadyRead);
```

#### Layout Qt
```cpp
// Utiliser des layouts, pas de positionnement absolu
auto layout = new QVBoxLayout;
layout->addWidget(m_label);
layout->addWidget(m_input);
layout->addStretch();  // Flexible space
setLayout(layout);
```

### Encapsulation

```cpp
class MyClass {
public:
    // Interface public
    void doSomething();
    int getValue() const { return m_value; }

private:
    // Implémentation privée
    void internalHelper();
    int m_value;
};
```

### Gestion d'Erreurs

```cpp
// Utiliser std::optional pour valeurs optionnelles
std::optional<Message> Message::deserialize(const QByteArray& data) {
    if (data.size() < MIN_SIZE) {
        return std::nullopt;  // Erreur
    }
    // ...
    return Message(...);      // Succès
}

// Utiliser à la réception
auto msgOpt = Message::deserialize(data);
if (msgOpt) {
    Message msg = *msgOpt;
    // ...
} else {
    qDebug() << "Failed to deserialize";
}
```

## 📋 Ajouter une Nouvelle Fonctionnalité

### 1. Créer la branche
```bash
git checkout -b feature/nouvelle-fonctionnalite
```

### 2. Implémenter
```
client/src/back/
    ├── MyNewClass.h
    └── MyNewClass.cpp

ou

server/
    ├── MyNewClass.h
    └── MyNewClass.cpp
```

### 3. Respecter la structure
```cpp
// MyNewClass.h
#ifndef MY_NEW_CLASS_H
#define MY_NEW_CLASS_H

#include <QtCore>

class MyNewClass : public QObject {
    Q_OBJECT
public:
    explicit MyNewClass(QObject* parent = nullptr);

signals:
    void somethingHappened();

private slots:
    void onEvent();

private:
    void helper();
    int m_data;
};

#endif
```

### 4. Ajouter à CMakeLists.txt
```cmake
set(PROJECT_SOURCES
    # ...existing files...
    client/src/back/MyNewClass.h
    client/src/back/MyNewClass.cpp
)
```

### 5. Écrire les tests
```cpp
// tests/test_my_new_class.cpp
#include <gtest/gtest.h>
#include "MyNewClass.h"

class MyNewClassTest : public ::testing::Test { ... };

TEST_F(MyNewClassTest, TestSomething) { ... }
```

### 6. Documenter
- Ajouter des commentaires dans le code
- Mettre à jour README.md si nécessaire
- Ajouter des exemples d'utilisation

### 7. Commit et Push
```bash
git add .
git commit -m "feat(back): ajouter MyNewClass pour ..."
git push origin feature/nouvelle-fonctionnalite
```

### 8. Pull Request
- Décrire les changements
- Lier les issues concernées
- Demander des reviewers

## 🧪 Tests

### Écrire un Test
```cpp
#include <gtest/gtest.h>
#include "MyClass.h"

class MyClassTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup avant chaque test
    }
    void TearDown() override {
        // Cleanup après chaque test
    }
};

TEST_F(MyClassTest, PositiveBehavior) {
    MyClass obj;
    EXPECT_EQ(obj.getValue(), 42);
}

TEST_F(MyClassTest, ErrorHandling) {
    MyClass obj;
    EXPECT_FALSE(obj.process(nullptr));
}
```

### Lancer les Tests
```bash
cd build
cmake --build . --config Debug
ctest --output-on-failure
./TestMessage          # Message serialization
```

### Coverage (optionnel)
```bash
# Compiler avec coverage
cmake -DENABLE_COVERAGE=ON ..
cmake --build .
# Générer rapport
gcov *.cpp
```

## 📖 Documentation du Code

### Commentaires

```cpp
// Bref commentaire pour une ligne
int count = 0;

// Longer explanation for a complex operation
// that spans multiple lines and needs more detail
// about edge cases or performance considerations
int result = complexCalculation();

/**
 * Detailed function documentation using Doxygen style
 * @param msg The message to process
 * @return true if successful, false otherwise
 * @throws MyException if data is invalid
 */
bool processMessage(const Message& msg);
```

### Pas de Commentaires Inutiles
```cpp
// ❌ Redondant
m_username = username;  // Set the username

// ✅ Bon - ne pas commenter l'évident
m_username = username;

// ✅ Commentaire utile
// Normalize username to lowercase for case-insensitive comparison
m_username = username.toLower();
```

## 🔍 Code Review

### Avant de soumettre
- [ ] Code compile sans erreurs/warnings
- [ ] Tests passent
- [ ] Pas de doublons/code mort
- [ ] Conventions respectées
- [ ] Documentation à jour

### Reviews
- Être constructif et respectueux
- Suggérer des améliorations
- Demander des clarifications sur le design

## 🚀 Déploiement

### Build Release
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

### Checklist Pre-Release
- [ ] Tous les tests passent
- [ ] Pas de warnings
- [ ] README à jour
- [ ] CHANGELOG rédigé
- [ ] Version bumpée (X.Y.Z)

## 📞 Support

- **Questions?** Ouvrir une issue
- **Bug trouvé?** Créer une issue avec reproduction steps
- **Amélioration?** Discuter en issue avant de coder

---

Merci de contribuer à WIZZ Mania ! 🚀
