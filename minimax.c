#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM_TABLE 10
#define NR_LINES  3
#define NR_CHILDREN 9
#define MAX_BUFFER 15

#define ERR -1

typedef struct node {
	char mat[DIM_TABLE];
	struct node *child[NR_CHILDREN];
	int level;
	char label;
} node;

int game_win(char mat[], char player) {
/* Functia determina daca s-a terminat jocul
si daca jucatorul player a castigat.
Parametrii de intrare:
	mat - 		tabela cu semnele X si 0
	player - jucatorul X sau 0
Returneaza:
	 0 - 	jocul nu s-a incheiat
	 1 - 	jocul a fost castigat
	-1 -	jocul s-a incheiat cu remiza
*/
	int i, remiza;

	if (((mat[0] == mat [3] ) && (mat[3] == mat[6]) && mat[3] != '-') ||
		((mat[1] == mat [4] ) && (mat[4] == mat[7]) && mat[4] != '-') ||
		((mat[2] == mat [5] ) && (mat[5] == mat[8]) && mat[5] != '-') ||

		((mat[0] == mat [1] ) && (mat[1] == mat[2]) && mat[1] != '-') ||
		((mat[3] == mat [4] ) && (mat[4] == mat[5]) && mat[4] != '-') ||
		((mat[6] == mat [7] ) && (mat[7] == mat[8]) && mat[7] != '-') ||

		((mat[0] == mat [4] ) && (mat[4] == mat[8]) && mat[4] != '-') ||
		((mat[2] == mat [4] ) && (mat[4] == mat[6]) && mat[4] != '-') ) {
			/* jocul s-a incheiat cu victorie */
//!!!!!!!!!!!!!!!!!!!!!!!!!
			if (player == 'X' || player == 'O') {
				/* Intereseaza care jucator castiga */
				if (((mat[0] == mat [3] ) && (mat[3] == mat[6]) && mat[3] == player) ||
				((mat[1] == mat [4] ) && (mat[4] == mat[7]) && mat[4] == player) ||
				((mat[2] == mat [5] ) && (mat[5] == mat[8]) && mat[5] == player) ||

				((mat[0] == mat [1] ) && (mat[1] == mat[2]) && mat[1] == player) ||
				((mat[3] == mat [4] ) && (mat[4] == mat[5]) && mat[4] == player) ||
				((mat[6] == mat [7] ) && (mat[7] == mat[8]) && mat[7] == player) ||

				((mat[0] == mat [4] ) && (mat[4] == mat[8]) && mat[4] == player) ||
				((mat[2] == mat [4] ) && (mat[4] == mat[6]) && mat[4] == player)) {
						return 1;
					}
				}
				else
					/* Nu intereseaza care jucator castiga */
					return 1;
	}
// !!!!!!!!!!!!!!!!!!!!!!!!!!!
	remiza = 1;

	for (i = 0; i < DIM_TABLE; i++)
		if (mat[i] == '-')
			remiza = 0;

	if (remiza == 1)
	/* jocul s-a incheiat cu remiza */
		return -1;
	else
	/* jocul continua */
		return 0;
}

int create_AND_OR(node *n, char initial_player) {
/* Functia determina arborele SI/SAU si eticheteaza fiecare nod cu T sau F.
Parametrii de intrare:
	n - 	nodul de plecare
	player - jucatorul initial, care trebuie sa castige
Returneaza:
	 0 - 	daca nodul n are eticheta F
	 1 - 	daca nodul n are eticheta T
Functia este recursiva. Conditia de oprire este nod frunza.
*/
	int i, r;

	/* Daca n este frunza */
	if (n->child[0] == NULL) {
		if (game_win(n->mat, initial_player) == 1)
			/* castiga jucatorul initial */
			r = 1;
		else
			/* nu castiga jucatorul initial */
			r = 0;
	}
	else {
		if (n->level % 2 == 1) {
			/* jucatorul adversar; noduri impare; nod SI */
			r = 1;
			i = 0;
			while (n->child[i] != NULL) {
				if (create_AND_OR(n->child[i], initial_player) == 0)
					r = 0;
				i++;
			}
		}
		else {
			/* jucatorul initial; noduri pare; nod SAU */
			r = 0;
			i = 0;
			while (n->child[i] != NULL) {
				if (create_AND_OR(n->child[i], initial_player) != 0)
					r = 1;
				i++;
			}
		}
	}
	if (r == 0)
		n->label = 'F';
	else
		n->label = 'T';
	return r;
}


node *create_node(char mat[], int level, char initial_player) {
/* Functia creeaza un nou nod in arbore
Parametrii de intrare:
	mat -	 	semnele X si 0
	level -		nivelul nodului
	initial_player - 	semnul initial, X sau 0; acesta se transmite nemodificat
	apelurilor recursive
Returneaza:
	pointer la nodul creat
Functia este recursiva. Conditia de oprire: nu mai exista copii - sau game_win
*/
	int i, j;
	node *nou, *child;
	char mat_child[DIM_TABLE];
	char player_child;

	/* Se aloca memorie pentru nodul parinte si se initializeaza datele sale */
	nou = (node*) malloc(sizeof(node));
	strcpy(nou->mat, mat);
	nou->level = level;
	for (i = 0; i < NR_CHILDREN; i++)
		nou->child[i] = NULL;

	/* Daca jocul s-a terminat, este nod terminal, nu mai are copii; se returneaza
	adresa nodului; nu intereseaza care jucator castiga */
	if (game_win(mat, ' ') == 1) {
		return nou;
	}
	else {
		/* se creeaza copiii, pe nivelul urmator */
		level = level + 1;
		/* daca nivelul urmator este impar, este acelasi jucator ca cel initial,
		  altfel este celalalt jucator */
		if (level % 2 == 1)
			player_child = initial_player;
		else {
			if (initial_player == 'X')
				player_child = 'O';
			else
				player_child = 'X';
		}

		/* se parcurge tabela; acolo unde se gaseste - se pune semnul X sau 0 in
		functie de jucator */
		for (i = 0; i < DIM_TABLE-1; i++) {
			if (mat[i] == '-') {
				strcpy(mat_child, mat);
				mat_child[i] = player_child;
				/* se creeaza nodul copil, cu aceeasi tabela si un semn schimbat */
				child = create_node(mat_child, level, initial_player);
				/* se memoreaza nodul copil in lista nodului parinte, la sfarsit */
				j = 0;
				while (nou->child[j] != NULL)
					j++;
				nou->child[j] = child;
			}
		}
	}
	return nou;
}

void free_node_memory(node *root) {
/* Functia eliberareaza memoria alocata nodului si copiilor sai.
Parametrii de intrare:
	root - pointer la nodul radacina
Returneaza:
	void
Functia este recursiva. Conditia de oprire: nodul nu mai are copii.
*/

	int i;
	/* se parcurg copiii nodului si se elibereaza memoria alocata lor */
	i = 0;
	while (root->child[i] != NULL) {
		free_node_memory(root->child[i]);
		i++;
	}
	/* se elibereaza memoria nodului parinte */
	free(root);
}


void display_node(node *n, FILE *fout, int c) {
/* Functia afiseaza informatiile din un nod si copiii sai, in functie de cerinta.
Parametrii de intrare:
	n - 	nodul de afisat
	fout -  afisarea
	c - 	cerinta 1 sau 2
Returneaza:
	void
Functia este recursiva. Conditia de oprire: nodul nu are copii.
*/
	char indent[DIM_TABLE];
	int i;

	if (n == NULL) /* Conditia de oprire: nodul nu are copii */
		return;

	/* Se construieste indentarea pe baza nivelului nodului */
	strcpy(indent,  "");
	switch (n->level) {
		case 8:
			strcat(indent, "\t");
		case 7:
			strcat(indent, "\t");
		case 6:
			strcat(indent, "\t");
		case 5:
			strcat(indent, "\t");
		case 4:
			strcat(indent, "\t");
		case 3:
			strcat(indent, "\t");
		case 2:
			strcat(indent, "\t");
		case 1:
			strcat(indent, "\t");
			break;
		default:
			strcpy(indent, "");
	}

	if (c == 1) {
		/* Cerinta 1 */
		/* Se tiparesc X si O sub forma de matrice */
		fprintf(fout, "%s", indent);
		for (i = 0; i < NR_LINES - 1; i++)
		 	fprintf(fout, "%c ", n->mat[i]);
		fprintf(fout, "%c\n", n->mat[NR_LINES-1]);

		fprintf(fout, "%s", indent);
		for (i = NR_LINES; i < 2*NR_LINES - 1; i++)
		 	fprintf(fout, "%c ", n->mat[i]);
		fprintf(fout, "%c\n", n->mat[2*NR_LINES-1]);

		fprintf(fout, "%s", indent);
		for (i = 2*NR_LINES; i < 3*NR_LINES - 1; i++)
		 	fprintf(fout, "%c ", n->mat[i]);
		fprintf(fout, "%c\n", n->mat[3*NR_LINES-1]);

		fprintf(fout, "\n");
	}
	else if (c == 2) {
		/* Cerinta 2 */
		fprintf(fout, "%s%c\n", indent, n->label);
	}
	else
		return;

	/* Se tiparesc copiii nodului */
	i = 0;
	while (n->child[i] != NULL) {
		display_node(n->child[i], fout, c);
		i++;
	}
}

int main (int argc, char* argv[]) {

	int i, in = 0, cerinta;
	FILE *fin = NULL, *fout = NULL;

	char initial_player[3];
	char buffer[MAX_BUFFER];
	char mat[DIM_TABLE];
	node *root;


	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][2]) {
				case '1':
					cerinta = 1;
					break;
				case '2':
					cerinta = 2;
					break;
				default:
					return ERR;
			}
		}
		/* primul argument care nu incepe cu '-' este inputul */
		if (argv[i][0] != '-' && in == 0) {
			fin = fopen(argv[i], "r");
			in = 1;
			i++;
		}
		/* al doilea argument care nu incepe cu '-' va fi mereu fisierul de
		iesire */
		if (argv[i][0] != '-' && in == 1)
			fout = fopen(argv[i], "w");
	}

	if (fin == NULL) {
		printf("Eroare la deschiderea fisierului de intrare\n");
		return ERR;
	}
	if (fout == NULL) {
		printf("Eroare la deschiderea fisierului de iesire\n");
		return ERR;
	}

	/* Se citeste prima linie din fisier, ce reprezinta jucatorul care e la rand */
	fgets(initial_player, 3, fin);
	initial_player[1] = '\0';

	/* Se citeste starea initiala a jocului */
	for (i = 0; i < NR_LINES; i++) {
		fgets(buffer, MAX_BUFFER, fin);
		sscanf(buffer, "%c %c %c\n", &(mat[i*NR_LINES]), &(mat[i*NR_LINES + 1]), &(mat[i*NR_LINES + 2]));
	}
	mat[DIM_TABLE-1] = '\0';

	/* Se creeaza nodul parinte din aceasta stare si toti copiii lui. In final va
	rezulta arborele */
	root = create_node(mat, 0, initial_player[0]);

	/* Se creeaza arborele Si/SAU */
	if (cerinta == 2)
		create_AND_OR(root, initial_player[0]);

	/* Se afiseaza arborele */
	display_node(root, fout, cerinta);

	/* Se elibereaza memoria alocata arborelui */
	free_node_memory(root);

	fclose(fin);
	fclose(fout);

	return 0;
}
