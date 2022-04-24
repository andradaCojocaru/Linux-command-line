#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define MAX_INPUT_LINE_SIZE 300
#define NMAX 50

struct Dir;
struct File;

typedef struct Dir{
	char *name;   // numele directorului
	struct Dir* parent;  // pointer catre parintele directorului(null pentru radacina)
	struct File* head_children_files;  // pointer catre primul element de tip File din interiorul directorului
	struct Dir* head_children_dirs; // pointer catre primul element de tip Dir din interiorul directorului
	struct Dir* next; // pointer catre urmatorul element din lista in care se afla directorul
}Dir; // structura de tip director
 
typedef struct File {
	char *name;  // numele fisierului
	struct Dir* parent; // pointer catre directorul pe
	struct File* next; // pointer catre urmatorul element din lista de fisiere
}File; // structura de tip fisier

int find_file(File *file_curr, char *name, int *n)
{
	int ok = 0;
	// nu exista fisiere in directorul parinte
	if (file_curr == NULL) {
		ok = 2;
	}

	// cautam fisierul in ierarhia de fisiere
	while(file_curr != NULL && file_curr->next != NULL && ok == 0) {
		if (strcmp(file_curr->name, name) == 0) {
			ok = 1;
		}
		file_curr = file_curr->next;
		*n = *n + 1;
	}
	// caz special cand fisierul cautat este ultimul
	if (file_curr != NULL && file_curr->next == NULL && ok == 0) {
		if (strcmp(file_curr->name, name) == 0) {
			ok = 1;
		}
		*n = *n + 1;
	}
	// se returneaza 1 daca fisierul a fost gasit, respectiv 0 in caz contrar
	return ok;
}

int find_directory(Dir *directory_curr, char *name, int *n)
{
	int ok = 0;
	// cautam directorul in ierarhia de directoare
	while(directory_curr != NULL && directory_curr->next != NULL && ok == 0) {
		if (strcmp(directory_curr->name, name) == 0) {
			ok = 1;
		}
		directory_curr = directory_curr->next;
		*n = *n + 1;
	}
	// caz special cand directorul cautat este ultimul
	if (directory_curr != NULL && directory_curr->next == NULL && ok == 0) {
		if (strcmp(directory_curr->name, name) == 0) {
			ok = 1;
		}
		*n = *n + 1;
	}
	// se returneaza 1 daca directorul a fost gasit, respectiv 0 in caz contrar
	return ok;
}
void touch (Dir* parent, char* name)
{
	int ok = 0, n = 0;
	// alocam memorie pentru noul fisier
	File *file_new = malloc(sizeof(File));
	File *file_curr;
	DIE(file_new == NULL, "Eroare alocare memorie");
	file_new->name = malloc((strlen(name) + 1) * sizeof(char));
	strcpy(file_new->name, name);
	file_new->parent = parent;
	file_new->next = NULL;
	file_curr = parent->head_children_files;
	// verifica daca fisierul se afla in ierarhia de fisiere
	ok = find_file(file_curr, name, &n); 

	// fisierul exista deja
	if (ok == 1) {
		printf("File already exists\n");
		free(file_new->name);
		free(file_new);
	// acesta nu exista
	} else {
		if (ok == 0) {
			// parcurgem pana la finalul listei
			while (n > 1) {
				file_curr = file_curr->next;
				n--;
			}
			file_curr->next = file_new;
		}
		// actualizam primul element de tip fisier daca este necesar
		if (parent->head_children_files == NULL) {
			parent->head_children_files = file_new;
			
		}
	}
}

void mkdir (Dir* parent, char* name)
{
	int ok = 0, n = 0;
	Dir *directory_new = malloc(sizeof(Dir));
	Dir *directory_curr;
	DIE(directory_new == NULL, "Eroare alocare memorie");
	directory_new->name = malloc((strlen(name) + 1) * sizeof(char));
	strcpy(directory_new->name, name);
	directory_new->parent = parent;
	directory_new->next = NULL;
	directory_curr = parent;
	// verifica daca directorul se afla in ierarhia de directoare
	ok = find_directory(directory_curr, name, &n);

	// directorul exista deja
	if (ok == 1) {
		printf("Directory already exists\n");
		free(directory_new->name);
		free(directory_new);
	// acesta nu exista
	} else {
		if (ok == 0) {
			// parcurgem pana la finalul listei
			while (n > 1) {
				directory_curr = directory_curr->next;
				n--;
			}
			directory_curr->next = directory_new;
		}
		// initializam campurile directorului
		directory_new->head_children_dirs = NULL;
		directory_new->head_children_files = NULL;
		// actualizam primul element de tip director daca este necesar
		if (parent->head_children_dirs == NULL) {
			parent->head_children_dirs = directory_new;
		}
	}
}

void ls (Dir* parent)
{
	File *file = parent->head_children_files;
	Dir *directory = parent->head_children_dirs;

	// parcurgem toate directoarele
	while(directory != NULL) {
		printf("%s\n", directory->name);
		directory = directory->next;
	}

	// parcurgem toate fisierele
	while(file != NULL) {
		printf("%s\n", file->name);
		file = file->next;
	}
}

void rm (Dir* parent, char* name)
{
	int ok = 0, n = 0, cn;
	File *file_curr, *file_prev;
	file_curr = parent->head_children_files;
	file_prev = file_curr;
	// cautam fisierul
	ok = find_file(file_curr, name, &n);
	cn = n;
	// fisierul nu a fost gasit
	if (ok != 1) {
		printf("Could not find the file\n");
	// acesta a fost gasit
	} else {
		// parcurgem lista pana la fisierul cautat
		while (n > 1) {
			file_prev = file_curr;
			file_curr = file_curr->next;
			n--;
		}
		// daca fisierul este primul trebuie sa actualizam head_children_files
		if (cn == 1) {
			if (parent->head_children_files->next != NULL) {
				parent->head_children_files = parent->head_children_files->next;
			} else {
				parent->head_children_files = NULL;
			}
		// fisierul este la mijloc, refacem legaturile intre vecinii acestuia
		} else {
			if (file_curr->next != NULL) {
				file_prev->next = file_curr->next;
			// fisierul este ultimul
			} else {
				file_prev->next = NULL;
			}
		}
		// eliberare memorie
		free(file_curr->name);
		free(file_curr);
		file_curr = NULL;
	}
}

void rmdir (Dir* parent, char* name)
{
	int ok = 0, n = 0, cn, okfile = 0;
	Dir *directory_curr, *directory_prev;
	File *file_curr, *file_prev;
	directory_curr = parent;
	// cautam directorul
	ok = find_directory(directory_curr, name, &n);
	cn = n;

	// directorul nu a fost gasit
	if (ok != 1) {
		printf("Could not find the dir\n");
	// acesta a fost gasit
	} else {
		// parcurgem lista directoarelor pana ajungem la cel cautat
		while (n > 1) {
			directory_prev = directory_curr;
			directory_curr = directory_curr->next;
			n--;
		}
		// daca directorul este primul, actualizam head_children_files
		if (cn == 2) {
			if (parent->head_children_dirs->next != NULL) {
				parent->head_children_dirs = parent->head_children_dirs->next;
			} else {
				parent->head_children_dirs = NULL;
			}
		// daca directorul este la mijloc, refacem legaturile
		} else {
			if (directory_curr->next != NULL) {
				directory_prev->next = directory_curr->next;
			// fisierul este la final
			} else {
				directory_prev->next = NULL;
				parent = directory_curr->parent;
			}
		}

		// parcurgem toate fisierele si le stergem
		file_prev = directory_curr->head_children_files;
		file_curr = file_prev;
		while(file_prev != NULL && okfile == 0) {
			file_prev = file_curr;
			if (file_curr->next != NULL) {
				file_curr = file_curr->next;
			} else {
				okfile = 1;
			}
			rm(directory_curr, file_prev->name);
		}
		// ne asiguram ca parintele pointeaza catre parintele vecinului
		if (directory_curr->parent->parent != NULL) {
            directory_curr->parent = directory_curr->parent->parent;
		}

		if (directory_curr->next != NULL) {
            directory_prev->next = directory_curr->next;
		} else {
		    directory_prev->next = NULL;
		}
		// eliberam memoria
		free(directory_curr->name);
		free(directory_curr);
		directory_curr = NULL;
	}
}
void cd(Dir** target, char *name)
{
	int ok = 0, n = 0, okfinish = 0, cn;
	Dir *directory_curr, *directory_prev;
	directory_curr = (*target);
	// cautam directorul
	ok = find_directory(directory_curr->head_children_dirs, name, &n);
	cn = n;
	// daca primim comanda "cd ..", coboram un nivel in ierarhie
	if(strcmp(name, "..") == 0) {
		*target = directory_curr->parent;
		okfinish = 1;
	// daca comanda este alta
	// daca nu se gaseste directorul
	} else if (ok != 1) {
		printf("No directories found!\n");
	// parcurgem ierarhia pana la directorul cautat
	} else {
        directory_curr = directory_curr->head_children_dirs;

		while (n > 1) {
			directory_prev = directory_curr;
			directory_curr = directory_curr->next;
			n--;
		}
	}
	// schimbam referinta targetului sa pointeze catre directorul cautat
	if (okfinish == 0) {
		*target = directory_curr;
	}
}

char *pwd (Dir* target)
{
	// alocam memorie pentru sirul cu calea
	char *path = malloc(NMAX);
	Dir *directory_curr = target;
	strcpy(path, "/home");
	// parcurgem directoarele pana ajungem in root
	while (strcmp(directory_curr->name, "home") != 0) {
		directory_curr = directory_curr->parent;
	}
	// concatenam numele fisierelor prin care trecem recursiv
	while (strcmp(directory_curr->name, target->name) != 0) {
		strcat(path,"/");
		strcat(path, directory_curr->head_children_dirs->name);
		directory_curr = directory_curr->next;
	}
	return path;

}

void stop (Dir* target)
{
	Dir *directory_prev, *directory_curr;
	File *file_prev, *file_curr;
	int okf = 0, okd = 0;
	// pornim din directorul radacina
	directory_prev = target;
	directory_curr = directory_prev;
	// parcurgem ierarhia de directoare
	while (directory_prev != NULL && okd == 0) {
		okf = 0;
		if (directory_prev->head_children_files != NULL) {
			file_prev = directory_prev->head_children_files;
			file_curr = file_prev;
			// parcurgem ierarhia de fisiere
			while(file_prev != NULL && okf == 0) {
				file_prev = file_curr;
				if (file_curr->next != NULL) {
					file_curr = file_curr->next;
				} else {
					okf = 1;
				}
				// dezalocam memoria pentru fisiere
				free(file_prev->name);
				free(file_prev);
			}
		}

		// trecem la directorul urmator sau incheiem functia
		if (directory_prev->next != NULL) {
			directory_curr = directory_prev->next;
		} else {
			okd = 1;
		}

		// dezalocam memoria pentru directoare
		free(directory_prev->name);
		free(directory_prev);

		directory_prev = directory_curr;
	}
}

void tree (Dir* target, int level)
{
	int ok = 0;
	Dir *copy = target->next;
	// se parcurg toate directoarele parinti
	if (target != NULL) {
		while (copy->head_children_dirs == NULL && ok == 0) {
			if (copy->next != NULL) {
				copy = copy->next;
			} else {
				ok = 1;
			}
		}
		// daca directoarele apartin directorului target, se afiseaza
		if (strcmp(copy->parent->name, target->name) == 0) {
			for (int i = 0; i < 4 *  level; i++) {
				printf(" ");
			}
			if (copy != NULL) {
				printf("%s\n", copy->name);
			}
			if (copy->next != NULL) {
				tree(copy, level + 1);
			}

			// la intoarcerea din recursivitate, se afiseaza directoarele
			if (target->head_children_dirs->next != NULL) {
				Dir *directory_curr = target->head_children_dirs->next;
				while (directory_curr != NULL) {
					if (strcmp(directory_curr->parent->name, target->name) == 0) {
						for (int i = 0; i < 4 * level; i++) {
							printf(" ");
						}
						printf("%s\n", directory_curr->name);
					}
					directory_curr = directory_curr->next;
				}
			}
			// apoi se afiseaza fisierele copii
			File *file_curr = target->head_children_files;
			while(file_curr != NULL) {
				if (strcmp(file_curr->parent->name, target->name) == 0) {
					for (int i = 0; i < 4 * level; i++) {
						printf(" ");
					}
					printf("%s\n", file_curr->name);
					file_curr = file_curr->next;
				}
 			}
		}
	}
}

void mv(Dir* parent, char *oldname, char *newname)
{
	int okd1 = 0, okf1 = 0, n = 0, m = 0, q = 0, okd2 = 0, okf2 = 0;
	Dir *directory_curr = parent, *directory_prev;
	File *file_curr = parent->head_children_files;
	// se cauta directorul curent
	okd1 = find_directory(directory_curr, oldname, &n);
	// se cauta fisierul curent
	okf1 = find_file(file_curr, oldname, &m);
	// se cauta directorul cu care dorim sa schimbam
	okd2 = find_directory(directory_curr, newname, &q);
	// se cauta fisierul cu care dorim sa schimbam
	okf2 = find_file(file_curr, newname, &q);
	// variabilele n, m, q ne ajuta sa gasim pozitia in ierarhie

	// daca numele dat nu este nici fisier, nici director
	if (okd1 == 0 && (okf1 == 0 || okf1 == 2)) {
		printf("File/Director not found\n");
	// daca s-a gasit o instanta cu numele nou
	} else if (okd2 == 1 || okf2 == 1) {
		printf("File/Director already exists\n");
	// daca numele dat este al unui director
	} else if (okd1 == 1) {
		// alocam memorie pentru noul director
		Dir *directory_new = malloc(sizeof(Dir));
		directory_new->name = malloc((strlen(newname) + 1) * sizeof(char));
		strcpy(directory_new->name, newname);
		directory_prev = directory_curr;

		if (parent->head_children_dirs->next != NULL) {
            parent->head_children_dirs = parent->head_children_dirs->next;
		} else {
		    parent->head_children_dirs = directory_new;
		}

		// cautam pozitia vechiului director
		while (n > 1) {
            directory_prev = directory_curr;
			directory_curr = directory_curr->next;
			n--;
		}

		// creem legaturi
		if (directory_prev->next != NULL && directory_prev->next->next != NULL) {
            directory_prev->next = directory_prev->next->next;
            directory_curr->next->parent = directory_prev;
		} else {
		    directory_prev->next = directory_new;
		}
		
		// copiem referintele catre fisierele si directoarele copii
        directory_new->head_children_dirs = directory_curr->head_children_dirs;
		directory_new->head_children_files = directory_curr->head_children_files;
		
		// parcurgem intreaga ierarhie de directoare
		directory_prev = directory_curr;
		while (directory_curr->next != NULL) {
			directory_curr = directory_curr->next;
		}
		directory_curr->next = directory_new;
		directory_new->parent = directory_curr;

		// dezalocam memoria
		free(directory_prev->name);
		free(directory_prev);
		

	// daca numele dat este al unui fisier
	} else if (okf1 == 1) {
		// stergem fisierul vechi
		rm(parent, oldname);
		// adaugam fisierul nou
		touch(parent, newname);
	}
}

void get_line(char *line, char *line_new, char *command, char *t)
{
	// parseaza sirurile
	fgets(line, MAX_INPUT_LINE_SIZE, stdin);
	strcpy(line_new, line);
	t = strtok(line, "\n ");
	strcpy(command, t);
}

void verificare_malloc_main(Dir *first_directory, char *line, char *line_new,
						char *command, char *file, char *directory,
						char *file_new)
{
	// verifica alocarea de memorie pentru variabilele folosite in main
	DIE(first_directory == NULL, "Eroare alocare memorie");
	DIE(line == NULL, "Eroare alocare memorie");
	DIE(line_new == NULL, "Eroare alocare memorie");
	DIE(command == NULL, "Eroare alocare memorie");
	DIE(file == NULL, "Eroare alocare memorie");
	DIE(directory == NULL, "Eroare alocare memorie");
	DIE(file_new == NULL, "Eroare alocare memorie");

}

void free_main(char *line, char *line_new, char *command, char *file,
			char *directory, char *file_new)
{
	// dezaloca memorie pentru variabilele folosite in main
	free(line);
	free(line_new);
	free(command);
	free(file);
	free(directory);
	free(file_new);
}

int main ()
{
	char *line = malloc(MAX_INPUT_LINE_SIZE),
		*line_new = malloc(MAX_INPUT_LINE_SIZE), *command = malloc(NMAX), *t,
		*file = malloc(NMAX), *directory = malloc(NMAX),
		*file_new = malloc(NMAX);
	int count = 0;
	Dir *first_directory = malloc(sizeof(Dir));
	Dir *new_directory;
	verificare_malloc_main(first_directory, line, line_new, command, file,
						directory, file_new);

	// citeste cate o linie
	get_line(line, line_new, command, t);
	// creeaza directorul home
	first_directory->name = malloc((strlen("home") + 1) * sizeof(char));
	strcpy(first_directory->name, "home");
	first_directory->parent = first_directory;
	first_directory->head_children_files = NULL;
	first_directory->head_children_dirs = NULL;
	first_directory->next = NULL;
	new_directory = first_directory;

	// preia fiecare comanda pana cand se da comanda "stop"
	do {
		if (strcmp(command, "touch") == 0) {
			sscanf(line_new, "%s %s", command, file);
			touch(new_directory, file);
		} else if (strcmp(command, "mkdir") == 0) {
			sscanf(line_new, "%s %s", command, directory);
			mkdir(new_directory, directory);
		} else if (strcmp(command, "rm") == 0) {
			sscanf(line_new, "%s %s", command, file);
			rm(new_directory, file);
		} else if (strcmp(command, "rmdir") == 0) {
			sscanf(line_new, "%s %s", command, directory);
			rmdir(new_directory, directory);
		} else if (strcmp(command, "ls") == 0) {
			sscanf(line_new, "%s", command);
			ls(new_directory);
		} else if (strcmp(command, "mv") == 0) {
			sscanf(line_new, "%s %s %s", command, file, file_new);
			mv(new_directory, file, file_new);
		} else if (strcmp(command, "cd") == 0) {
			sscanf(line_new, "%s %s", command, directory);
			cd(&new_directory, directory);
			if (strcmp(directory, "..") != 0) {
                count++;
			} else {
			    count--;
			}
		} else if (strcmp(command, "tree") == 0) {
			sscanf(line_new, "%s", command);
			tree(new_directory, 0);
		} else if (strcmp(command, "pwd") == 0) {
			char *path;
			sscanf(line_new, "%s", command);
			path = pwd(new_directory);
			printf("%s\n", path);
			free(path);
		} else {
		}
		get_line(line, line_new, command, t);

	} while (strcmp(command, "stop") != 0);
	stop(first_directory);

	// se dezaloca memoria
	free_main(line, line_new, command, file, directory, file_new);
	return 0;
}
