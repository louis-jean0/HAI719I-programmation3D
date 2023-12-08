# Commandes pour tester le TP

Si le TP ne fonctionne pas chez vous, merci de lancer ces commandes :

```
template/$ rm -r build
template/$ mkdir build
template/$ cd build
template/build$ cmake ..
template/build$ cd ..
template/$ sh build.sh
```

Pour afficher un rendu Unlit :

- mettre ```Context::rendering_type = Unlit;``` dans main de main.cpp (ligne 194)

Pour afficher un rendu avec Phong :

- mettre ```Context::rendering_type = Phong;``` dans main de main.cpp (ligne 194)

Pour afficher un rendu avec un model réfléchissant :

- mettre ```Context::rendering_type = Reflective;``` dans main de main.cpp (ligne 194)

Pour afficher un rendu PBR :

- mettre ```Context::rendering_type = PBR;``` dans main de main.cpp (ligne 194)

Pour enlever la skybox :

- commenter ```initSkybox();``` dans init() de main.cpp (ligne 114)

Après avoir choisi le type de rendu et build, choisissez le modèle :

- lancer ```sh run.sh data/BoomBox.gltf data/BoomBox_baseColor.png data/BoomBox_normal.png data/BoomBox_occlusionRoughnessMetallic.png``` pour la BoomBox
- lancer ```sh run.sh data/WaterBottle.gltf data/WaterBottle_baseColor.png data/WaterBottle_normal.png data/WaterBottle_occlusionRoughnessMetallic.png``` pour la bouteille d'eau
- lancer ```sh run.sh data/BarramundiFish.gltf data/BarramundiFish_baseColor.png data/BarramundiFish_normal.png data/BarramundiFish_occlusionRoughnessMetallic.png``` pour le Barramundi
- lancer ```sh run.sh data/SciFiHelmet.gltf data/SciFiHelmet_BaseColor.png data/SciFiHelmet_Normal.png data/SciFiHelmet_MetallicRoughness.png``` pour le casque
- lancer ```sh run.sh data/Corset.gltf data/Corset_baseColor.png data/Corset_normal.png data/Corset_occlusionRoughnessMetallic.png``` pour le corset


# Template de TP


## Dépendances

- cmake (Outil pour créer des projets cross-plateforme)
- glm (Bibliothèque de mathématiques adaptées à OpenGL)
- glut
- assimp (Bibliothèque d'import de modèles 3D)

Sur Ubuntu (avec les droits administateurs):
```
apt install cmake freeglut3-dev libassimp-dev libglm-dev
```

Si vous n'avez pas les droits il vous faut compiler les deux sous-modules dans le dossier "thirdparties".
```
git submodule update --init --recursive
```
Normalement le script de compilation s'occupe de compiler les deux modules une fois que le code est téléchargé.

## Compilation

Pour créer le makefile :
```
template/$ mkdir build
template/$ cd build
template/build$ cmake ..
```

Pour tout compiler :
```
template/build$ make
```

Pour compiler le tp seulement :
```
template/build$ make tp
```

## Exécution

```

```


