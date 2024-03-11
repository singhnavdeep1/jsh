ARCHITECTURE
.
├── ARCHITECTURE.md
├── AUTHORS.md
├── jsh
├── Makefile
├── README.md
├── src
│   ├── internes.c
│   ├── internes.h
│   ├── jobs.c
│   ├── jobs.h
│   ├── main.c
│   ├── parser.c
│   ├── parser.h
│   ├── pipe.c
│   ├── pipe.h
│   ├── prompt.c
│   ├── prompt.h
│   ├── redirections.c
│   ├── redirections.h
│   ├── shell.c
│   ├── shell.h
│   ├── substitutions.c
│   └── substitutions.h
├── SUJET.md
└── test.sh

INTERNES:
Implémentation des commandes internes
jobs -t n’est pas implémenté

JOBS:
Gestion des jobs
Faire attention à la fonction update\_job
Le statut des jobs est mis à jour avant chaque tour de boucle dans le main

MAIN:
Fichier principal du programme, c’est ici qu’on appelle la fonction readline

PARSER:
Permet de parser les lignes de commande en séparant avec les espaces

PIPE:
Détection des pipes, la suite de la logique des pipes est gérée dans JOBS dans la fonction launch\_job

PROMPT:
Permet la génération du prompt et notammen, permet de tronquer le chemin courant pour atteindre les 30 caractères
Il est mis à jour à chaque tour de boucle dans le main

REDIRECTIONS:
Permet la detection des redirections, la suite de la logique des redirections est réalisée dans JOBS dans la fonction launch\_job

SHELL:
Fichier qui contient des variables globales diverses
(Si vous regardez l’historique git de ce module vous risquez de rigoler)
La variable ret\_code permet de retenir les codes de retour des commandes

SUBSTITUTIONS:
Détecte les substitutions, exécute la substitution en générant une nouvelle ligne de commande. Dans cette nouvelle ligne de commande, les substitutions sont remplacés par des noms de fichier temporaires (tubes anonymes)


Grossièrement, nous gérons dans l'ordre : substitutions puis pipes puis redirections(bien entendu ce n'est pas aussi partitionné)


