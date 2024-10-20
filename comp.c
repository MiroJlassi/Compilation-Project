#include <stdio.h>
#include <string.h>

char symbole[100]; // Pour stocker le symbole courant

// call methods: 
void lire_symbole();
void erreur();
void accepter(char *T);
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
void Liste_instPrime();
void I();
void Exp();
void ExpPrime();
void Exp_simple();
void Exp_simple_Prime();
void Terme();
void TermePrime();
void Facteur();
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
    Inst_composee();
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

void Inst_composee()
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

void Exp() {
    Exp_simple();
    ExpPrime();
}

void ExpPrime() {
    Oprel();
    Exp();
}

void Exp_simple(){
    Facteur();
    Exp_simple_Prime();
}

void Exp_simple_Prime(){
    opadd();
    Terme();
    Exp_simple_Prime();
}

void Terme(){
    Facteur();
    TermePrime();
}

void TermePrime(){
    Opmul();
    Facteur();
    TermePrime();
}

void Facteur(){
    if (strcmp(symbole, "id") == 0){
        accepter("id");
        FacteurPrime();
    }
    else if (strcmp(symbole, "nb") == 0){
        accepter("nb");
        FacteurPrime();
    }
    else if (strcmp(symbole, "(") == 0){
        accepter("(");
        Exp_simple();
        accepter(")");
        FacteurPrime();
    }else{
        erreur();
    }
}


