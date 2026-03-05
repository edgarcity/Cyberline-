# Cyberline — Robot suiveur de ligne autonome

Robot mobile autonome conçu et fabriqué entièrement de A à Z dans le cadre d'un challenge scolaire. Inspiré du design angulaire du Cybertruck de Tesla, il suit une ligne noire sur un parcours balisé par 6 marqueurs, détecte les obstacles et s'arrête avant toute collision. Une application Android permet de lui envoyer une destination via Bluetooth.

## Fonctionnement

- L'utilisateur sélectionne une porte cible (1 à 6) depuis l'application Android via Bluetooth
- Le robot suit la ligne jusqu'au marqueur correspondant
- Il s'immobilise 5 secondes en activant le buzzer, puis reprend sa route
- En cas d'obstacle détecté, il s'arrête et émet un signal sonore

## Matériel

- Arduino Uno (firmware C++ maison)
- Capteur infrarouge Maker Line 5 canaux (suivi de ligne)
- Capteur ultrasonique HC-SR04 (détection d'obstacle)
- Pont en H L298N (commande des moteurs DC)
- Module Bluetooth HC-05
- Batterie NiMH 7.2V
- 12 pièces carrosserie modélisées sur Fusion 360 et imprimées en 3D

## Contenu du dépôt

- `docs/` → documentation complète du projet (PDF)
- `code/` → firmware C++ pour Arduino Uno
- `3D/` → fichiers des 12 pièces modélisées sur Fusion 360
- `app/` → application Android (MIT App Inventor)

## Bilan

- Robot fonctionnel en conditions réelles de challenge
- Design 100% original inspiré du Tesla Cybertruck
- Aucune pièce standard : tout modélisé et imprimé
- Application mobile dédiée pour le contrôle Bluetooth
- Compétences : CAO Fusion 360, électronique, C++ embarqué, développement Android

## Licence

[CC BY-NC 4.0](https://creativecommons.org/licenses/by-nc/4.0/) — Utilisation libre et modification autorisées, usage commercial interdit.
