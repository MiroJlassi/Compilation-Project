#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define id 1
#define pv 2 // ;
#define nb 3
#define dp 4  // :
#define aff 5 // affectation :=
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
#define po 17 // parenthese ouvrante
#define pf 18 // parenthese fermante
#define opmul 19
#define END_OF_FILE 35
// les mots cles
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

void reculer(int k);
char carsuivant();
int unilexid();
int rangerid(int k, int *c);
char *chercher_type(int index);
int compatible(char *type1, char *type2);
unilex analex();

void reculer(int k)
{
    fseek(fp, -k, SEEK_CUR);
}

char carsuivant()
{
    return fgetc(fp);
}

int unilexid()
{ // si le lexeme est un mot cle, retourne unite lexicale sinon retourne id
    int k = 0;
    int i = 0;
    while (i < 14 && (!k))
    {
        if (strcmp(tab_mot_cle[i], ch) == 0)
        {
            // printf(" mot cle \t%s\n", tab_mot_cle[i]);
            k = 1;
        }
        else
        {
            i++;
        }
    }
    if (k == 1)
    {
        // printf("code mot cle %d", mot_cle[i]);
        return mot_cle[i];
    }
    else
    {
        return id;
    }
}

int rangerid(int k, int *c)
{ // retourne un pointeur sur la table tab_iden si le lexeme s'y trouve sinon il l'ajoute et retourne un pointeur, sinon on retourne 0
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
            strcpy(tab_iden[*c].type, "NULL"); // Type non defini par defaut        //////////////////////////////////////////////////////////
            nb_identifiant++;
            (*c)++;
            return *c - 1;
        }
    }
    else // c'est un mot cle
    {
        return 0;
    }
}

// analyse semantique : chercher_type et compatible

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

unilex analex() {
    int etat = 0;

    while (1) {
        switch (etat) {
        case 0:
            car = carsuivant();
            if (car == ' ' || car == '\t' || car == '\n') {
                etat = 0;
                break;
            } else if (car == '.') {
                etat = 28;
                break;
            } else if (isalpha(car)) {
                z = 0;
                ch[0] = car;
                etat = 1;
            } else if (isdigit(car)) {
                z = 0;
                ch[0] = car;
                etat = 3;
            } else if (car == '<') {
                etat = 5;
            } else if (car == '>') {
                etat = 10;
            } else if (car == '=') {
                etat = 9;
            } else if (car == ';') {
                etat = 15;
            } else if (car == ':') {
                etat = 18;
            } else if (car == ')') {
                etat = 19;
            } else if (car == '+') {
                symbole.ul = opadd;
                symbole.att = '+';
                printf("Token: ADDITION_OPERATOR, Value: '+'\n");
                return symbole;
            } else if (car == '-') {
                symbole.ul = opadd;
                symbole.att = '-';
                printf("Token: SUBTRACTION_OPERATOR, Value: '-'\n");
                return symbole;
            } else if (car == '*') {
                symbole.ul = opmul;
                symbole.att = '*';
                printf("Token: MULTIPLICATION_OPERATOR, Value: '*'\n");
                return symbole;
            } else if (car == '/') {
                symbole.ul = opmul;
                symbole.att = '/';
                printf("Token: DIVISION_OPERATOR, Value: '/'\n");
                return symbole;
            } else if (car == EOF) {
                symbole.ul = END_OF_FILE;
                symbole.att = 0;
                printf("Token: EOF\n");
                return symbole;
            } else {
                printf("Unexpected character: '%c'\n", car);
                etat = 0;
            }
            break;
        case 1: // Identifier or keyword
            car = carsuivant();
            if (isalpha(car) || isdigit(car)) {
                z++;
                ch[z] = car;
            } else {
                etat = 2;
                ch[z + 1] = '\0';
            }
            break;
        case 2:
            reculer(1);
            symbole.ul = unilexid();
            symbole.att = rangerid(symbole.ul, &c);
            printf("Token: IDENTIFIER_OR_KEYWORD, Value: '%s'\n", ch);
            return symbole;
        case 3: // Numeric constant
            car = carsuivant();
            if (isdigit(car)) {
                z++;
                ch[z] = car;
            } else {
                etat = 4;
                ch[z + 1] = '\0';
            }
            break;
        case 4:
            reculer(1);
            symbole.ul = nb;
            symbole.att = atoi(ch);
            printf("Token: NUMBER, Value: %d\n", symbole.att);
            return symbole;
        case 5: // Relational operators
            car = carsuivant();
            if (car == '=') {
                symbole.ul = oprel;
                symbole.att = ppe;
                printf("Token: LESS_THAN_OR_EQUAL, Value: '<='\n");
                return symbole;
            } else if (car == '>') {
                symbole.ul = oprel;
                symbole.att = dif;
                printf("Token: NOT_EQUAL, Value: '<>'\n");
                return symbole;
            } else {
                reculer(1);
                symbole.ul = oprel;
                symbole.att = ppq;
                printf("Token: LESS_THAN, Value: '<'\n");
                return symbole;
            }
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
            printf("Token: EQUAL, Value: '='\n");
            return symbole;
        case 10:
            car = carsuivant();
            if (car == '=')
            {
                etat = 11;
            }
            else
            {
                etat = 12;
            }
            break;
        case 11:
            symbole.ul = oprel;
            symbole.att = pge;
            printf("Token: GREATER_THAN_OR_EQUAL, Value: '>='\n");
            return symbole;
        case 12:
            reculer(1);
            symbole.ul = oprel;
            symbole.att = pgq;
            printf("Token: GREATER_THAN, Value: '>'\n");
            return symbole;
        case 13:
            symbole.ul = 100;
            symbole.att = 0;
            return symbole;
        case 14:
            printf("Erreur : caractère inattendu -> '%c'\n", car);
            etat = 0; // Reset to initial state
            break;
        case 15:
            symbole.ul = pv;
            symbole.att = 0;
            printf("Token: SEMICOLON, Value: ';'\n");
            return symbole;
        case 16: // Handle ','
            symbole.ul = v;
            symbole.att = 0;
            printf("Token: COMMA, Value: ','\n");
            return symbole;
        case 17:
            symbole.ul = po;
            symbole.att = 0;
            printf("Token: OPEN_PARENTHESIS, Value: '('\n");
            return symbole;
        case 18: // Handle ':'
            car = carsuivant();
            if (car == '=') {
                symbole.ul = aff; // Assignment operator
                symbole.att = 0;
                printf("Token: ASSIGNMENT_OPERATOR, Value: ':='\n");
                return symbole;
            } else {
                reculer(1); // Go back if not '='
                symbole.ul = dp; // Just a colon
                symbole.att = 0;
                printf("Token: COLON, Value: ':'\n");
                return symbole;
            }
        case 19:
            symbole.ul = pf;
            symbole.att = 0;
            printf("Token: CLOSE_PARENTHESIS, Value: ')'\n");
            return symbole;
        case 20:
            reculer(1);
            symbole.ul = dp;
            symbole.att = 0;
            return symbole;
        case 22:
            if (car == '+' || car == '-') {
                symbole.ul = opadd; // Arithmetic operators
                symbole.att = (car == '+') ? '+' : '-';
                return symbole;
            }
            break;
        case 23:
            car = carsuivant();
            if (car == '|')
            {
                etat = 24;
            }
            else
            {
                etat = 14; // caractère inattendu ;
            }
            break;
        case 24:
            symbole.ul = opadd;
            symbole.att = 0;
            return symbole;
        case 25:
            symbole.ul = opmul;
            symbole.att = 0;
            return symbole;
        case 26:
            car = carsuivant();
            if (car == '&')
            {
                etat = 27;
            }
            else
            {
                etat = 14; // caractère inattendu
            }
            break;
        case 27:
            symbole.ul = opmul;
            symbole.att = 0;
            return symbole;
        case 28: // Handle '.'
            symbole.ul = pt; // Period
            symbole.att = 0;
            printf("Token: PERIOD, Value: '.'\n");
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
void Type(char **type);
void Inst_composee(char *t1);
void Inst();
void Liste_inst(char *t1);
void Liste_instPrime(char *t1);
void I(char *t1);
void Exp(char *t1);
void ExpPrime(char *t1);
void Exp_simple(char *t1);
void Exp_simple_Prime(char *t1);
void Terme(char *t1);
void TermePrime(char *t1, char *t2);
void Facteur(char *t1);

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

void P() {
    char *t1 = "vide";  
    if (symbole.ul == program) {
        accepter(program);
        accepter(id);
        accepter(pv);
        Dcl();
        Inst_composee(t1);  // Pass type parameter
        accepter(pt);  // Ensure the program ends with '.'
        printf("Parsing complete: Program is valid.\n");
    } else {
        erreur();
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

void List_idPrime() {
    if (symbole.ul == v) { 
        accepter(v);
        accepter(id);
        List_idPrime();
    }
}

void Type(char **type) {
    if (symbole.ul == integer) {
        accepter(integer);
        *type = strdup("integer");  
    }
    else if (symbole.ul == chart) {
        accepter(chart);
        *type = strdup("char");
    }
    else {
        erreur();
        *type = NULL;
    }
}

void Inst_composee(char *t1) {
    if (symbole.ul == begin) {
        accepter(begin);       
        Liste_inst(t1);       
        accepter(end);         
    } else {
        erreur();
    }
}

void Liste_inst(char *t1) {
    I(t1);  
    Liste_instPrime(t1);
}

void Liste_instPrime(char *t1) {
    if (symbole.ul == pv) {  
        accepter(pv);
        Liste_inst(t1);      
    }
}

void I(char *t1) {
    char t[20];              
    char *type_id = NULL;    
    int num;                 

    if (symbole.ul == id) {
        num = symbole.att;
        accepter(id);
        type_id = chercher_type(num); 
        accepter(aff);              
        Exp(t);                      

        if (compatible(type_id, t)) {
            strcpy(t1, "vide");
        } else {
            strcpy(t1, "erreur_de_type");
            printf("Erreur : Type incompatibility in assignment to '%s'.\n", tab_iden[num].nom);
        }
    }
    else if (symbole.ul == iff) {
        accepter(iff);
        Exp(t);            
        if (strcmp(t, "boolean") != 0) {
            strcpy(t1, "erreur_de_type");
            printf("Erreur : Condition non booléenne\n");
        }
        accepter(then);
        I(t1);
        accepter(elsee);
        I(t1);
    }
    else if (symbole.ul == whilee) {
        accepter(whilee); 
        char t1[20];
        Exp(t1);  
        printf("Parsing instruction: Symbol %d\n", symbole.ul);

        if (strcmp(t1, "boolean") != 0) {
            strcpy(t1, "erreur_de_type");
            printf("Erreur : Condition non booléenne dans le while\n");
        }
        printf("Parsing instruction: Symbol %d\n", symbole.ul);

        accepter(doo);  
        printf("Parsing instruction: Symbol %d\n", symbole.ul);

        if (symbole.ul == begin) {
            Inst_composee(t1); 
        } else {
            I(t1);  
        }
    }
    else if (symbole.ul == read || symbole.ul == write) {
        accepter(symbole.ul); 
        accepter(po); 
        accepter(id); 
        accepter(pf);
    }
    else if (symbole.ul == write || symbole.ul == writeln) {
        accepter(symbole.ul);
        accepter(po);
        num = symbole.att;
        type_id = chercher_type(num);
        accepter(id);
        accepter(pf);

        if (strcmp(type_id, "integer") != 0 && strcmp(type_id, "char") != 0) {
            strcpy(t1, "erreur_de_type");
            printf("Erreur : Type incompatible pour write/writeln\n");
        }
    }
    else {
        erreur();
    }
}

void Exp(char *t1) {
    char t2[20];
    Exp_simple(t1);  
    ExpPrime(t1);
}

void ExpPrime(char *t1) {
    if (symbole.ul == oprel) {  // Relational operator
        char t2[20];
        accepter(oprel);  // Accept the operator
        printf("Parsing relational expression: Symbol %d\n", symbole.ul);

        Exp(t2);          // Parse the right-hand expression
        printf("Parsing relational expression: Symbol %d\n", symbole.ul);

        // Check type compatibility
        if (!compatible(t1, t2)) {
            strcpy(t1, "erreur_de_type");
            printf("Erreur : Types incompatibles dans l'expression relationnelle\n");
        } else {
            strcpy(t1, "boolean");  // Result of relational expressions is boolean
        }
        printf("Parsing relational expression: Symbol %d\n", symbole.ul);

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
            strcpy(t1, "erreur_de_type"); // Propagation de l'erreur
            printf("Erreur semantique : types incompatibles dans l'operation arithmetique.\n");
        }

        Exp_simple_Prime(t1);
    }
}

void Terme(char *t1) {
    char t2[20] = "vide";
    Facteur(t2);
    TermePrime(t1, t2);
}

void TermePrime(char *t1, char *t2) {
    if (symbole.ul == opmul) {
        accepter(opmul);
        char t3[20] = "vide";
        Facteur(t3);

        if (compatible(t2, t3)) {
            strcpy(t1, t2);
        } else {
            strcpy(t1, "erreur_de_type");
            printf("Erreur : Types incompatibles dans le terme\n");
        }
    } else {
        strcpy(t1, t2);
    }
}

void Facteur(char *t1) {
    if (symbole.ul == id) {
        int num = symbole.att;
        char *type = chercher_type(num);

        if (type == NULL) {
            strcpy(t1, "erreur_de_type");
            printf("Erreur : Identifiant non déclaré\n");
        } else {
            strcpy(t1, type);
        }
        accepter(id);
    }
    else if (symbole.ul == nb) {
        strcpy(t1, "integer");
        accepter(nb);
    }
    else if (symbole.ul == po) {
        accepter(po);
        Exp_simple(t1);
        accepter(pf);
    }
    else {
        erreur();
        strcpy(t1, "erreur_de_type");
    }
}

void remove_comments(FILE *input, FILE *output) {
    char ch;
    int in_comment = 0;
    while ((ch = fgetc(input)) != EOF) {
        if (ch == '(') {
            char next = fgetc(input);
            if (next == '*') {
                in_comment = 1;
            } else {
                fputc(ch, output);
                ungetc(next, input); 
            }
        }

        if (!in_comment) {
            fputc(ch, output);
        }

        if (ch == '*' && in_comment) {
            char next = fgetc(input);
            if (next == ')') {
                in_comment = 0;
            } else {
                fputc('*', output);
                ungetc(next, input);
            }
        }
    }
}

int main() {
    char fichier[50];
    printf("Entrez le nom du fichier source : ");
    scanf("%s", fichier);

    fp = fopen(fichier, "r");
    if (fp == NULL) {
        printf("Impossible d'ouvrir le fichier %s.\n", fichier);
        return 1;
    }
    FILE *new_file = fopen("new_file.txt", "w");
    if (new_file == NULL) {
        printf("Impossible de créer un fichier de sortie.\n");
        fclose(fp);
        return 1;
    }
    remove_comments(fp, new_file);
    fclose(fp);
    fclose(new_file);
    new_file = fopen("new_file.txt", "r");
    if (new_file == NULL) {
        printf("Impossible d'ouvrir le fichier modifié.\n");
        return 1;
    }
    symbole = analex();
    P();
    fclose(new_file);
    return 0;
}