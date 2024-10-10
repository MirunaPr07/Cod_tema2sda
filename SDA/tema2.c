/*PRUNOIU Miruna - 311CB*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node {
    int nivel, nr_copii;
    char caracter, *sir_compact; //am nevoie de sir_compact pt task ul 4
    struct node **children;
} Node, *Tree;

void adauga_sufixe(char *sir, Tree arbore) {
    int lg_sir, gasit, i, k;
    Tree copie_arb;
    char *sufix;
    strcat(sir, "$"); //concatenez din prima $ la sfarsitul sirului
    lg_sir = strlen(sir);
    for (i = lg_sir - 1; i >= 0; i--) {
        sufix = sir + i; //extrag fiecare sufix
        copie_arb = arbore;
        //fiecare sufix extras il parcurg
        int j = 0; //j reprezinta caracterul pe care il caut in arbore din sufix
        //schimbare ca sa nu mai am vector predefinit de 27 de elem
        //voi avea pt fiecare nod numarul de copii specificat in structura
        gasit = 1; //presupun ca am cateva caractere comune celor din sufix la inceputul arborelui
        while (copie_arb->nr_copii != 0 && sufix[j] != 0 && gasit == 1) {
            gasit = 0;
            for (k = 0; k <= copie_arb->nr_copii - 1; k++)
                if (copie_arb->children[k]->caracter == sufix[j]) {
                    copie_arb = copie_arb->children[k]; //inaintez in arbore cat
                    //timp corespund caracterele
                    gasit = 1;
                    break;
                }
            if (gasit == 1)
                j++;
        }
        //imi ajunge copie_arb parintele caruia trebuie sa i l pun drept fiu pe primul
        //caracter care nu mai corespunde
        //am ajuns cu copie_arb in punctul in care trebuie sa adaug in arbore restul de
        //caractere din sufix
        if (sufix[j] != 0 && copie_arb->nr_copii != 0) { //cazul in care trebuie sa adaug
        //un nou copil intr un vector deja existent de copii
            //realoc vectorul de copii incat sa adaug unul singur
            copie_arb->children = realloc(copie_arb->children, (copie_arb->nr_copii + 1) * sizeof(Tree));
            copie_arb->nr_copii++;
            copie_arb->children[copie_arb->nr_copii - 1] = calloc(1, sizeof(Node));
            if (copie_arb->children[copie_arb->nr_copii - 1] == NULL) {
                printf("ERR");
                return;
            }
            copie_arb->children[copie_arb->nr_copii - 1]->caracter = sufix[j]; //adaug caracterul la care am
            //ajuns in vectorul de copii
            k = copie_arb->nr_copii - 1; //cu k parcurg vectorul de copii
            while (k >= 1 && copie_arb->children[k]->caracter < copie_arb->children[k - 1]->caracter) { //codul ascii
            //al lui $ e < decat al literelor
                Tree aux;
                aux = copie_arb->children[k];
                copie_arb->children[k] = copie_arb->children[k - 1];
                copie_arb->children[k - 1] = aux;
                //interschimb elementele din vectorul de copii pana cand caracterele sunt in ordine
                k--;
            }
            //tb sa ma mut pe copilul creat
            copie_arb = copie_arb->children[k];
            j++;
        }
        //cazul in care trebuie sa imi aloc celula(primul copil gasit pt nodul curent)
        while (sufix[j] != 0) { //cat timp nu s a termnat sufixul
            copie_arb->nr_copii = 1;
            copie_arb->children = calloc(1, sizeof(Tree)); //vector de elemente de tip tree
            if (copie_arb->children == NULL) {
                printf("ERR");
                return;
            }
            copie_arb->children[0] = calloc(1, sizeof(Node));
            if (copie_arb->children[0] == NULL) {
                printf("ERR");
                return;
            }
            copie_arb->children[0]->caracter = sufix[j]; //adaug caracterul curent din sufix
            copie_arb = copie_arb->children[0]; //inaintez in arbore
            j++;
        }
    }
}

void parcurgere_latime(Tree arbore, Tree adresele[500], int *nr_parinti) {
    //pacurgerea in latime implica o coada?? -> o implementez cu un vector de
    //adrese(tip Tree), trimis ca param
    //o modificare ca sa stiu unde pun endline uri-> ma folosesc de nivele
    int index_crt, i;
    adresele[0] = arbore; //adaug radacina ca primul element
    arbore->nivel = 0;
    *nr_parinti = 1;
    index_crt = 0;
    while (index_crt <= *nr_parinti - 1) { //cat timp nu s a terminat vectorul de adrese
        for (i = 0; i <= adresele[index_crt]->nr_copii - 1; i++) { //parcurg copiii
                adresele[index_crt]->children[i]->nivel = adresele[index_crt]->nivel + 1;
                //setez nivelul copiilor
                adresele[*nr_parinti] = adresele[index_crt]->children[i];
                (*nr_parinti)++; //am adaugat sa sfarsit copiii
        }
        index_crt++;
    }
}

void afisare_latime(Tree adresele[500], int nr_parinti, FILE *fis_iesire) {
    int index, ultimul_niv;
    //vectorul de adrese e format prin parcurgere in latime
    ultimul_niv = 1;
    for (index = 1; index <= nr_parinti - 1; index++) { //plec cu index de la 1 ca sa sar peste radacina
        if (ultimul_niv != adresele[index]->nivel) {
            fprintf(fis_iesire, "\n");
            ultimul_niv = adresele[index]->nivel; //il actualizez de fiecare data cand ajung la alt nivel
        }
        fprintf(fis_iesire, "%c ", adresele[index]->caracter);
    }
    fprintf(fis_iesire, "\n");
}

int numar_frunze(Tree adresele[500], int nr_parinti) {
    int nr_frunze = 0, index_crt;
    //parcurg vectorul de adrese si numar cate noduri au numarul de copii 0
    index_crt = 1; //sar peste radacina
    while (index_crt <= nr_parinti - 1) {
        if (adresele[index_crt]->nr_copii == 0)
            nr_frunze++;
        index_crt++;
    }
    return nr_frunze;
}

int sufixe_k(Tree adresele[500], int nr_parinti, char *lg) {
    int nr_sufixe, k, index_crt;
    k = lg[0] - '0';
    nr_sufixe = 0;
    //arborele meu contine sufixele, la care am concatenate si caracterul '$'
    //eu trebuie sa calculez cate sufixe au lg k fara caract '$'
    //nivelul unui nod este egal cu lungimea sufixului respectiv (cu tot cu $)
    //parcurg vectorul de noduri si cresc numarul de sufixe cu lg k + 1
    //cand gasesc ca am nivelul k + 1 (am ajuns pe un '$'), iar nodul nu mai are copii,
    //cresc nr de sufixe de lg k
    for (index_crt = 0; index_crt <= nr_parinti - 1; index_crt++)
        if (adresele[index_crt]->nivel == k + 1 && adresele[index_crt]->nr_copii == 0)
            nr_sufixe++;
    return nr_sufixe;
}

int max_desc(Tree adresele[500], int nr_parinti) {
    int maxim = 0, index_crt;
    //parcurg vectorul de noduri si calculez pe baza lui numarul max de descendenti directi ai unui nod
    for (index_crt = 0; index_crt <= nr_parinti - 1; index_crt++)
        if (adresele[index_crt]->nr_copii > maxim)
            maxim = adresele[index_crt]->nr_copii;
    return maxim;
}

int exista_sufix(char *sufix, Tree arbore) {
    int gasit, i, j;
    Tree copie_arb;
    copie_arb = arbore;
    for (i = 0; sufix[i] != 0; i++) { //parcurg sufixul pe care il caut
        gasit = 0; //presupun ca nu gasesc caracterul la care sunt
        for (j = 0; j <= copie_arb->nr_copii - 1 && gasit == 0; j++) { //parcurg copiii
        //de la nivelul curent
            if (sufix[i] == copie_arb->children[j]->caracter) {
                gasit = 1; //se opreste for ul daca am gasit caracterul
                copie_arb = copie_arb->children[j]; //inaintez in arbore
            }
        }
        if (gasit == 0) //daca nu am gasit caracterul curent returnez deja 0
            return 0;
    }
    if (copie_arb->children[0]->caracter == '$') //mai verific si daca am gasit un sufix de sine
        return 1; //statator, sa nu fie doar inceputul unui sufix
    return 0;
}

void creeaza_compact(Tree arbore, Tree arb_compact) {
    Tree noduri_leg[500], leg_comp[500]; //in vectorii noduri_leg si leg_comp retin nodurile
    //care vor face legatura cu noi noduri
    int nr_legaturi, index_crt, i;
    noduri_leg[0] = arbore;
    leg_comp[0] = arb_compact;
    nr_legaturi = 1; //am acelasi numar de elemente in ambii vectori
    for (index_crt = 0; index_crt <= nr_legaturi - 1; index_crt++) {
        //noduri_leg[index_crt] reprezinta nodul caruia ii parcurg fiii
        leg_comp[index_crt]->nr_copii = noduri_leg[index_crt]->nr_copii;
        leg_comp[index_crt]->children = calloc(noduri_leg[index_crt]->nr_copii, sizeof(Tree));
        //mereu unui nod de legatura din vectorul de legaturi din arborele compact ii voi aloca atatia fii
        //cati are si nodul curent din vectorul de legaturi din arborele atomic
        for (i = 0; i <= noduri_leg[index_crt]->nr_copii - 1; i++) { //parcurg copiii nodului de leg
            leg_comp[index_crt]->children[i] = calloc(1, sizeof(Node)); //aloc fiecare fiu
            char *sir;
            Tree copie_arb;
            copie_arb = noduri_leg[index_crt]->children[i];
            sir = calloc(100, sizeof(char));
            sir[0] = noduri_leg[index_crt]->children[i]->caracter;
            //imi creez un sir in care adaug caracterul fiecarui nod care are doar un fiu
            //sirurile ce contin doar '$' sunt considerate siruri separate de restul
            while (copie_arb->nr_copii == 1 && copie_arb->children[0]->caracter != '$') {
                copie_arb = copie_arb->children[0];
                char *caract;
                caract = calloc(2, sizeof(char));
                caract[0] = copie_arb->caracter;
                strcat(sir, caract); //adaug caracterul in sir
                free(caract);
            }
            leg_comp[index_crt]->children[i]->sir_compact = sir;
            //adaug sirul la indexul curent din vectorul ce contine legaturile vectorului compact
            if (sir[0] != '$') {
                //daca sirul format reprezinta o legatura catre un alt nod
                //ii adaug in vectorii de legaturi
                leg_comp[nr_legaturi] = leg_comp[index_crt]->children[i];
                //fiul va avea alti fii, va fi legatura catre alte noduri
                noduri_leg[nr_legaturi] = copie_arb;
                nr_legaturi++;
            }
        }
    }
}

void afisare_latime_compact(Tree adresele[500], int nr_parinti, FILE *fis_iesire) {
    int index, ultimul_niv;
    ultimul_niv = 1;
    //aceeasi gandire ca la functia la care afisez arborele atomic, doar ca aici afisez un sir
    //am creat vectorul de adrese, parcurgand in latime arborele compact, iar acum
    //doar afisez pentru fiecare nod din vector sirul
    //nivelul ma ajuta sa imi dau seama unde pun '\n'
    for (index = 1; index <= nr_parinti - 1; index++) {
        if (ultimul_niv != adresele[index]->nivel) {
            fprintf(fis_iesire, "\n");
            ultimul_niv = adresele[index]->nivel;
        }
        fprintf(fis_iesire, "%s ", adresele[index]->sir_compact);
    }
    fprintf(fis_iesire, "\n");
}

int main(int argc, char *argv[]) {
    int nr_siruri, nr_parinti, nr_sufixe, i;
    char *sir, aux, *sufix;
    Tree arbore, adresele[500];
    FILE *fis_intrare, *fis_iesire;
    if (strcmp(argv[1], "-c2") == 0) {
        fis_intrare = fopen(argv[3], "r");
        fis_iesire = fopen(argv[4], "w");
    }
    else {
        fis_intrare = fopen(argv[2], "r");
        fis_iesire = fopen(argv[3], "w");
    }
    //pana aici mi am aranjat citirile din terminal
    arbore = calloc(1, sizeof(Node));
    if (arbore == NULL) {
        printf("ERR");
        return 0;
    }
    arbore->caracter = '*';
    //mi am alocat radacina, in ea imi pun caracterul distinctiv *
    sir = calloc(30, sizeof(char)); //imi aloc 30 de caractere pentru
    //sirurile ce vor fi citite
    fscanf(fis_intrare, "%d", &nr_siruri);
    if (strcmp(argv[1], "-c3") == 0) {
        fscanf(fis_intrare, "%d", &nr_sufixe);
        sufix = calloc(30, sizeof(char));
    }
    fscanf(fis_intrare, "%c", &aux); //citesc terminatorul de sir ca sa nu ma incurce
    //pentru fiecare task imi creez la fel arborele, deci am o mare parte comuna
    for (i = 0; i <= nr_siruri - 1; i++) {
        fscanf(fis_intrare, "%s", sir);
        fscanf(fis_intrare, "%c", &aux); //terminatorul de sir
        //fac prelucrarea cu sirul citit
        adauga_sufixe(sir, arbore); //imi creeaza arborele 
        //-> pt fiecare sir citit, adaug toate sufixele in arbore
    }
    parcurgere_latime(arbore, adresele, &nr_parinti); //imi creeaza vectorul de noduri
    //(parcurs in latime), nivelul pt fiecare nod
    //daca am task ul 1, dupa crearea vectorului de noduri si a arborelui, doar il afisez in latime
    if (strcmp(argv[1], "-c1") == 0) {
        afisare_latime(adresele, nr_parinti, fis_iesire); //aici se termina primul task
    } //facut eficient, nu cu 26 de copii pt fiecare nod
    //ca sa determin numarul de frunze, numarul de sufixe de lg k si nr maxim de descendenti ai
    //unui nod, folosesc vectorul adresele creat in functia parcurgere_latime
    if (strcmp(argv[1], "-c2") == 0) {
        fprintf(fis_iesire, "%d\n", numar_frunze(adresele, nr_parinti));
        fprintf(fis_iesire, "%d\n", sufixe_k(adresele, nr_parinti, argv[2]));
        fprintf(fis_iesire, "%d\n", max_desc(adresele, nr_parinti));
    }
    if (strcmp(argv[1], "-c3") == 0) {
        for (i = 0; i <= nr_sufixe - 1; i++) {
            fscanf(fis_intrare, "%s", sufix);
            fscanf(fis_intrare, "%c", &aux); //terminatorul de sir
            //fac prelucrarea cu sirul citit
            fprintf(fis_iesire, "%d\n", exista_sufix(sufix, arbore)); //verific daca am sufixul
            //citit in arbore sau nu
        }
        free(sufix);
    }
    //am deja creat arborele atomic
    //acum il creez pe cel compact pe baza aceluia
    if (strcmp(argv[1], "-c4") == 0) {
        Tree arb_compact;
        arb_compact = calloc(1, sizeof(Node));
        arb_compact->sir_compact = NULL;
        if (arb_compact == NULL) {
            printf("ERR");
            return 0;
        }
        //mi am alocat un nou arbore
        creeaza_compact(arbore, arb_compact); //il creez pe cel compact pe baza celui atomic
        Tree adresele_comp[500];
        int parinti_comp;
        parcurgere_latime(arb_compact, adresele_comp, &parinti_comp); //creez un vector ce
        //contine fiecare nod si pt cel atomic
        afisare_latime_compact(adresele_comp, parinti_comp, fis_iesire); //il afisez, parcurs in latime
        for (i = parinti_comp - 1; i >= 0; i--) {
            if (adresele_comp[i]->nr_copii != 0)
                free(adresele_comp[i]->children);
            free(adresele_comp[i]->sir_compact);
            free(adresele_comp[i]);
        }
    }
    free(sir);
    for (i = nr_parinti - 1; i >= 0; i--) {
        if (adresele[i]->nr_copii != 0)
            free(adresele[i]->children);
        free(adresele[i]);
    }
    fclose(fis_intrare);
    fclose(fis_iesire);
    return 0;
}
