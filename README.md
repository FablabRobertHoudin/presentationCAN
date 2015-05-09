# presentationCAN
Présentation faite par E. Goudot du Fablab Robert-Houdin, suite à la demande du professeur Jérôme Nouveau.
Faite le 10 avr 2015 (16h-17h) au lycée Camille Claudel

Ce projet contient les sources et documentations de la démonstration sur table qui consistais en:

- un Arduino Due ayant 
    une interface Bluetooth, relié à un téléphone (Android)
    une interface CAN, pour envoi de message vers un autre calculateur (PIC)
- une carte de développement basée sur un PIC (18F24K80) avec interface CAN
    les messages CAN reçus servant à alumer une LED RGB de la couleur fournie par l'appli Android.

Les dossiers sont:

- ColorController

Contient l'application Android (avec Eclipse 'adt-bundle-windows-x86')
L'APK se trouve dans le sous répertoire bin.
Il faut avoir lié l'interface Bluetooth (SPA311) avant de pouvoir la selectionner.
L'application est un mix de récup trouvée sur internet, surtout pour la partie communication BT.

- CANmanager

Sketch Arduino (Due) à télécharger dans l'Arduino.
Bibliothèque "Due_CAN" à installer

- PIC18F25K80_LEDrgb.X

Application PIC qui recoit les trames CAN et les interprète pour une LED RGB type PL9823
