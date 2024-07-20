//Setup

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define INGPTR 2000
#define DEPSIZE 50
#define NAMELEN 50
#define RECEIPTLEN 2000
#define MAXLINELENGTH 1024

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0700)
#endif

typedef struct dish {
    char name[NAMELEN];
    char receipt[RECEIPTLEN];
    struct ingredient* dependencies[DEPSIZE];
    struct dish* next;
    struct dish* previous;
} dsh;

typedef struct ingredient {
    char name[35];
    struct ingredient* next;
    struct ingredient* previous;
} ing;

//Code

void initializeItem(dsh* dish, ing* ingredient) {
    memset(dish, 0, sizeof(dish));
    memset(ingredient, 0, sizeof(ingredient));
}

int isStringEmpty(const char* str) {
    return str[0] == '\0';
}

void dishout(dsh* head) {
    dsh* help = head;
    int i = 1, j = 0;
    if (help == NULL) {
        printf("Die Liste ist leer!\n");
    }
    for (help = head; help != NULL; help = help->next) {
        printf("%d: %s, Zutaten: ", i, help->name);
        if (help->dependencies[j] == NULL) {
            printf("Keine Angabe");
        }
        for (int j = 0; help->dependencies[j] != NULL && j < DEPSIZE; j++) {
            printf("%s ", help->dependencies[j]->name);
        }
        i++;
        printf("\n");
    }
    return;
}

void ingout(ing* head) {
    ing* help = head;
    int i = 1;
    if (help == NULL) {
        printf("Die Liste ist leer!\n");
    }
    while (help != NULL) {
        printf("%d: %s\n", i, help->name);
        help = help->next;
        i++;
    }
    return;
}

dsh* dishsearch(dsh* head, char name[]) {
    dsh* help = head;
    while (help != NULL && strcmp(name, help->name) > 0) {
        help = help->next;
    }
    if (help != NULL && strcmp(name, help->name) == 0) {
        return help;
    }
    else return NULL;
}

ing* ingsearch(ing* head, char name[]) {
    ing* help = head;
    while (help != NULL && strcmp(name, help->name) > 0) {
        help = help->next;
    }
    if (help != NULL && strcmp(name, help->name) == 0) {
        return help;
    }
    else return NULL;
}

dsh* dishadd(dsh* head, char name[]) {
    dsh* help = head, * temp = NULL, * new;
    if (head == NULL) {
        head = (dsh*)malloc(sizeof(dsh));
        *head = (dsh){ 0 };
        strcpy(head->name, name);
    }
    else if (strcmp(name, head->name) < 0) {
        new = (dsh*)malloc(sizeof(dsh));
        *new = (dsh){ 0 };
        strcpy(new->name, name);
        head->previous = new;
        head = new;
        head->next = help;
    }
    else {
        while (help != NULL && strcmp(name, help->name) > 0) {
            temp = help;
            help = help->next;
        }
        new = (dsh*)malloc(sizeof(dsh));
        *new = (dsh){ 0 };
        strcpy(new->name, name);
        temp->next = new;
        new->previous = temp;
        if (help != NULL) {
            new->next = help;
            help->previous = new;
        }
    }
    return head;
}

ing* ingadd(ing* head, char name[]) {
    ing* help = head, * temp = NULL, * new;
    if (head == NULL) {
        head = (ing*)malloc(sizeof(ing));
        *head = (ing){ 0 };
        strcpy(head->name, name);
    }
    else if (strcmp(name, head->name) < 0) {
        new = (ing*)malloc(sizeof(ing));
        *new = (ing){ 0 };
        strcpy(new->name, name);
        head->previous = new;
        head = new;
        head->next = help;
    }
    else {
        while (help != NULL && strcmp(name, help->name) > 0) {
            temp = help;
            help = help->next;
        }
        new = (ing*)malloc(sizeof(ing));
        *new = (ing){ 0 };
        strcpy(new->name, name);
        temp->next = new;
        new->previous = temp;
        if (help != NULL) {
            new->next = help;
            help->previous = new;
        }
    }
    return head;
}

dsh* dishrem(dsh* head, char name[]) {
    dsh* help = NULL, * temp = NULL;
    if (head == NULL) {
        printf("Die Liste ist leer, es gibt kein zu loeschendes Gericht!\n");
    }
    else {
        help = dishsearch(head, name);
        if (help == head) {
            if (head->next != NULL) {
                head = head->next;
                head->previous = NULL;
                free(help);
                printf("Gericht erfolgreich geloescht.\n");
            }
            else {
                free(help);
                help = NULL;
                head = NULL;
                printf("Gericht erfolgreich geloescht, die Liste ist jetzt leer.\n");
            }
        }
        else if (help != NULL) {
            temp = help->previous;
            temp->next = help->next;
            if (help->next != NULL) {
                temp = help->next;
                temp->previous = help->previous;
            }
            free(help);
            printf("Gericht erfolgreich geloescht.\n");
        }
        else {
            printf("Das gesuchte Gericht konnte nicht gefunden werden!\n");
        }
    }
    return head;
}

int ingremfromeverydish(dsh* dshhead, ing* ingredient){
	dsh* dshhelp = dshhead;
	int i = 0;
	
	while(dshhelp != NULL){
		for (i = 0; i < DEPSIZE && dshhelp->dependencies[i] != ingredient; i++);
		if (i < DEPSIZE && dshhelp->dependencies[i] == ingredient) {
			dshhelp->dependencies[i] = NULL;
			for (int j = i; j < DEPSIZE - 1; j++) {
				dshhelp->dependencies[j] = dshhelp->dependencies[j + 1];
			}
			dshhelp->dependencies[DEPSIZE - 1] = NULL;
		}
		dshhelp = dshhelp->next;
	}
	return EXIT_SUCCESS;
}

ing* ingrem(ing* head, char name[], dsh* dshhead) {
    ing* help = NULL, * temp = NULL;
    if (head == NULL) {
        printf("Die Liste ist leer, es gibt keine zu loeschende Zutat!\n");
    }
    else {
        help = ingsearch(head, name);
        if (help == head) {
            ingremfromeverydish(dshhead, help);
            if (head->next != NULL) {
                head = head->next;
                head->previous = NULL;
                free(help);
                printf("Zutat erfolgreich geloescht.\n");
            }
            else {
                free(help);
                help = NULL;
                head = NULL;
                printf("Zutat erfolgreich geloescht, die Liste ist jetzt leer.\n");
            }
        }
        else if (help != NULL) {
            ingremfromeverydish(dshhead, help);
            temp = help->previous;
            temp->next = help->next;
            if (help->next != NULL) {
                temp = help->next;
                temp->previous = help->previous;
            }
            free(help);
            printf("Zutat erfolgreich geloescht.\n");
        }
        else {
            printf("Die gesuchte Zutat konnte nicht gefunden werden!\n");
        }
    }
    return head;
}

int addingtodish(dsh* dshhead, ing* inghead, char dshname[], char ingname[]) {
    dsh* dshhelp = dishsearch(dshhead, dshname);
    ing* inghelp = ingsearch(inghead, ingname);
    int i = 0;
    if (inghelp == NULL) {
        return EXIT_FAILURE;
    }
    else {
        for (i = 0; i < DEPSIZE && dshhelp->dependencies[i] != NULL; i++);
        if (i < DEPSIZE && dshhelp->dependencies[i] == NULL) {
            dshhelp->dependencies[i] = inghelp;
        }
        else {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int remingfromdish(dsh* dshhead, ing* inghead, char dshname[], char ingname[]) {
    dsh* dshhelp = dishsearch(dshhead, dshname);
    ing* inghelp = ingsearch(inghead, ingname);
    int i = 0;
    if (inghelp == NULL) {
        return EXIT_FAILURE;
    }
    else {
        for (i = 0; i < DEPSIZE && dshhelp->dependencies[i] != inghelp; i++);
        if (i < DEPSIZE && dshhelp->dependencies[i] == inghelp) {
            dshhelp->dependencies[i] = NULL;
            for (int j = i; j < DEPSIZE - 1; j++) {
                dshhelp->dependencies[j] = dshhelp->dependencies[j + 1];
            }
            dshhelp->dependencies[DEPSIZE - 1] = NULL;
        }
        else {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int addrecipetodish(dsh* head, char name[NAMELEN], char recipe[RECEIPTLEN]) {
    dsh* help = dishsearch(head, name);
    if (help != NULL) {
        strcpy(help->receipt, recipe);
        return EXIT_SUCCESS;
    }
    else return EXIT_FAILURE;
}

int changedishrecipe(dsh* head, char name[]) {
    dsh* help = dishsearch(head, name);
    char recipe[RECEIPTLEN];

    printf("Geben Sie das Rezept ein. Beachten Sie dabei, dass alle Leerstriche mit Unterstrichen ausgetauscht werden müssen!\n");
    scanf("%s", recipe);

    if (strcpy(help->receipt, recipe) == NULL) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

dsh* changedishname(dsh* head, char oldname[], char newname[]) {
    char receipt[RECEIPTLEN] = { 0 };
    ing* dependencies[DEPSIZE] = { 0 };
    dsh* help = dishsearch(head, oldname);
    for (int i = 0; help->dependencies[i] != NULL && i < DEPSIZE; i++) {
        dependencies[i] = help->dependencies[i];
    }
    strcpy(receipt, help->receipt);
    head = dishrem(head, oldname);
    head = dishadd(head, newname);
    help = dishsearch(head, newname);
    for (int i = 0; dependencies[i] != NULL && i < DEPSIZE; i++) {
        help->dependencies[i] = dependencies[i];
    }
    strcpy(help->receipt, receipt);
    return head;
}

dsh* dishchange(dsh* dishhead, char name[], ing* inghead) {
    dsh* help = dishhead;
    ing* inghelp = inghead;
    ing* dependencies[DEPSIZE] = { 0 };
    int choice, check, status = 1;
    char newname[NAMELEN], receipt[RECEIPTLEN], ingname[NAMELEN];

    help = dishsearch(dishhead, name);

    if (help == NULL) {
        printf("Es wurde kein Gericht mit diesem Namen gefunden!\n");
        status = 0;
    }
    else {
        while (status) {
            printf("Welche Operation moechten Sie ausfuehren:\n");
            printf("1) Namen aendern\n2) Rezept aendern\n3) Zutat hinzufuegen\n4) Zutat entfernen\n5) Abbrechen\n");
            scanf("%d", &choice);
            switch (choice)
            {
            case 1:
                printf("Geben Sie den neuen Namen ein: ");
                scanf("%s", newname);
                dishhead = changedishname(dishhead, name, newname);
				status = 0;
            case 2:
                if (changedishrecipe(dishhead, name) == EXIT_SUCCESS) {
                    printf("Das Rezept wurde erfolgreich hinzugefügt.\n");
                    status = 0;
                }
                else {
                    printf("Das Rezept konnte nicht zur Zutat geschrieben werden.\n");
                }
                break;

            case 3:
                printf("Geben Sie den Name der Zutat ein: ");
                scanf("%s", ingname);
                check = addingtodish(dishhead, inghead, name, ingname);
                if (check == 0) {
                    printf("%s erfolgreich hunzugefuegt.\n", ingname);
                    status = 0;
                }
                else printf("%s konnte nicht hinzugefuegt werden. Kontrollieren Sie, ob es diese Zutat ueberhaupt gibt.\n", ingname);
                break;

            case 4:
                printf("Geben Sie den Name der Zutat ein: ");
                scanf("%s", ingname);
                inghelp = ingsearch(inghead, ingname);
                check = remingfromdish(dishhead, inghead, name, ingname);
                if (check == 0) {
                    printf("%s erfolgreich entfernt.\n", ingname);
                    status = 0;
                }
                else printf("%s konnte nicht entfernt werden. Kontrollieren Sie, ob das Gericht mit dieser Zutat schon verbunden ist, oder, ob es diese Zutat ueberhaupt gibt.\n", ingname);
                break;

            case 5:
                status = 0;
                break;

            default:
                printf("Unbekannte Eingabe!\n");
                break;
            }
        }
    }
    return dishhead;
}

void** read_file(char path[], dsh* dishhead, ing* inghead) {
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("Fehler beim Oeffnen der Datei");
        return NULL;
    }

    char name[NAMELEN] = "";
    char option[NAMELEN] = "";
    char temp[NAMELEN] = "";
    while ((fscanf(file, "%s %s\n", option, name)) == 2) {
        if (strcmp(option, "I_Name") == 0) {
            inghead = ingadd(inghead, name);
        }
        else if (strcmp(option, "D_Name") == 0) {
            dishhead = dishadd(dishhead, name);
            strcpy(temp, name);
        }
        else if (strcmp(option, "D_Dependency") == 0) {
            addingtodish(dishhead, inghead, temp, name);
        }
    }
	
    static void* arr[2];
    arr[0] = dishhead;
    arr[1] = inghead;
    fclose(file);
    return arr;
}

int read_recipes(char path[], dsh* dishead) {
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("Fehler beim Oeffnen der Datei");
        printf("Es konnten keine Rezepte ausgelesen werden.\n");
        return EXIT_FAILURE;
    }

    int retval = 0;
    dsh* dish;
    char name[NAMELEN] = "";
    char recipe[RECEIPTLEN] = "";
    while ((fscanf(file, "%s %s\n", name, recipe)) == 2) {
        dish = dishsearch(dishead, name);
        if (dish != NULL) {
            strcpy(dish->receipt, recipe);
        }
        else {
            retval = -1;
            printf("Die Rezepte-Datei konnte zwar geoeffnet werden, aber es gab ein Problem beim Auslesen der Datei-Inhalte. Moeglicherweise sind nicht alle ihre Rezepte in die Liste aufgenommen worden.\n");
        }
    }
    return retval;
}

int write_file(char path[], dsh* dshhead, ing* inghead) {
    dsh* dshhelp = dshhead;
    ing* inghelp = inghead;
    FILE* file = fopen(path, "w");
    if (!file) {
        perror("Fehler beim Oeffnen der Datei");
        printf("Es konnten keine Gerichte und Zutaten gespeichert werden.\n");
        return EXIT_FAILURE;
    }
    while (inghelp != NULL) {
        fprintf(file, "I_Name %s\n", inghelp->name);
        inghelp = inghelp->next;
    }
    while (dshhelp != NULL) {
        fprintf(file, "D_Name %s\n", dshhelp->name);
        for (int i = 0; i < DEPSIZE; i++) {
            if (dshhelp->dependencies[i] != NULL) {
                fprintf(file, "D_Dependency %s\n", dshhelp->dependencies[i]->name);
            }
        }
        dshhelp = dshhelp->next;
    }

    return EXIT_SUCCESS;
}

int write_recipes(char path[], dsh* dshhead) {
    dsh* dshhelp = dshhead;

    FILE* file = fopen(path, "w");
    if (!file) {
        perror("Fehler beim Oeffnen der Datei");
        printf("Es konnten keine Rezepte gespeichert werden.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;

    while (dshhelp != NULL) {
        if (!isStringEmpty(dshhelp->receipt)) {
            fprintf(file, "%s %s\n", dshhelp->name, dshhelp->receipt);
        }
        dshhelp = dshhelp->next;
    }
}

int fileOrDirectoryExists(const char* path) {
    struct stat buffer;
    return (stat(path, &buffer) == 0);
}

char** check_for_files(void) {
    char folderName[] = "data";
    static char filePaths[2][100] = { 0 };
	static char* filePathsPtrs[2] = { filePaths[0], filePaths[1] };

    if (!fileOrDirectoryExists(folderName)) {
        if (MKDIR(folderName) == -1) {
            perror("Fehler beim Erstellen des Daten-Ordners");
            printf("Moeglicherweise wird das Programm in einem schreibgeschuetztem Ordner ausgefuehrt. Verschieben Sie das Programm in einem anderen Ordner oder starten Sie es mit Administratorrechten.\n");
            return NULL;
        }
        printf("Der Daten-Ordner '%s' wurde erfolgreich erstellt.\n", folderName);
    }

    snprintf(filePaths[0], sizeof(filePaths[0]), "%s\\dishes_and_ingredients.txt", folderName);
    snprintf(filePaths[1], sizeof(filePaths[1]), "%s\\recipes.txt", folderName);

    if (!fileOrDirectoryExists(filePaths[0])) {
        FILE* file1 = fopen(filePaths[0], "w");
        if (file1 == NULL) {
            perror("Fehler beim Erstellen einer Datei.");
            return NULL;
        }
        fclose(file1);
        printf("Die Datei '%s' wurde erfolgreich erstellt.\n", filePaths[0]);
    }

    if (!fileOrDirectoryExists(filePaths[1])) {
        FILE* file2 = fopen(filePaths[1], "w");
        if (file2 == NULL) {
            perror("Fehler beim Erstellen einer Datei.");
            return NULL;
        }
        fclose(file2);
        printf("Die Datei '%s' wuirde erfolgreich erstellt.\n", filePaths[1]);
    }
    return filePathsPtrs;
}

void debug(dsh* head) {
    dsh* help;
    int j = 0;
    for (help = head; help != NULL; help = help->next) {
        printf("Name: %s (%d), Next: %d, Previous: %d Dependencies: ", help->name, help, help->next, help->previous);
        for (int i = 0; i < DEPSIZE && help->dependencies[i] != NULL; i++) {
            printf("%s ", help->dependencies[i]->name);
        }
        printf("\n");
    }
}

void ingdebug(ing* head) {
    ing* help = head;
    int i = 1;
    while (help != NULL) {
        printf("Name: %s (%d) Next: %d Previous: %d\n", help->name, help, help->next, help->previous);
        help = help->next;
        i++;
    }
    return;
}

void print_recipes(dsh* head){
	dsh* help = head;
    while (help != NULL) {
        printf("Name: %s Recipe: %s\n", help->name, help->receipt);
		help = help->next;
    }
	return;
}

int main() {
    int status = 1;
    int choice;
    int inp, inp2;
    int check;
    char cinp[NAMELEN], cinp2[NAMELEN];
    char path[MAXLINELENGTH];
    char** filePaths;
    dsh* dshhead = NULL;
    dsh* dshhelp = NULL;
    ing* inghead = NULL;
    ing* inghelp = NULL;
    void** arr;
    printf("Willkommen zum Essensmanager!\n");
    if (!_getcwd(path, sizeof(path))) {
        perror("Fehler beim Auslesen des Pfades.");
        printf("Moeglicherweise wird das Programm in einem schreibgeschuetztem Ordner ausgefuehrt. Verschieben Sie das Programm in einem anderen Ordner oder starten Sie es mit Administratorrechten.\n");
    }
    else {
        filePaths = check_for_files();
		if (filePaths == NULL) {
			printf("Ein Fehler ist bei der Datei-Suche aufgetreten.\n");
			printf("Beende Programm.\n");
			return EXIT_FAILURE;
		} else {
            arr = read_file(filePaths[0], dshhead, inghead);
            dshhead = (dsh*)arr[0];
            inghead = (ing*)arr[1];
			read_recipes(filePaths[1], dshhead);
        }
    }

    while (status == 1) {
        dshhelp = NULL;
        inghelp = NULL;
        printf("1)  Gerichte ausgeben\n");
        printf("2)  Gericht hinzufuegen\n");
        printf("3)  Gericht loeschen\n");
        printf("4)  Gericht suchen\n");
        printf("5)  Gericht bearbeiten\n");
        printf("6)  Zutaten ausgeben\n");
        printf("7)  Zutat hinzufuegen\n");
        printf("8)  Zutat loeschen\n");
        printf("9)  Zutat suchen\n");
        printf("10) Programm beenden\n");
        printf("11) DEBUG\n");

        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            dishout(dshhead);
            break;

        case 2:
            printf("Wie soll Ihr neues Gericht heissen: ");
            scanf("%s", cinp);
            dshhead = dishadd(dshhead, cinp);
            break;

        case 3:
            printf("Welches Gericht soll geloescht werden: ");
            scanf("%s", cinp);
            dshhead = dishrem(dshhead, cinp);
            break;

        case 4:
            printf("Wie heisst Ihr gericht: ");
            scanf("%s", cinp);
            dshhelp = dishsearch(dshhead, cinp);
            if (dshhelp != NULL) {
                printf("Gefunden: %s\n", dshhelp->name);
            }
            else {
                printf("Es konnte kein Gericht mit diesem Namen gefunden werden!\n");
            }
            break;

        case 5:
            printf("Welches Gericht moechten Sie bearbeiten: ");
            scanf("%s", cinp);
            dshhead = dishchange(dshhead, cinp, inghead);
            break;

        case 6:
            ingout(inghead);
            break;

        case 7:
            printf("Name der neuen Zutat: ");
            scanf("%s", cinp);
            inghead = ingadd(inghead, cinp);
            break;

        case 8:
            printf("Welche Zutat soll geloescht werden: ");
            scanf("%s", cinp);
            inghead = ingrem(inghead, cinp, dshhead);
            break;

        case 9:
            printf("Wie heisst Ihre Zutat: ");
            scanf("%s", cinp);
            inghelp = ingsearch(inghead, cinp);
            if (inghelp != NULL) {
                printf("Gefunden: %s\n", inghelp->name);
            }
            else {
                printf("Es konnte keine Zutat mit diesem Namen gefunden werden!\n");
            }
            break;

        case 10:
            status = 0;
			printf("Aenderungen werden gespeichert...\n");
			if(write_file(filePaths[0], dshhead, inghead) == 1) {
				printf("Es gab einen Fehler beim Beschreiben einer Datei. Ihre Aenderungen wurden nicht gespeichert.\n");
				printf("Moechten Sie das Programm trotzdem beenden? [Y/N]\n");
				scanf("%s", cinp);
				if(strcmp("Y", cinp) == 0) printf("Beende Programm!\n");
				else if(strcmp("N", cinp) == 0) status = 1;
				else printf("Ungueltige Eingabe: Programm wird nicht beendet.\n");
            }
            else {
                write_recipes(filePaths[1], dshhead);
                printf("Beende Programm!\n");
            }
            break;

        case 11:
			print_recipes(dshhead);
            break;

        default:
            printf("Unbekannte Eingabe!\n");
            break;
        }
    }
    return EXIT_SUCCESS;
}
