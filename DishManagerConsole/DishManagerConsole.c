//Setup

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INGPTR 2000
#define DEPSIZE 50
#define NAMELEN 50
#define RECEIPTLEN 2000
#define MAXLINELENGTH 1024

typedef struct dish {
    char name[NAMELEN];
    char receipt[RECEIPTLEN];
    struct ingredient* dependencies[DEPSIZE];
    int depcount[DEPSIZE];
    struct dish* next;
    struct dish* previous;
} dsh;

typedef struct ingredient {
    char name[35];
    struct ingredient* next;
    struct ingredient* previous;
    struct dish* dishptr[INGPTR];
} ing;

//Code

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

int remingindish(dsh* head, char name[]) {
    dsh* help = head;
    int i = 0, j = i;
    while (help != NULL) {
        if (help->dependencies[i] != NULL && strcmp(help->dependencies[i]->name, name) == 0) {
            help->dependencies[i] == NULL;
            for (j = i; j < DEPSIZE - 1 && help->dependencies[j] != NULL; j++) {
                help->dependencies[j] = help->dependencies[j + 1];
            }
            help->dependencies[j + 1] = NULL;
        }
        help = help->next;
    }
    return EXIT_SUCCESS;
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

ing* ingrem(ing* head, char name[], dsh* dshhead) {
    ing* help = NULL, * temp = NULL;
    if (head == NULL) {
        printf("Die Liste ist leer, es gibt keine zu loeschende Zutat!\n");
    }
    else {
        help = ingsearch(head, name);
        if (help == head) {
            remingindish(dshhead, name);
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
            remingindish(dshhead, name);
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

dsh* dishchange(dsh* dishhead, char name[], ing* inghead) {
    dsh* help = dishhead;
    ing* inghelp = inghead;
    ing* dependencies[DEPSIZE] = {0};
    int choice1, choice2, i, j, k, l;
    char newname[NAMELEN], receipt[RECEIPTLEN], ingname[NAMELEN];

    help = dishsearch(dishhead, name);
    if (help == NULL) {
        printf("Es wurde kein Gericht mit diesem Namen gefunden!\n");
    }
    else {
        printf("Welche Operation moechten Sie ausfuehren:\n");
        printf("1) Namen aendern\n2) Rezept aendern\n3) Zutat hinzufuegen\n4) Zutat entfernen\n");
        scanf("%d", &choice1);
        switch (choice1)
        {
        case 1:
            printf("Geben Sie den neuen Namen ein: ");
            scanf("%s", newname);
            for (int i = 0; help->dependencies[i] != NULL && i < DEPSIZE; i++) {
                dependencies[i] = help->dependencies[i];
            }
            strcpy(receipt, help->receipt);
            dishhead = dishrem(dishhead, name);
            dishhead = dishadd(dishhead, newname);
            help = dishsearch(dishhead, newname);
            for (int i = 0; dependencies[i] != NULL && i < DEPSIZE; i++) {
                help->dependencies[i] = dependencies[i];
            }
            strcpy(help->receipt, receipt);
            break;

        case 2:
            printf("Diese Funktion wird noch nicht unterstuetzt, wir arbeiten daran, auch dies zu ermoeglichen.\n");
            break;

        case 3:
            printf("Geben Sie den Name der Zutat ein: ");
            scanf("%s", ingname);
            inghelp = ingsearch(inghead, ingname);
            if (inghelp == NULL) {
                printf("Es wurde keine Zutat mit diesem Namen gefunden!\n");
            }
            else {
                for (i = 0; i < DEPSIZE && help->dependencies[i] != NULL; i++);
                if (i < DEPSIZE && help->dependencies[i] == NULL) {
                    help->dependencies[i] = inghelp;
                    for (j = 0; j < INGPTR && inghelp->dishptr[j] != NULL; j++);
                    if (j < INGPTR && inghelp->dishptr[j] == NULL) {
                        inghelp->dishptr[j] = help;
                        printf("%s wurde erfolgreich zu %s hinzugefuegt.\n", inghelp->name, help->name);
                        printf("%d %d\n", help, inghelp->dishptr[j]);
                    }
                    else {
                        printf("Die maximale Anzahl an Gerichten, die diese Zutat besitzen, ist erreicht worden, loeschen Sie zuerst diese Zutat aus einem Gericht.\n");
                    }
                }
                else {
                    printf("Die maximale Anzahl an Zutaten ist erreicht worden, loeschen Sie zuerst eine andere Zutat, um eine neue hinzuzufuegen.\n");
                }
            }
            break;

        case 4:
            printf("Geben Sie den Name der Zutat ein: ");
            scanf("%s", ingname);
            inghelp = ingsearch(inghead, ingname);
            if (inghelp == NULL) {
                printf("Es wurde keine Zutat mit diesem Namen gefunden!\n");
            }
            else {
                for (i = 0; i < DEPSIZE && help->dependencies[i] != inghelp; i++);
                if (i < DEPSIZE && help->dependencies[i] == inghelp) {
                    help->dependencies[i] = NULL;
                    for (int j = i; j < DEPSIZE - 1; j++) {
                        help->dependencies[j] = help->dependencies[j + 1];
                    }
                    for (k = 0; k < INGPTR && inghelp->dishptr[k] != help; k++);
                    if (k < INGPTR && inghelp->dishptr[k] == help) {
                        inghelp->dishptr[k] = NULL;
                        for (int l = k; l < INGPTR - 1; l++) {
                            inghelp->dishptr[k] = inghelp->dishptr[l + 1];
                        }
                    }
                    help->dependencies[DEPSIZE - 1] = NULL;
                    inghelp->dishptr[INGPTR - 1] = NULL;
                }
                else {
                    printf("Dieses Gericht besitzt keine Zutat mit diesem Namen\n");
                }
            }
            break;

        default:
            printf("Unbekannte Eingabe!\n");
            printf("Beende Operation\n");
            break;
        }
    }
    return dishhead;
}

void debug(dsh* head) {
    dsh* help;
    int j = 0;
    for (help = head; help != NULL; help = help->next) {
        printf("Name: %s (%d), Next: %d, Previous: %d Dependencies: ", help->name, help, help->next, help->previous);
        for (int i = 0; i < DEPSIZE && help->dependencies[i] != NULL; i++) {
            printf("%s ", help->dependencies[i]->name);
            for (j = 0; j < INGPTR && help->dependencies[i]->dishptr[j] != help; j++);
            printf("(DishPointer: %d),", help->dependencies[i]->dishptr[j]);
        }
        printf("\n");
    }
}

void** read_file(char path[], dsh* dishhead, ing* inghead) {
    FILE* file = fopen("C:\\Users\\GabrielM\\Test\\DishManagerCOnsole\\Data.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    int check = 0;
    char name[NAMELEN] = "";
    char option[NAMELEN] = "";
    while ((check = fscanf(file, "%s %s\n", option, name)) == 2) {
        if (strcmp(option, "I_Name") == 0) {
            inghead = ingadd(inghead, name);
        }
        else if (strcmp(option, "D_Name") == 0) {
            dishhead = dishadd(dishhead, name);
        }
        else if (strcmp(option, "D_Dependency") == 0) {
            //Add code
        }
        else if (strcmp(option, "D_Recipe") == 0) {
            //Add code
        }
    }

    static void* arr[2];
    arr[0] = dishhead;
    arr[1] = inghead;
    fclose(file);
    return arr;
}

int main() {
    int status = 1;
    int choice;
    int inp, inp2;
    char cinp[NAMELEN], cinp2[NAMELEN];
    dsh* dshhead = NULL;
    dsh* dshhelp = NULL;
    ing* inghead = NULL;
    ing* inghelp = NULL;
    void** arr;
    printf("Willkommen zum Essensmanager!\n");

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
            printf("Beende Programm!\n");
            break;

        case 11:
            debug(dshhead);
            /*arr = read_file("C:/Users/GabrielM/Test/DishManagerCOnsole/Data.txt", dshhead, inghead);
            dshhead = (dsh*)arr[0];
            inghead = (ing*)arr[1];
            printf("%s %s\n", dshhead->name, inghead->name);*/
            break;

        default:
            printf("Unbekannte Eingabe!\n");
            break;
        }
    }
    return EXIT_SUCCESS;
}