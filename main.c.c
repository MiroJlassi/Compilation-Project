#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define id 1
#define pv 2 // ;
#define nb 3
#define dp 4  // :
#define aff 5 // affectation
#define oprel 6
#define ppe 7  // <=
#define dif 8  // <>
#define ppq 9  // <
#define pgq 10 // >
#define pge 11 // >=
#define ega 12 // =
#define opadd 13
#define bl 14 // blanc
#define pt 15 // .
#define v 16  // virgule
#define po 17 // parenthèse ouvrante
#define pf 18 // parenthèse fermante
#define opmul 19
#define eof 35
// les mots clés
#define program 20
#define var 21
#define integer 22
#define chart 23
#define begin 24
#define end 25
#define iff 26
#define then 27
#define elsee 28
#define read 29
#define readln 30
#define write 31
#define writeln 32
#define doo 33
#define whilee 34

typedef struct unilex
{
    int ul;
    int att;
} unilex;

typedef struct identifiant
{
    char nom[100];
    char type[100];
} identifiant;

int c = 0;
FILE *fp; // fichier source

int mot_cle[20] = {20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34};
char tab_mot_cle[20][20] = {"program", "var", "integer", "char", "begin", "end", "if", "then", "else", "read", "readln", "write", "writeln", "do", "while"};

identifiant tab_iden[100];
char car;
unilex symbole;
char ch[20];
int z = 0;
int nb_identifiant; // calcule la longueur de tab_iden

void reculer(int k)
{
    fseek(fp, -k, SEEK_CUR);
}

char carsuivant()
{
    return fgetc(fp);
}

int unilexid()
{ // si le lexème est un mot clé, retourne unité lexicale sinon retourne id
    int k = 0;
    int i = 0;
    while (i < 14 && (!k))
    {
        if (strcmp(tab_mot_cle[i], ch) == 0)
        {
            printf(" mot cle \t%s\n", tab_mot_cle[i]);
            k = 1;
        }
        else
        {
            i++;
        }
    }
    if (k == 1)
    {
        printf("code mot cle %d", mot_cle[i]);
        return mot_cle[i];
    }
    else
    {
        return id;
    }
}

int rangerid(int k, int *c)
{ // retourne un pointeur sur la table tab_iden si le lexème s'y trouve sinon il l'ajoute et retourne un pointeur, sinon on retourne 0
    int s = 0;

    if (k == id)
    {
        while ((strcmp(tab_iden[s].nom, ch) != 0) && (s < (*c)))
            s++;
        if (s < *c)
        {
            return s;
        }
        else
        {
            strcpy(tab_iden[*c].nom, ch);
            strcpy(tab_iden[*c].type, "NULL"); // Type non défini par défaut        //////////////////////////////////////////////////////////
            nb_identifiant++;
            (*c)++;
            return *c - 1;
        }
    }
    else
    {
        return 0;
    }
}

// analyse semantique

// Cherche le type d'un identifiant
char *chercher_type(int index)
{
    if (index < 0 || index >= nb_identifiant)
    {
        printf("Erreur : index d'identifiant invalide !\n");
        return NULL;
    }
    return (tab_iden[index].type); // Retourne le type
}

int compatible(char *type1, char *type2)
{
    // Si les types sont egaux, alors ils sont compatibles
    if (strcmp(type1, type2) == 0)
    {
        return 1;
    }
    // Sinon, les types ne sont pas compatibles
    else
    {
        return 0;
    }
}

unilex analex()
{
    int etat = 0;

    while (1)
    {
        switch (etat)
        {
        case 0:
            car = carsuivant();

            if (car == ' ' || car == '\t' || car == '\n')
            {
                etat = 0;
                break;
            }
            else if (isalpha(car))
            {
                z = 0;
                ch[0] = car;
                etat = 1;
            }
            else if (isdigit(car))
            {
                z = 0;
                ch[0] = car;
                etat = 3;
            }
            else if (car == '<')
            {
                etat = 5;
            }
            else if (car == '>')
            {
                etat = 10;
            }
            else if (car == '=')
            {
                etat = 9;
            }
            else if (car == ';')
            {
                etat = 15;
            }
            else if (car == ',')
            {
                etat = 16;
            }
            else if (car == '(')
            {
                etat = 17;
            }
            else if (car == ':')
            {
                etat = 18;
            }
            else if (car == ')')
            {
                etat = 19;
            }
            else if (car == eof)
            {
                printf("end of file ");
                etat = 13;
            }
            else
            {
                etat = 14;
            }
            break;
        case 1:
            car = carsuivant();

            if (isalpha(car) || isdigit(car))
            {
                z++;
                ch[z] = car;
            }
            else
            {
                etat = 2;
                ch[z + 1] = '\0';
                // printf("c'est la chaine %s \n", ch);
                z = 0;
            }
            break;
        case 2:
            reculer(1);
            symbole.ul = unilexid();
            symbole.att = rangerid(symbole.ul, &c); // on stocke le retour de rangerid dans symbole.att (donc si l'identifiant existe on stocke son pointeur dans tab_iden)
            return symbole;
        case 3:
            car = carsuivant();
            if (isdigit(car))
            {
                z++;
                ch[z] = car;
            }
            else
            {
                etat = 4;
                ch[z + 1] = '\0';
                z = 0;
            }
            break;
        case 4:
            reculer(1);
            symbole.ul = nb;
            symbole.att = atoi(ch);
            return symbole;
        case 5:
            car = carsuivant();
            switch (car)
            {
            case '=':
                etat = 6;
                break;
            case '>':
                etat = 7;
                break;
            default:
                etat = 8;
                break;
            }
            break;
        case 6:
            symbole.ul = oprel;
            symbole.att = ppe;
            return symbole;
        case 7:
            symbole.ul = oprel;
            symbole.att = dif;
            return symbole;
        case 8:
            reculer(1);
            symbole.ul = oprel;
            symbole.att = ppq;
            return symbole;
        case 9:
            symbole.ul = oprel;
            symbole.att = ega;
            return symbole;
        case 10:
            car = carsuivant();
            if (car == '=')
            {
                etat = 11;
            }
            else
            {
                reculer(1);
                symbole.ul = oprel;
                symbole.att = pgq;
                return symbole;
            }
            break;
        case 11:
            symbole.ul = oprel;
            symbole.att = pge;
            return symbole;
        case 12:
            symbole.ul = oprel;
            symbole.att = pgq;
            return symbole;
        case 13:
            symbole.ul = 100;
            symbole.att = 0;
            return symbole;
        case 14:
            printf("Erreur : caractere inattendu ");
            etat = 0;
            break;
        case 15:
            symbole.ul = pv;
            symbole.att = 0;
            return symbole;
        case 16:
            symbole.ul = v;
            symbole.att = 0;
            return symbole;
        case 17:
            symbole.ul = po;
            symbole.att = 0;
            return symbole;
        case 18:
            ch[0] = car;
            car = carsuivant();
            if (car == '=')
            {
                symbole.ul = aff;
                symbole.att = 0;
                return symbole;
            }
            else
            {
                etat = 20;
            }
            break;
        case 19:
            symbole.ul = pf;
            symbole.att = 0;
            return symbole;
        case 20:
            reculer(1);
            symbole.ul = dp;
            symbole.att = 0;
            return symbole;
        }
    }
}

// analyse syntaxique ----------------------------------------------------------------------------------------------

void erreur();
void accepter(int t);
void P();
void Dcl();
void dclPrime();
void List_id();
void List_idPrime();
void Type();
void Inst_composee();
void Inst();
void Liste_inst();
void Liste_instPrime();
void I();
void Exp();
void ExpPrime();
void Exp_simple();
void Exp_simple_Prime();
void Terme();
void TermePrime();
void Facteur();

// Fonction pour afficher une erreur
void erreur()
{
    printf("ERREUR: Symbole inattendu -> %d\n", symbole.ul);
}

// Fonction d'acception :
void accepter(int t)
{
    if (symbole.ul == t)
    {
        symbole = analex();
    }
    else
    {
        erreur();
    }
}

void P()
{
    if (symbole.ul == program)
    {
        accepter(program);
        accepter(id);
        accepter(pv);
        Dcl();
        Inst_composee();
    }
}

void Dcl()
{
    dclPrime();
}

void dclPrime()
{
    if (symbole.ul == var)
    {
        accepter(var);
        List_id();
        accepter(dp);

        // definir le type pour les identifiants
        char *type = NULL;
        Type(&type);
        for (int i = 0; i < nb_identifiant; i++)
        {
            if (strcmp(tab_iden[i].type, "NULL") == 0) // identifiant sans type
            {
                strcpy(tab_iden[i].type, type); // attribuer le type
            }
        }
        accepter(pv);
        dclPrime();
    }
}

void List_id()
{
    if (symbole.ul == id)
    {
        accepter(id);
        List_idPrime();
    }
}

void List_idPrime()
{
    if (symbole.ul == v)
    {
        accepter(v);
        accepter(id);
        List_idPrime();
    }
}

void Type(char **type)
{
    if (symbole.ul == integer)
    {
        accepter(integer);
        *type = "integer";
    }
    else if (symbole.ul == chart)
    {
        accepter(chart);
        *type = "char";
    }
    else
    {
        erreur();
    }
}

void Inst_composee()
{
    if (symbole.ul == begin)
    {
        accepter(begin);
        Liste_inst();
        accepter(end);
    }
}

void Liste_inst()
{
    char *t1 = "vide"; // intialise comme vide cad pas d'erreur de type
    I(&t1);
    Liste_instPrime();
}

void Liste_instPrime()
{
    if (symbole.ul == pv)
    {
        accepter(pv);
        Liste_inst();
    }
}

void I(char *t1)
{
    char *t = NULL;       // initialisation
    char *type_id = NULL; // type de l'identifiant
    int num;              // index dans la table des identifiants tab_iden

    if (symbole.ul == id)
    {
        num = symbole.att;
        accepter(id);
        type_id = chercher_type(num);
        accepter(aff);
        Exp(&t);

        if (compatible(type_id, t))
        {
            t1 = "vide";
        }
        else
        {
            t1 = "erreur_de_type";
            printf("Erreur : Incompatibilité des types");
        }
    }
    else if (symbole.ul == iff)
    {
        accepter(iff);
        Exp(&t); // pour les expressions booleennes
        if (strcmp(t, "boolean") != 0)
        {
            t1 = "erreur_de_type";
            printf("Erreur : Condition non booléenne\n");
        }
        accepter(then);
        I(t1);
        accepter(elsee);
        I(t1);
    }
    else if (symbole.ul == whilee)
    {
        accepter(whilee);
        Exp(&t);
        if (strcmp(t, "boolean") != 0)
        {
            t1 = "erreur_de_type";
            printf("Erreur : Condition non booléenne \n");
        }
        accepter(doo);
        I(t1);
    }
    else if (symbole.ul == read || symbole.ul == readln)
    {
        accepter(symbole.ul);
        accepter(po);
        num = symbole.att;
        type_id = chercher_type(num);
        accepter(id);
        accepter(pf);

        if (strcmp(type_id, "integer") != 0 && strcmp(type_id, "char") != 0)
        {
            t1 = "erreur_de_type";
            printf("Erreur : Type incompatible pour read/readln\n");
        }
    }
    else if (symbole.ul == write || symbole.ul == writeln)
    {
        accepter(symbole.ul);
        accepter(po);
        num = symbole.att;
        type_id = chercher_type(num);
        accepter(id);
        accepter(pf);

        if (strcmp(type_id, "integer") != 0 && strcmp(type_id, "char") != 0)
        {
            t1 = "erreur_de_type";
            printf("Erreur : Type incompatible pour write/writeln\n");
        }
    }
    else
    {
        erreur();
    }
}

void Exp(char *t1)
{
    char t2[20] = "vide";
    Exp_simple(t1); // Analyse la premiere partie de l'expression (partie gauche)
    ExpPrime(t1);   // Analyse les relations (operateurs relationnels) et les met a jour
}

void ExpPrime(char *t1)
{
    if (symbole.ul == oprel)
    {
        char t2[20] = "vide";
        accepter(oprel);
        Exp(t2); // Analyse la partie droite de l'expression

        // Verification de la compatibilite des types
        if (!compatible(t1, t2))
        {
            t1 = "erreur_de_type"; // Propagation de l'erreur
            printf("Erreur sémantique : types incompatibles dans l'opération relationnelle.\n");
        }
    }
}

void Exp_simple(char *t1)
{
    char t2[20] = "vide";
    Facteur(t2);
    strcpy(t1, t2);
    Exp_simple_Prime(t1);
}

void Exp_simple_Prime(char *t1)
{
    if (symbole.ul == opadd)
    {
        char t2[20] = "vide";
        accepter(opadd);
        Terme(t2); // Analyse le terme suivant et determine son type

        if (!compatible(t1, t2))
        {
            t1 = "erreur_de_type"; // Propagation de l'erreur
            printf("Erreur sémantique : types incompatibles dans l'opération arithmétique.\n");
        }

        Exp_simple_Prime(t1);
    }
}

void Terme(char **t)
{
    char *t1 = NULL;
    Facteur(&t1);
    TermePrime(t, t1);
}

void TermePrime(char **t, char *t1)
{
    if (symbole.ul == opmul)
    {
        accepter(opmul);
        char *t2 = NULL;
        Facteur(&t2);

        if (compatible(t1, t2))
        {
            *t = t1;
        }
        else
        {
            *t = "erreur_de_type";
            printf("Erreur : Types incompatibles dans le terme\n");
        }
    }
    else
    {
        *t = t1;
    }
}

void Facteur(char **t)
{
    if (symbole.ul == id)
    {
        int num = symbole.att;
        *t = chercher_type(num);

        if (*t == NULL)
        {
            *t = "erreur_de_type";
            printf("Erreur : Identifiant non déclaré\n");
        }
        accepter(id);
    }
    else if (symbole.ul == nb)
    {
        *t = "integer";
        accepter(nb);
    }
    else if (symbole.ul == po)
    {
        accepter(po);
        Exp_simple(*t);
        accepter(pf);
    }
    else
    {
        erreur();
    }
}

int main()
{
    char fichier[50];
    printf("Entrez le nom du fichier source : ");
    scanf("%s", fichier);

    fp = fopen(fichier, "r");
    if (fp == NULL)
    {
        printf("Impossible d'ouvrir le fichier %s.\n", fichier);
        return 1;
    }

    do
    {
        symbole = analex();

        printf("\nSymbole : %d, Attribut : %d\n", symbole.ul, symbole.att);

    } while (symbole.ul != 100);

    P();
    fclose(fp);

    return 0;
}
