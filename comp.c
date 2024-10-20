#include <stdio.h>
#include <stdio.h>
#include <string.h>

char symbole[100]; // Pour stocker le symbole courant

// Fonction pour lire une entrée utilisateur
void lire_symbole()
{
    printf("Entrer une unité syntaxique : ");
    scanf("%s", symbole); // Lire l'unité syntaxique entrée par l'utilisateur
}

// Fonction pour afficher une erreur
void erreur()
{
    printf("ERREUR: Symbole inattendu -> %s\n", symbole);
}

// Fonction d'acception : vérifie si le symbole actuel correspond à la chaîne attendue
void accepter(char *T)
{
    if (strcmp(symbole, T) == 0)
    {
        lire_symbole(); // Lire le symbole suivant
    }
    else
    {
        erreur(); // Afficher une erreur si le symbole ne correspond pas
    }
}

void P()
{
    accepter("program");
    accepter("id");
    accepter(";");
    Dcl();
    Inst_composée();
}
void Dcl()
{
    dclPrime();
}

void dclPrime()
{
    if (strcmp(symbole, "var") == 0)
    {
        accepter("var");
        List_id();
        accepter(":");
        Type();
        accepter(";");
        dclPrime();
    }
}
void List_id()
{
    accepter("id");
    List_idPrime();
}

void List_idPrime()
{
    accepter(",");
    accepter("id");
    List_idPrime();
}

void Type()
{
    if (strcmp(symbole, "integer") == 0)
        accepter("integer");
    else if (strcmp(symbole, "char") == 0)
        accepter("char");
    else
        erreur();
}

void Inst_composée()
{
    accepter("begin");
    Inst();
    accepter("end");
}

void Inst()
{
    Liste_inst();
}

void Liste_inst()
{
    I();
    Liste_instPrime();
}

void Liste_instPrime()
{
    if (strcmp(symbole, ";") == 0)
    {
        accepter(";");
        Liste_instPrime();
    }
}

void I()
{
    if (strcmp(symbole, "id") == 0)
    {
        accepter("id");
        accepter(":=");
        Exp_simple();
    }
    else if (strcmp(symbole, "if") == 0)
    {
        accepter("if");
        Exp();
        accepter("then");
        I();
        accepter("else");
        I();
    }
    else if (strcmp(symbole, "while") == 0)
    {
        accepter("while");
        Exp();
        accepter("do");
        I();
    }
    else if (strcmp(symbole, "read") == 0)
    {
        accepter("read");
        accepter("(");
        accepter("id");
        accepter(")");
    }
    else if (strcmp(symbole, "readln") == 0)
    {
        accepter("readln");
        accepter("(");
        accepter("id");
        accepter(")");
    }
    else if (strcmp(symbole, "write") == 0)
    {
        accepter("write");
        accepter("(");
        accepter("id");
        accepter(")");
    }
    else if (strcmp(symbole, "writeln") == 0)
    {
        accepter("writeln");
        accepter("(");
        accepter("id");
        accepter(")");
    }
    else
        erreur();
}
