# RAR3D
TP Reconstitution, Analyse et Restitution 3D


## Enoncé :

1. Utiliser les maillages disponibles sur plubel pour :

    * calculer le nombre de sommets et de faces,
    * calculer un histogramme de la valence des sommets,
    * calculer un histogramme des angles dièdres,
    * calculer les aires des faces,
    * calculer les cartes de courbures et en prendre quelques images "de référence"...

Et quelques traitements possibles (1 à choisir et à documenter):

    * lisser le maillage
    * décimer le maillage
    * remailler

A chaque étape, détailler les algorithmes utilisés (ceux de meshlab sont référencés par les articles scientifiques), analyser les impacts de manière générale, analyser les impacts sur vos maillages, conclure.

2. télécharger et compiler les outils de S. Furhmann : https://github.com/simonfuhrmann/mve

les utiliser sur le matériel de photogrammétrie (les 15 photos de statues). Le script "reconstruct.bash" peut faciliter les différentes étapes (les exécutables de la compilation des outils doivent être dans un rép. "bin/" , les images dans un rép. "images/").

CF diapo 21 du CM :

Utilisation de MVE
https://github.com/simonfuhrmann/mve/wiki/MVE-Users-Guide

    makescene -i <image-dir> <scene-dir>
    sfmrecon <scene-dir>
    dmrecon -s2 <scene-dir>
    scene2pset -F2 <scene-dir> <scene-dir>/pset-L2.ply
    fssrecon <scene-dir>/pset-L2.ply <scene-dir>/surface-L2.ply
    meshclean -t10 <scene-dir>/surface-L2.ply <scene-dir>/surface-L2-clean.ply


## Outils disponibles :
    meshlab
    pymeshlab
    votre programmation C++ (OpenMesh, CGAL...)