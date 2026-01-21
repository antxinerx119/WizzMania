# WIZZ Mania

MSN est de retour !

## Introduction

Dans les années 2000, l’avènement d’internet a vu émerger des outils de communication et le plus populaire d’entre eux était MSN Messenger et son mythique wizz.

Cet outil utilise les communications via réseau en protocole TCP/IP. Le protocole TCP/IP est l'un des protocoles fondamentaux permettant des communications fiables entre des machines sur un réseau. Ce projet propose de mettre en œuvre une application de chat en temps réel utilisant ce protocole.

## Contexte

L’objectif de ce sujet est de réaliser une application de chat à l’aide de TCP/IP et de C++.

Vous allez devoir créer deux applications :
- **Serveur** : un serveur centralisé capable de gérer plusieurs connexions de clients.
- **Client** : une application cliente capable de se connecter au serveur et d'envoyer/recevoir des messages.

Sans oublier de créer une interface pour visualiser l’ensemble :
- **Interface Utilisateur** : ajouter une interface utilisateur simple en utilisant Qt pour rendre l'application plus conviviale.

À noter que vous allez devoir créer 2 projets : 1 pour le client et 1 pour le serveur, même si une partie du code est commune. Vous pouvez éventuellement faire un seul projet qui vous demande le profil de l'application lancée, mais vous devrez être capable de démarrer plusieurs fois le projet pour faire communiquer le client et le serveur ensemble.

## Technologies Utilisées

- C++
- Librairie STL
- POO (Programmation Orientée Objet)
- Librairie Winsock de Windows
- Qt pour l'interface utilisateur
- Tests Unitaires

## Structure du Projet

Le projet est séparé en 3 parties :
- **front** : Interface utilisateur (Qt)
- **back** : Logique métier
- **server** : Serveur de chat

## Exigences du Serveur

Le serveur doit :
- Gérer plusieurs connexions de clients simultanément.
- Utiliser le protocole TCP/IP pour les communications.
- Permettre l'envoi et la réception de messages en temps réel.
- Supporter la fonctionnalité "wizz" (notification spéciale).
- Être capable de gérer les déconnexions et reconnexions des clients.

## Installation et Configuration

### Prérequis
- Windows (pour Winsock)
- Qt installé
- Compilateur C++ (e.g., Visual Studio ou MinGW)

### Compilation
- Utilisez CMake pour configurer le projet.
- Compilez séparément le client et le serveur.

### Lancement
- Lancez d'abord le serveur.
- Lancez ensuite un ou plusieurs clients pour vous connecter au serveur.

## Tests

- Les tests unitaires sont dans le dossier `tests`.
- Utilisez un framework comme Google Test pour les tests.

## Aller Plus Loin

Amusez vos utilisateurs et donnez leur la possibilité de jouer à des jeux ou applications intégrés qui sont accessibles par toutes les personnes du chat. Vous devez les développer de A à Z.

## Compétences Visées

- Installer et configurer son environnement de travail en fonction du projet.
- Développer des interfaces utilisateur
- Développer des composants métier
- Contribuer à la gestion d'un projet informatique
- Analyser les besoins et maquetter une application
- Définir l'architecture logicielle d'une application
- Préparer et exécuter les plans de tests d'une application
