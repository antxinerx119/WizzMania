# GUIDE DE TESTING & DÉPLOIEMENT - WIZZ Mania

## 🧪 Testing Complet

### 1. Tests Unitaires (Google Test)

#### Compiler les tests
```bash
cd build
cmake --build . --config Debug
```

#### Lancer tous les tests
```bash
./TestMessage           # Sérialisation des messages
./TestServer           # Serveur (connexions, broadcast, etc.)
./TestClientNetwork    # Client (logique réseau)
```

#### Résultats attendus
```
[==========] Running 3 test suites (X tests).
[----------] Global test environment set-up.
[..............] X tests from TestMessage (X ms total)
[..............] X tests from TestServer (X ms total)
[..............] X tests from TestClientNetwork (X ms total)
[==========] 3 test suites passed. X tests from 3 suites ran.
[  PASSED  ] X tests.
[  FAILED  ] 0 tests.
```

### 2. Tests Manuels (Integration Testing)

#### Scénario 1 : Connexion et Chat Simple

**Setup:**
- Terminal 1 : `./Mess --server`
- Terminal 2 : `./Mess` → Alice / pass1
- Terminal 3 : `./Mess` → Bob / pass2

**Test:**
1. Alice envoie "Hello Bob!" → Bob reçoit le message
2. Bob reçoit la notification "Alice joined"
3. Fermer Alice → Bob reçoit "Alice left"

**Résultat Attendu:** ✅ All messages reçus sans erreur

#### Scénario 2 : Effet Wizz

**Setup:**
- Serveur + 2 clients comme ci-dessus

**Test:**
1. Alice clique sur "Wizz!"
2. Bob reçoit la notification "Alice sent you a Wizz"
3. Fenêtre de Bob tremble pendant ~500ms

**Résultat Attendu:** ✅ Animation shake effect

#### Scénario 3 : Multiple Clients

**Setup:**
- Terminal 1 : `./Mess --server`
- Terminal 2-5 : `./Mess` avec différents utilisateurs

**Test:**
1. Tous les (n) clients envoient des messages
2. Vérifier que chacun reçoit les (n-1) messages autres
3. Chaque client quitte successivement

**Résultat Attendu:** ✅ Tous les clients communiquent sans perte de message

#### Scénario 4 : Reconnexion

**Setup:**
- Serveur + Alice connectée

**Test:**
1. Alice envoie un message
2. Fermer Alice brutalement (kill -9)
3. Relancer Alice avec même username
4. Vérifier reconnexion réussie

**Résultat Attendu:** ✅ Reconnexion sans crash serveur

### 3. Tests de Performance

#### Benchmark Throughput

```bash
./Mess --benchmark --duration 10 --num-clients 5 --msg-per-sec 100
# Résultat: X messages/sec, Y% packet loss, Z ms avg latency
```

#### Stress Test

```bash
./Mess --stress --num-clients 50 --duration 60
# Montée en charge progressive, vérifier:
# - CPU < 80%
# - Memory < 500MB
# - Pas de deadlock
# - Pas de message loss
```

---

## 🐛 Debugging

### Activer les Logs Détaillés

#### Modifier `src/common/Protocol.h`
```cpp
#define DEBUG_PROTOCOL 1  // Enable detailed logging
```

#### Recompiler
```bash
cmake --build .
```

#### Logs générés
```
[Server] New client connected from 127.0.0.1:54321
[Server] Received LoginRequest from socket#1
[Server] User 'Alice' authenticated successfully
[Server] Broadcasting UserJoin to 4 clients
[Client] Message received: ChatBroadcast from Alice
```

### Utiliser QtCreator Debugger

```bash
# Générer build avec symboles de debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

# Ouvrir dans QtCreator et utiliser le debugger graphique
```

### Breakpoints

Dans QtCreator ou gdb :
```
break server/Server.cpp:128    # handleLogin()
break client/src/back/ClientNetwork.cpp:45  # sendMessage()
continue
```

### Wireshark (TCP Protocol Analysis)

```bash
# Capturer le trafic TCP port 12345
wireshark --capture-filter "tcp port 12345" &

# Générer du trafic
./Mess --server &
./Mess &

# Analyser les packets dans Wireshark
# - Vérifier le MAGIC_NUMBER (57495A5A)
# - Vérifier la taille des payloads
# - Vérifier les message types
```

---

## 📦 Packaging & Distribution

### Créer un Release

#### 1. Mettre à jour Version
```
CMakeLists.txt:
project(Mess VERSION 1.0.0 LANGUAGES CXX)

BUGFIX_AND_TODO.md:
Version: 1.0.0 (2026-02-23)
```

#### 2. Créer CHANGELOG
```markdown
# CHANGELOG - v1.0.0

## New Features
- [x] Multi-client chat with TCP/IP
- [x] User authentication (login/password)
- [x] Wizz animation effect
- [x] Real-time user list

## Bug Fixes
- Fixed inconsistent server header
- Fixed password handling in login
- Implemented proper message buffering
- Added visual effect for Wizz

## Performance
- ~1000 messages/sec per client
- <500MB memory for 50 clients

## Known Issues
- No persistent message history yet
- No private rooms yet
```

#### 3. Tag Git Release
```bash
git tag -a v1.0.0 -m "Release v1.0.0 - Stable Production"
git push origin v1.0.0
```

#### 4. Build Release
```bash
# Windows
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release

# Créer un ZIP
mkdir wizz-mania-1.0.0
cp Mess.exe wizz-mania-1.0.0/
cp README.md wizz-mania-1.0.0/
cp PROTOCOL_SPEC.md wizz-mania-1.0.0/
7z a wizz-mania-1.0.0.zip wizz-mania-1.0.0/
```

#### 5. Upload
```bash
# GitHub Releases
gh release create v1.0.0 wizz-mania-1.0.0.zip

# Vérifier
gh release list
```

---

## 🚀 Déploiement en Production

### Checklist Pre-Deployment

- [ ] Tous les tests passent (unit + integration)
- [ ] Code reviewed et approuvé
- [ ] No compiler warnings
- [ ] Performance benchmark OK
- [ ] Documentation à jour
- [ ] CHANGELOG rédigé
- [ ] Version bumpée
- [ ] Git tagged

### Configuration de Production

#### server/Server.cpp
```cpp
const int PROD_PORT = 12345;
const bool LOG_DEBUG = false;  // Désactiver logs verbose
const int MAX_CLIENTS = 1000;
```

#### Lancement du Serveur
```bash
# En daemon/background service
nohup ./Mess --server > server.log 2>&1 &

# Ou via systemd (Linux)
[Unit]
Description=WIZZ Mania Server
After=network.target

[Service]
Type=simple
ExecStart=/opt/wizz-mania/Mess --server
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target

# Activer
sudo systemctl enable wizz-mania
sudo systemctl start wizz-mania
```

### Monitoring de Serveur

#### Vérifier l'État
```bash
# Port écoute
netstat -tlnp | grep 12345

# Processus
ps aux | grep "Mess --server"

# Logs
tail -f server.log

# Nombre de clients connectés
netstat -tnp | grep 12345 | wc -l
```

#### Redémarrage Graceful
```bash
# Envoyer signal SIGTERM (graceful)
kill -TERM <pid>

# Attendre que les clients se déconnectent
# Puis relancer

# Éviter SIGKILL qui ferme les connexions brutalement
kill -9 <pid>  # ❌ MAUVAIS - Peut causer corruption DB
```

### Backup de Database

```bash
# Copier la base SQLite
cp server/wizzmania.db server/wizzmania.db.backup

# Ou ajouter au cron (quotidien)
0 2 * * * cp /opt/wizz-mania/wizzmania.db /backups/wizzmania-$(date +\%Y\%m\%d).db
```

---

## 📊 Monitoring & Logs

### Activer les Statistiques

```cpp
// server/Server.cpp
struct ServerStats {
    int totalConnects = 0;
    int peakConnected = 0;
    int totalMessages = 0;
    int failedLogins = 0;
};

void Server::logStats() {
    qDebug() << "=== Server Statistics ===";
    qDebug() << "Total connections:" << m_stats.totalConnects;
    qDebug() << "Peak simultaneous:" << m_stats.peakConnected;
    qDebug() << "Total messages:" << m_stats.totalMessages;
    qDebug() << "Failed logins:" << m_stats.failedLogins;
}

// Appeler toutes les heures
QTimer *statsTimer = new QTimer(this);
connect(statsTimer, &QTimer::timeout, this, &Server::logStats);
statsTimer->start(3600000);  // 1 heure
```

### Log Rotation

```bash
# Utiliser logrotate (Linux)
/opt/wizz-mania/server.log {
    daily
    rotate 7
    compress
    delaycompress
    notifempty
    create 0640 wizz-mania wizz-mania
    postrotate
        systemctl reload wizz-mania
    endscript
}
```

---

## 🔒 Sécurité en Production

### Checklist Sécurité

- [ ] Passwords hachés (bcrypt/PBKDF2 et non en clair)
- [ ] SSL/TLS activé pour les connexions
- [ ] Rate limiting anti-brute-force
- [ ] Input validation / sanitization
- [ ] SQL injection prevention (utilisrer parameterized queries)
- [ ] Firewall configuré (port 12345 limité)
- [ ] Logs d'audit activés
- [ ] Backup de DB réguliers

### Exemple Hash Password (Future)

```cpp
#include <bcrypt/bcrypt.h>

// Lors de l'enregistrement
std::string hashed = bcrypt::generate_hash(password);
m_db.saveHashedPassword(username, hashed);

// Lors du login
if (bcrypt::validate_password(password, m_db.getHashedPassword(username))) {
    // Authentification réussie
}
```

---

## 🆘 Troubleshooting Production

### Le Serveur Crash

**Symptôme** : Serveur arrête répondre

**Diagnostic** :
```bash
tail -100 server.log  # Vérifier les derniers logs
dmesg | tail -50      # Kernel messages
free -h               # Mémoire disponible
```

**Solutions** :
- Augmenter la mémoire allouée
- Réduire le nombre max de clients
- Vérifier les fuites mémoire (valgrind)

### Les Clients ne Peuvent pas se Connecter

**Symptôme** : "Connection refused"

**Diagnostic** :
```bash
netstat -tlnp | grep 12345
ps aux | grep "Mess --server"
```

**Solutions** :
- Vérifier que le serveur est lancé
- Vérifier le firewall
- Vérifier les permissions réseau

### Slowndown / Lag

**Symptôme** : Délai de réception des messages

**Diagnostic** :
```bash
# Latence TCP
ping <server-ip>
nstat -r  # Network stats

# Charge CPU
top
htop
```

**Solutions** :
- Optimiser le code (voir PROTOCOL_SPEC.md)
- Augmenter les ressources serveur
- Implémenter la mise en cache (caching)

---

## 📈 Scaling Future

### Pour 10 000+ Clients

1. **Architecture** : Passer de single-threaded à thread pool
2. **Database** : Migrer de SQLite à PostgreSQL/MySQL
3. **Load Balancer** : Ajouter un load balancer (Nginx)
4. **Microservices** : Séparer serveur en modules indépendants
5. **Caching** : Redis pour l'user list et message cache
6. **Message Queue** : RabbitMQ pour async processing

### Exemple: Scale avec Docker

```dockerfile
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y qt6-base-dev cmake
WORKDIR /app
COPY . .
RUN mkdir build && cd build && cmake .. && cmake --build .
EXPOSE 12345
CMD ["./Mess", "--server"]
```

```bash
# Lancer 3 instances serveur
docker run -p 12345:12345 wizz-mania:1.0.0 &
docker run -p 12346:12345 wizz-mania:1.0.0 &  # Port 12346 → 12345 in container
docker run -p 12347:12345 wizz-mania:1.0.0 &

# Nginx load balance les 3 instances
```

---

**Guide Testing & Deployment v1**  
Last Updated: 2026-02-23
