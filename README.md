# presentationCAN
Présentation faite par E. Goudot du Fablab Robert-Houdin, au lycée Camille Claudel, suite à la demande du professeur Jérôme Nouveau.
Faite le 10 avr 2015 (16h-17h)

Ce projet contient les sources et documentations de la démonstration sur table qui consistais en:

- un Arduino Due ayant 
    une interface Bluetooth, relié à un téléphone (Android)
    une interface CAN, pour envoi de message vers un autre calculateur (PIC)
- une carte de développement basée sur un PIC (18F24K80) avec interface CAN
    les messages CAN reçu servant à alumer une LED RGB de la couleur fournie par l'appli Android.

Les dossiers sont:

* ColorController *
Contient l'application Android (avec Eclipse 'adt-bundle-windows-x86')
L'APK se trouve dans le sous r�pertoire bin.
Il faut avoir li� l'interface Bluetooth (SPA311) avant de pouvoir la selectionner.
L'application est un mix de r�cup trouv�e sur internet, surtout pour la partie communication BT.

