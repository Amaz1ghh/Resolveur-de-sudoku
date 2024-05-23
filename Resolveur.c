// Mallet Piel, Ait Ouazzou Yan
// ELIMINATION_Version1_final.c

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define N 3
#define TAILLE (N * N)

typedef struct {
	int valeur;
	int candidats[TAILLE];
	int nbCandidats;
} tCase1;

typedef tCase1 tGrille[TAILLE][TAILLE];

void chargerGrille(tGrille g){
    char nomFichier[30];
    FILE *f;
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    if (f == NULL) {
        printf("\nERREUR sur le fichier %s\n", nomFichier);
    } else {
        for (int i = 0; i < TAILLE; i++) {
            for (int j = 0; j < TAILLE; j++) {
                fread(&g[i][j].valeur, sizeof(int), 1, f);
            }
        }
        fclose(f);
    }
}

void afficherGrille(tGrille grille) {
    // Afficher les numéros de colonnes
    printf("    ");
    for (int col = 1; col <= TAILLE; col++) {
        printf(" %3d ", col);
        if (col % (int)sqrt(TAILLE) == 0 && col < TAILLE) {
            printf(" ");
        }
    }
    printf("\n");

    printf("   +");
    for (int col = 0; col < TAILLE; col++) {
        printf("-----");
        if (col % (int)sqrt(TAILLE) == (int)sqrt(TAILLE) - 1 && col < TAILLE - 1) {
            printf("+");
        }
    }
    printf("+\n");

    // Afficher la grille
    for (int row = 0; row < TAILLE; row++) {
        printf("%2d |", row + 1);
        for (int col = 0; col < TAILLE; col++) {
            if (col % (int)sqrt(TAILLE) == 0 && col > 0) {
                printf("|");
            }
            if (grille[row][col].valeur == 0) {
                printf("   . ");
            } else {
                printf(" %3d ", grille[row][col].valeur);
            }
        }
        printf("|\n");

        if (row % (int)sqrt(TAILLE) == sqrt(TAILLE) - 1 && row < TAILLE - 1) {
            printf("   +");
            for (int col = 0; col < TAILLE; col++) {
                printf("-----");
                if (col % (int)sqrt(TAILLE) == (int)sqrt(TAILLE) - 1 && col < TAILLE - 1) {
                    printf("+");
                }
            }
            printf("+\n");
        }
    }
    printf("   +");
    for (int col = 0; col < TAILLE; col++) {
        printf("-----");
        if (col % (int)sqrt(TAILLE) == (int)sqrt(TAILLE) - 1 && col < TAILLE - 1) {
            printf("+");
        }
    }
    printf("+\n");
    printf("\n");
}

bool possible(tGrille grille, int ligne, int colonne, int valeur) {
    bool possible = true;
    int i, l;
    // vérification de la ligne et de la colonne
    i=0;
    while (i < TAILLE && possible) {
        if (grille[ligne][i].valeur == valeur || grille[i][colonne].valeur == valeur) {
            possible = false;
        }
        i++;
    }
    // vérification du carré
    int ligneCarre = ligne / N;
    int colonneCarre = colonne / N;
    i=0;
    while (i < N && possible) {
        l = 0;
        while (l < N && possible) {
            if (grille[ligneCarre * N + i][colonneCarre * N + l].valeur == valeur) {
                possible = false;
            }
            l++;
        }
        i++;
    }
    return possible;
}

int nbCandidats(tCase1 laCase){
	return laCase.nbCandidats;
}

bool estCandidat(tCase1 laCase, int val){
	bool trouve;
	int i;
	trouve = false;
	i = 0;
	while ((trouve == false) && (i < nbCandidats(laCase))){
		if (laCase.candidats[i] == val){
			trouve = true;
		}
		i++;
	}
	return trouve;
}

void ajouterCandidat(tCase1 *laCase, int val){
	if ((laCase->nbCandidats) < TAILLE && (estCandidat(*laCase, val) == false)){
		laCase->candidats[nbCandidats(*laCase)] = val;
		laCase->nbCandidats = nbCandidats(*laCase) + 1;
	}
}

void retirerCandidat(tCase1 *laCase, int val){
	int indice;
	bool trouve;
	indice = 0;
	trouve = false;
	if (estCandidat(*laCase, val)){
		while (indice < nbCandidats(*laCase) && (trouve == false)){
			if (laCase->candidats[indice] == val){
				trouve = true;
			}
			indice++;
		}
		laCase->nbCandidats = nbCandidats(*laCase) - 1;
		laCase->candidats[indice-1] = laCase->candidats[nbCandidats(*laCase)];
	}
}

int initialiserCandidats(tGrille grille){
	bool trouve;
	int indice;
	int debutLigne;
	int debutColonne;
    int ligne;
    int colonne;
	int nbCandidatsDebut;

	nbCandidatsDebut = 0;

	for (int i = 0; i < TAILLE; i++){
		for (int j = 0; j < TAILLE; j++){
			if (grille[i][j].valeur == 0){
				grille[i][j].nbCandidats = 0;
				for (int val = 1; val < TAILLE+1; val++){
					trouve = false;
					indice = 0;
					//cherche si le candidat val est present dans la ligne/colonne
					while ((trouve == false) && (indice < TAILLE)){
						if ((grille[i][indice].valeur == val) || (grille[indice][j].valeur == val)){
							trouve = true;
						}
						indice++;
					}
					//cherche si le candidat val est present dans le bloc
					debutLigne = i - i % N;   //indice de la ligne de la première case du bloc
    				debutColonne = j - j % N; //indice de la colonne de la première case du bloc
					ligne = 0;
					while ((trouve == false) && (ligne < N)){
						colonne = 0;
						while ((colonne < N) && (trouve == false)){
							if (grille[debutLigne + ligne][debutColonne + colonne].valeur == val){
								trouve = true;
							}
							colonne++;
						}
						ligne++;
					}
					if (trouve == false){
						ajouterCandidat(&grille[i][j], val);
						nbCandidatsDebut++;
					}
				}
			}
		}
	}
	return nbCandidatsDebut;
}

bool backtracking(tGrille grille, int numeroCase){
    int ligne;
    int colonne;
    bool resultat;
    resultat = false;
    if (numeroCase == TAILLE * TAILLE){
        //on a traité toutes les cases, la grille est résolue
        resultat = true;
    }else{
        // On récupère les "coordonnées" de la case
        ligne = numeroCase / TAILLE;
        colonne = numeroCase % TAILLE;
        if (grille[ligne][colonne].valeur != 0){
            // Si la case n’est pas vide, on passe à la suivante
            // (appel récursif)
            resultat = backtracking(grille, numeroCase+1);
        }else{
            for (int valeur = 1; valeur < TAILLE+1; valeur++){
                if (possible(grille, ligne, colonne, valeur)){
                    // Si la valeur est autorisée on l'inscrit dans la case...
                    grille[ligne][colonne].valeur = valeur;
                    // ... et on passe à la suivante : appel récursif pour voir si ce choix est bon par la suite
                    if (backtracking(grille,numeroCase+1) == true){
                        resultat = true;
                    }else{
                        grille[ligne][colonne].valeur = 0;
                    }
                }
            }
        }
    }
    return resultat;
}

void retireCandPartout(tGrille g, int lig, int col, int val){
	int debutLigne;
	int debutColonne;
	for (int k = 0; k < TAILLE; k++){
		if (g[k][col].valeur == 0){
			retirerCandidat(&g[k][col], g[lig][col].valeur);
		}
		if (g[lig][k].valeur == 0){
			retirerCandidat(&g[lig][k], g[lig][col].valeur);
		}
	}
	//retire la valeur de la liste des candidats pour le bloc
	debutLigne = lig - lig % N;   //indice de la ligne de la première case du bloc
	debutColonne = col - col % N; //indice de la colonne de la première case du bloc
	for (int ligne = 0; ligne < N; ligne++){
		for (int colonne = 0; colonne < N; colonne++){
			if (g[debutLigne + ligne][debutColonne + colonne].valeur == 0){
				retirerCandidat(&g[debutLigne + ligne][debutColonne + colonne], g[lig][col].valeur);
			}
		}
	}
}

//première méthode d'élimination : singleton nu
bool singleton_nu(tGrille g){
    bool progression;
    progression = false;
    for (int i = 0; i < TAILLE; i++){
        for (int j = 0; j < TAILLE; j++){
            if ((g[i][j].valeur == 0) && (nbCandidats(g[i][j]) == 1)){
                g[i][j].valeur = g[i][j].candidats[0];
                retirerCandidat(&g[i][j], g[i][j].valeur);
                //retire la valeur de la liste des candidats pour la ligne et la colonne
                retireCandPartout(g, i, j, g[i][j].valeur);
                progression = true;
            }
		}
    }
    return progression;
}

//deuxième méthode d'élimination : singleton caché
bool singleton_cache(tGrille g){
    bool cand;
    bool trouve;
    bool progression;
    int compte;
    int k;
    int indiceLig;
    int indiceCol;
    int o;
    int lig;
    int col;
    progression = false;
    for (int i = 0; i < TAILLE; i++){
        compte = 1;
        while (compte < TAILLE+1){
            k = 0;
            cand = true;
            indiceLig = -1;
            while ((k < TAILLE) && (cand)){
                if (g[i][k].valeur == compte){
                        cand = false;
                    }
                if (cand && g[i][k].valeur == 0){
                    o = 0;
                    trouve = false;
                    while ((o < nbCandidats(g[i][k]) && (trouve == false))){
                        if ((g[i][k].candidats[o] == compte) && (indiceLig >= 0)){
                            cand = false;
                            trouve = true;
                        }
                        if ((g[i][k].candidats[o] == compte) && (indiceLig < 0)){
                            indiceLig = k;
                            trouve = true;
                        }
                        o++;
                    }
                }
                k++;
            }
            if (cand){
                g[i][indiceLig].valeur = compte;
                g[i][indiceLig].nbCandidats = 0;
                retireCandPartout(g, i, indiceLig, g[i][indiceLig].valeur);
                progression = true;
            }
            compte++;
        }
    }
    for (int i = 0; i < TAILLE; i++){
        compte = 1;
        while (compte < TAILLE+1){
            k = 0;
            cand = true;
            indiceCol = -1;
            while ((k < TAILLE) && (cand)){
                if (g[k][i].valeur == compte){
                        cand = false;
                    }
                if (cand && g[k][i].valeur == 0){
                    o = 0;
                    trouve = false;
                    while ((o < nbCandidats(g[k][i]) && (trouve == false))){
                        if ((g[k][i].candidats[o] == compte) && (indiceCol >= 0)){
                            cand = false;
                            trouve = true;
                        }
                        if ((g[k][i].candidats[o] == compte) && (indiceCol < 0)){
                            indiceCol = k;
                            trouve = true;
                        }
                        o++;
                    }
                }
                k++;
            }
            if (cand){
                g[indiceCol][i].valeur = compte;
                g[indiceCol][i].nbCandidats = 0;
                retireCandPartout(g, i, indiceCol, g[indiceCol][i].valeur);
                progression = true;
            }
            compte++;
        }
    }
    for (int i = 0; i < TAILLE; i+=N){
        for (int k = 0; k < TAILLE; k+=N){
            compte = 1;
            while (compte < TAILLE+1){
                cand = true;
                indiceLig = -1;
                indiceCol = -1;
                lig = 0;
                while ((lig < N) && (cand)){
                    col = 0;
                    while ((col < N) && (cand)){
                        if (g[i + lig][k + col].valeur == compte){
                            cand = false;
                        }
                        if ((cand) && (g[i + lig][k + col].valeur == 0)){
                            o = 0;
                            trouve = false;
                            while ((o < nbCandidats(g[i + lig][k + col])) && (trouve == false)){
                                if ((g[i + lig][k + col].candidats[o] == compte) && (indiceLig >= 0)){
                                    cand = false;
                                    trouve = true;
                                }
                                if ((g[i + lig][k + col].candidats[o] == compte) && (indiceLig < 0)){
                                    indiceLig = i + lig;
                                    indiceCol = k + col;
                                    trouve = true;
                                }
                                o++;
                            }
                        }
                        col++;
                    }
                    lig++;
                }
                if (cand){
                    g[indiceLig][indiceCol].valeur = compte;
                    g[indiceLig][indiceCol].nbCandidats = 0;
                    retireCandPartout(g, indiceLig, indiceCol, g[indiceLig][indiceCol].valeur);
                    progression = true;
                }
                compte++;
            }
        }
    }
    return progression;
}

bool paire_nues(tGrille g){
    //technique de la paires nues
    int val1, val2, lig2, col2, ind1, ind2;
    bool trouve;
    bool progression = false;
    for (int i = 0; i < TAILLE; i+=N){
        for (int k = 0; k < TAILLE; k+=N){
            for (int lig = 0; lig < N; lig++){
                for (int col = 0; col < N; col++){
                    //si on trouve une paire...
                    if ((g[i + lig][k + col].valeur == 0) && (nbCandidats(g[i + lig][k + col]) == 2)){
                        val1 = g[i + lig][k + col].candidats[0];
                        val2 = g[i + lig][k + col].candidats[1];
                        //...on parcourt le bloc pour voir s'il existe une 2e paire de même valeurs
                        lig2 = 0;
                        trouve = false;
                        while ((lig2 < N) && (!trouve)){
                            col2 = 0;
                            while ((col2 < N) && (!trouve)){
                                if (((lig2 != lig) || (col2 != col)) && (g[i + lig2][k + col2].valeur == 0) && (nbCandidats(g[i + lig2][k + col2]) == 2)){
                                    if (((g[i + lig2][k + col2].candidats[0] == val1) || (g[i + lig2][k + col2].candidats[0] == val2)) && ((g[i + lig2][k + col2].candidats[1] == val1) || (g[i + lig2][k + col2].candidats[1] == val2))){
                                        ind1 = i + lig2;
                                        ind2 = k + col2;
                                        trouve = true;
                                    }
                                }
                                col2++;
                            }
                            lig2++;
                        }
                        //si oui, on retire les valeurs du reste du bloc
                        if (trouve){
                            for (int lig3 = 0; lig3 < N; lig3++){
                                for (int col3 = 0; col3 < N; col3++){
                                    if ((g[i + lig3][k + col3].valeur == 0) && (((i+lig3 != ind1) || (k+col3 != ind2)) && ((i+lig3 != i+lig) || (k+col3 != k+col))) && ((estCandidat(g[i + lig3][k + col3], val1)) || (estCandidat(g[i + lig3][k + col3], val2)))){
                                        retirerCandidat(&g[i + lig3][k + col3], val1);
                                        retirerCandidat(&g[i + lig3][k + col3], val2);
                                        progression = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return progression;
}

bool paire_cachees(tGrille g){
    //technique paire cachée
    int o, compte, lig, col, indLig1, indLig2, lig2, col2, indCol1, indCol2, l, colonne, ligne, parcours, parcours2, nbCase0, val;
    bool trouve, valide, progression;
    progression = false;
    //ligne
    for (int i = 0; i < TAILLE; i++){
        parcours = 0;
        nbCase0 = 0;
        while ((parcours < TAILLE) && (nbCase0 < 3)){
            if (g[i][parcours].valeur == 0){
                nbCase0++;
            }
            parcours++;
        }
        if (nbCase0 > 2){
            for (int k = 1; k < TAILLE+1; k++){
                o = 0;
                compte = 0;
                while ((o < TAILLE) && (compte < 3)){
                    if (g[i][o].valeur == 0){
                        if ((estCandidat(g[i][o], k)) && (compte == 1)){
                            compte++;
                            indCol2 = o;
                        } else if ((estCandidat(g[i][o], k)) && (compte == 0)){
                            compte++;
                            indCol1 = o;
                        } else if ((estCandidat(g[i][o], k)) && (compte == 2)){
                            compte++;
                        }
                    } else if (g[i][o].valeur == k){
                        compte = 3;
                    }
                    o++;
                }
                if (compte == 2){
                    l = 0;
                    valide = true;
                    while ((l < nbCandidats(g[i][indCol1])) && (valide)){
                        val = g[i][indCol1].candidats[l];
                        if ((k != val) && (estCandidat(g[i][indCol2], val))){
                            trouve = false;
                            colonne = 0;
                            while ((colonne < TAILLE) && (!trouve)){
                                if ((g[i][colonne].valeur == 0) && (estCandidat(g[i][colonne], val)) && (colonne != indCol1) && (colonne != indCol2)){
                                    trouve = true;
                                } else if(g[i][colonne].valeur == val){
                                    trouve = true;
                                }
                                colonne++;
                            }
                            if ((!trouve) && ((nbCandidats(g[i][indCol1]) != 2) || (nbCandidats(g[i][indCol2]) != 2))){
                                g[i][indCol1].nbCandidats = 2;
                                g[i][indCol1].candidats[0] = k;
                                g[i][indCol1].candidats[1] = val;
                                g[i][indCol2].nbCandidats = 2;
                                g[i][indCol2].candidats[0] = k;
                                g[i][indCol2].candidats[1] = val;
                                valide = false;
                                progression = true;
                            }
                        }
                        l++;
                    }
                }
            }
        }
    }

    //colonne
    for (int j = 0; j < TAILLE; j++){
        parcours = 0;
        nbCase0 = 0;
        while ((parcours < TAILLE) && (nbCase0 < 3)){
            if (g[parcours][j].valeur == 0){
                nbCase0++;
            }
            parcours++;
        }
        if (nbCase0 > 2){
            for (int k = 1; k < TAILLE+1; k++){
                o = 0;
                compte = 0;
                while ((o < TAILLE) && (compte < 3)){
                    if (g[o][j].valeur == 0){
                        if ((estCandidat(g[o][j], k)) && (compte == 1)){
                            compte++;
                            indLig2 = o;
                        } else if ((estCandidat(g[o][j], k)) && (compte == 0)){
                            compte++;
                            indLig1 = o;
                        } else if ((estCandidat(g[o][j], k)) && (compte == 2)){
                            compte++;
                        }
                    } else if (g[o][j].valeur == k){
                        compte = 3;
                    }
                    o++;
                }
                if (compte == 2){
                    l = 0;
                    valide = true;
                    while ((l < nbCandidats(g[indLig1][j])) && (valide)){
                        val = g[indLig1][j].candidats[l];
                        if ((k != val) && (estCandidat(g[indLig2][j], val))){
                            trouve = false;
                            ligne = 0;
                            while ((ligne < TAILLE) && (!trouve)){
                                if ((g[ligne][j].valeur == 0) && (estCandidat(g[ligne][j], val)) && (ligne != indLig1) && (ligne != indLig2)){
                                    trouve = true;
                                } else if(g[ligne][j].valeur == val){
                                    trouve = true;
                                }
                                ligne++;
                            }
                            if ((!trouve) && ((nbCandidats(g[indLig1][j]) != 2) || (nbCandidats(g[indLig2][j]) != 2))){
                                g[indLig1][j].nbCandidats = 2;
                                g[indLig1][j].candidats[0] = k;
                                g[indLig1][j].candidats[1] = val;
                                g[indLig2][j].nbCandidats = 2;
                                g[indLig2][j].candidats[0] = k;
                                g[indLig2][j].candidats[1] = val;
                                valide = false;
                                progression = true;
                            }
                        }
                        l++;
                    }
                }
            }
        }
    }

    //bloc
    for (int i = 0; i < TAILLE; i+=N){
        for (int j = 0; j < TAILLE; j+=N){
            parcours = 0;
            parcours2 = 0;
            nbCase0 = 0;
            while ((parcours < N) && (nbCase0 < 3)){
                while ((parcours2 < N) && (nbCase0 < 3)){
                    if (g[i+parcours][j+parcours2].valeur == 0){
                        nbCase0++;
                    }
                    parcours2++;
                }
                parcours++;
            }
            if (nbCase0 > 2){
                for (int k = 0; k < TAILLE+1; k++){
                    compte = 0;
                    lig = 0;
                    while ((lig < N) && (compte < 3)){
                        col = 0;
                        while ((col < N) && (compte < 3)){
                            if ((estCandidat(g[i+lig][j+col], k)) && (compte == 1)){
                                compte++;
                                indLig2 = i+lig;
                                indCol2 = j+col;
                            } else if ((estCandidat(g[i+lig][j+col], k)) && (compte == 0)){
                                compte++;
                                indLig1 = i+lig;
                                indCol1 = j+col;
                            } else{
                                compte++;
                            }
                            col++;
                        }
                        lig++;
                    }
                    if (compte == 2){
                        l = 0;
                        valide = true;
                        while ((l < nbCandidats(g[indLig1][indCol1])) && (valide)){
                            val = g[indLig1][indCol1].candidats[l];
                            if ((k != val) && (estCandidat(g[indLig2][indCol2], val))){
                                trouve = false;
                                lig2 = 0;
                                while ((lig2 < N) && (!trouve)){
                                    col2 = 0;
                                    while ((col2 < N) && (!trouve)){
                                        if ((g[i+lig2][j+col2].valeur == 0) && (estCandidat(g[i+lig2][j+lig2], val)) && ((i+lig2 != indLig1) || (j+lig2 != indCol1)) && ((i+lig2 != indLig2) || (j+lig2 != indCol2))){
                                            trouve = true;
                                        } else if(g[i+lig2][j+col2].valeur == val){
                                            trouve = true;
                                        }
                                        col2++;
                                    }
                                    lig2++;
                                }
                                if ((!trouve) && ((nbCandidats(g[indLig1][indCol1]) != 2) || (nbCandidats(g[indLig2][indCol2]) != 2))){
                                    g[indLig1][indCol1].nbCandidats = 2;
                                    g[indLig1][indCol1].candidats[0] = k;
                                    g[indLig1][indCol1].candidats[1] = val;
                                    g[indLig2][indCol2].nbCandidats = 2;
                                    g[indLig2][indCol2].candidats[0] = k;
                                    g[indLig2][indCol2].candidats[1] = val;
                                    valide = false;
                                    progression = true;
                                }
                            }
                            l++;
                        }
                    }
                }
            }
        }
    }
    return progression;
}

int main(){

    tGrille g;

    
    //debut temps CPU
    clock_t begin = clock();

    chargerGrille(g);

    bool progression;

    afficherGrille(g);

    //saisie des données
	initialiserCandidats(g);
    progression = true;

    while(progression){
        progression = false;
        //parcours tableau
        progression = singleton_nu(g);
        if (!progression){
            progression = singleton_cache(g);
            if (!progression){
                progression = paire_nues(g);
                if (!progression){
                    progression = paire_cachees(g);
                    if (!progression){
                        backtracking(g, 0);
                    }
                }
            }
        }
    }

    //fin temps CPU
    clock_t end = clock();

    double tempsCPU = (end - begin) * 1.0 / CLOCKS_PER_SEC;

    afficherGrille(g);
    printf("Temps CPU = %f secondes\n", tempsCPU);

    return EXIT_SUCCESS;
}