# parking-projet
# 🚗 Système de Gestion de Parking Intelligent (Smart Parking System)
**Licence IDAI 2025-2026**

---

## 👥 Membres du Groupe
* **Imane Rhanebou**
* **Mohamed Zarki**
* **Abdallaoui Alaoui Mohamed**

---

## 📝 Présentation du Projet
Ce projet implémente une simulation interactive de gestion de parking intelligent, intégrant des véhicules thermiques (**ICE**) et électriques (**EV**). Développé en **C++** avec la bibliothèque graphique **Raylib**, il illustre la gestion des zones de recharge et des politiques de tarification dynamique en temps réel.

Le projet repose sur une architecture robuste respectant les principes de la **Programmation Orientée Objet (POO)** et l'utilisation de **Design Patterns** pour assurer la modularité du code.
## 💻 Concepts POO Appliqués
* **Héritage :** Utilisation d'une classe `Vehicle` pour les propriétés de base, avec des spécialisations `ICEVehicle` et `EVVehicle`.
* **Polymorphisme :** Gestion différenciée des comportements de recharge et de facturation.
* **Encapsulation :** Protection des données liées aux transactions et aux états des places de stationnement.
* **Parking Fury**
Parking Fury est une simulation interactive de conduite et de stationnement développée en C++ avec Raylib. Le jeu place l'utilisateur aux commandes d'une flotte de véhicules dans un environnement urbain dense où la précision et la gestion stratégique sont de mise.

Contrairement aux jeux de parking classiques, cette version "Smart" intègre des mécaniques de gestion réelle :

Navigation Top-Down : Une vue de dessus immersive pour un contrôle total des trajectoires des véhicules.

Écosystème Mixte : Le joueur doit gérer non seulement le stationnement des voitures thermiques (ICE), mais aussi les besoins spécifiques des véhicules électriques (EV).

Défi de Recharge : Le nom "Fury" reflète l'aspect dynamique de la gestion des places : il faut trouver et manoeuvrer vers les Spots 4, 5 ou 6 pour recharger les batteries avant l'épuisement, tout en surveillant les coûts accumulés.

---

## 🏗️ Architecture Technique & Design Patterns
Le code est organisé de manière modulaire pour séparer la logique métier de la visualisation :

| Répertoire | Contenu | Concepts POO / Design Patterns |
| :--- | :--- | :--- |
| `include/` | Fichiers d'en-tête (`.h`) pour les classes (`Car`, `ParkingZone`, etc.) | **Encapsulation** & **Héritage** (base `Vehicle`). |
| `src/` | Fichiers d'implémentation (`.cpp`) des classes | **Polymorphisme** (calcul des tarifs par zone). |
| `config/` | Paramètres du système (Tarifs, coordonnées) | **Singleton** (pour `ConfigManager`). |
| `demos/` | Point d'entrée (`main.cpp`) utilisant la classe `Game` | **State Pattern** (gestion des écrans de jeu). |
| `tests/` | Code source des tests unitaires | Validation de la logique de stationnement. |

---

## 🎮 Guide de l'Utilisateur & Interface (HUD)

### 🕹️ Commandes de Pilotage
* **Flèches Directionnelles :** Déplacer le véhicule sélectionné vers les zones de parking.
* **Touche TAB :** Changer de véhicule (Cycle entre Car 0, 1, 2 et 3).
* **Boutons HUD (Haut) :** Icônes interactives pour la **Pause (||)**, le **Son** et le **Retour**.
* **Contrôles Tactiles :** Pavé directionnel coloré à droite de l'écran pour une navigation à la souris.

### 📊 Tarification Dynamique des Zones
Le simulateur gère trois zones distinctes avec des tarifs appliqués automatiquement :
* **Parking 1 (Bleu) :** Zone standard — **1.50 dh/h**.
* **Parking 2 (Rouge) :** Zone Premium — **5.00 dh/h**.
* **EV Station (Vert) :** Zone de recharge dédiée — **0.45 dh/h** (Spots 4, 5, 6).
### 🔘 Boutons de l'Interface (HUD)
Situés en haut et à gauche de l'écran :
* **Icône Haut-Parleur :** Activer ou couper les effets sonores de la simulation.
* **Icône Flèche Retour :** Réinitialiser la position des véhicules ou revenir au menu principal.
* **Icône Pause (||) :** Mettre la simulation en pause pour analyser l'occupation des parkings.
* **Boutons Tactiles (Violet/Vert/Rouge/Bleu) :** Une réplique des contrôles directionnels est disponible à l'écran pour une utilisation à la souris.
*  ## 🛠 Technologies Used
Le projet repose sur une stack technologique optimisée pour la performance et le rendu en temps réel :

* **C++ :** Langage de programmation principal, utilisé pour implémenter la logique métier complexe, la gestion de la mémoire et les principes de la POO (Héritage, Encapsulation, Polymorphisme).
* **Raylib :** Bibliothèque graphique 2D utilisée pour le rendu du jeu, la gestion des entrées utilisateur (clavier/souris) et la gestion fluide des animations de véhicules.
# 🎬 Démonstration## 🎥 Démonstration Vidéo (Full Walkthrough)
La démonstration complète dure **3 min 19s**. Elle couvre l'intégralité du cycle de vie du stationnement et de la recharge.

### ⏱️ Chapitres de la vidéo :
* **0:00 - 1:00 :** Initialisation du système et présentation de l'interface Raylib.
* **1:00 - 2:00 :** Navigation des véhicules thermiques (ICE) et tarification.
* **2:00 - 3:19 :** Gestion des véhicules électriques (EV), recharge sur les spots 4-6 et conclusion.


[Cliquez ici pour voir la vidéo de démonstration]

https://drive.google.com/file/d/1qqBNDfhHjkhifUghRueCtFLiz-fj55cg/view?usp=sharing
## 📄 Rapport du Projet
Le rapport détaillé (conception, diagrammes et choix techniques) est disponible ici :
👉 [**Télécharger le Rapport PDF (947 KB)**]https://github.com/imane-prog/parking-projet/blob/main/rapport%20parking%20projet.pdf 


---

## 🛠️ Compilation et Exécution
Le projet inclut un `Makefile` pour automatiser la construction.

### 1. Prérequis
* Compilateur **g++** (C++11 ou supérieur).
* Bibliothèque **Raylib** installée (chemin par défaut : `/usr/local/raylib/`).

### 2. Instructions de compilation
```bash
make clean
make demo
# Exécuter l'application :

./smart_parking_demo


### ⚖️ License
Ce projet est sous licence **MIT**. Vous pouvez consulter le fichier [LICENSE](LICENSE) pour plus de détails sur les droits d'utilisation et de reproduction.









 
