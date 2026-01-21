# Architecture du Projet WIZZ Mania

## Diagramme d'Architecture

```
+-------------------+       TCP/IP       +-------------------+
|                   | <----------------> |                   |
|     Client        |                    |     Serveur       |
|                   |                    |                   |
| +-------------+   |                    | +-------------+   |
| |    Front    |   |                    | |   Gestion   |   |
| |    (Qt)     |   |                    | | Connexions  |   |
| +-------------+   |                    | +-------------+   |
|                   |                    |                   |
| +-------------+   |                    | +-------------+   |
| |    Back     |   |                    | |   Logique   |   |
| | (Logique)   |   |                    | |   Chat      |   |
| +-------------+   |                    | +-------------+   |
|                   |                    |                   |
+-------------------+                    +-------------------+
         |                                       |
         |                                       |
         v                                       v
+-------------------+                    +-------------------+
|                   |                    |                   |
|   Interface       |                    |   Base de         |
|   Utilisateur     |                    |   Données         |
|   (Qt Widgets)    |                    |   (optionnel)     |
+-------------------+                    +-------------------+
```

## Description des Composants

### Client
- **Front (Qt)** : Interface utilisateur graphique pour l'interaction avec l'utilisateur (fenêtres, boutons, zones de texte pour les messages).
- **Back (Logique)** : Gestion de la logique métier côté client, comme la préparation des messages, gestion des connexions TCP/IP, traitement des "wizz".

### Serveur
- **Gestion des Connexions** : Utilise Winsock pour accepter et gérer plusieurs connexions clients simultanément.
- **Logique Chat** : Traite les messages reçus, les diffuse aux autres clients, gère les fonctionnalités spéciales comme le "wizz".

### Communication
- Protocole TCP/IP via Winsock pour une communication fiable entre client et serveur.
- Le serveur agit comme un hub central pour relayer les messages entre clients.

### Partie Commune (Shared)
- Classes utilitaires, définitions communes, etc.

## Flux de Données
1. Le client se connecte au serveur via TCP/IP.
2. Le client envoie des messages ou des commandes (e.g., wizz) au serveur.
3. Le serveur reçoit, traite et diffuse les messages à tous les clients connectés.
4. Les clients reçoivent et affichent les messages via l'interface Qt.

## Technologies
- C++ avec STL
- Winsock pour le réseau
- Qt pour l'UI
- Programmation Orientée Objet (POO)
