# Compteur_geiger
Projet arduino avec tube J305 sensible aux rayons beta et gamma et baromètre BME280. Les mesures sont envoyées sur thingspeak.com.

On utilise un arduino uno wifi rev3, connecté en i2c à un module baromètre/hygromètre/thermomètre BME280, et par une simple broche à un module geiger comportant un tube J305, sensible aux rayons gamma (photon de haute énergie) et beta (electron).

L'arduino compte les impacts détectés par le module geiger, et envoie les données au site thngspeak.com (https://thingspeak.com/channels/1092322 pour mon compte).

Le site produit des graphiques, et tweete tous les matins un compte rendu des radiations détectées dans la journée.
("Compteur Geiger - Montpellier" pour mon compte twitter).
