#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#define NC 1
#define TVA 21

//Declaration structure
typedef struct
{
    int Jour,Mois,Annee;
} date;
typedef struct
{
    char Rue[50],Numero[5],Localite[25];
    int CodePostal;
} adresse;
typedef struct
{
    char Telephone [20], Email[100];
} contact;
typedef struct
{
    char Nom[40];
    char Prenom[20];
    adresse AdresseClient;
    contact ContactClient;
    date DateDeNaissance;
    int Fidelite;
} Client;

typedef struct
{
    int reference;
    char designation[25];
    float PrixHTVA;
    int Quantite;
} article;

 typedef struct liste
{
    struct liste *suite;
    article Donnees;
}liste;

//Prototype fonction
void AffichierClient(Client*);
int AfficherListeClients(Client**,int );
int AgeClient(date, Client*);
long AjouterClientBin(char[], Client);

FILE *CreerFichier(date, Client);
int ComparerNomPrenom(char[],char[],char[],char[]);

void DateSyst (date*);
int DateValide(date);
void DefDateF (date*);

void EcrireDonneesClient(Client*,char[]);
void EncoderClient(Client*);
void EncoderDate(date*, char[]);
void EncoderFacture(article items[]);
void EnregistrerClient(char[],long ,Client*);
int ExisteFichier(char[]);

void LocaliserFichiersClients( char[], char[]);

void GenererFacture(FILE*,date DateFacturation,Client *customer,liste *ListeProduit);

int NombreClients(char[]);

void ModifierClient(Client*);

int PointsFidelite (float);

long RechercherClient(char[],Client*,char[],char[]);
void RecupererClients(char[],Client*, int);

void Tri (Client *ptr, Client**ptrI,int size);

void SauvegarderClientsTxt(char[],Client**,int);

void EncoderArticle(article*);
void AfficherArticle(article);
int ListeVide (liste*);
void NouvelleListe(liste*);
void DetruireListe(liste**);
void AfficherListe(liste*);
void InsererElementListe(liste**);
int CompareritemsNum(int, int);
int Compareritems(char[], char[]);

liste* RechercherElementliste(liste*, char[], liste**);
void SupprimerElementListe(liste**);
void ModifierArticle (article*);
void PresenterListe(liste*);


//Main
int main()
{
    Client customer;
    date DateFact;
    Client *ptr=NULL;
    Client **ptrI=NULL;
    liste *ListeProduits = NULL;
    liste *items = NULL;
    liste *zero=NULL;
    FILE *Fichier;
    long position;
    int check=0,NbPd=0,out=0,choicea=0,choicec=0,OutMenuLvL1,OutMenuLvl3=0,NbrCl=0,numC=0,InLvL2 = 0; // verif=0
    char NomClient[40],PrenomClient[20],FichierClientsbin[100],FichierClientstxt[100],choiceb, Designation[25];


    //date
    printf("########################################\n");
    printf("    Programme de facturation de \n");
    printf("ISET\t B1G1 \t Gaide Chevronnay Tiberio \n");
    printf("########################################\n");
    printf("\n\nAppuyez sur une touche pour continuer\n");
    fflush(stdin);getch();system("cls");

    DefDateF(&DateFact); //on définit notre date de facturation
    system("cls");
    LocaliserFichiersClients(FichierClientsbin,FichierClientstxt);

    NbrCl=NombreClients(FichierClientsbin);
    ptr=(Client*)malloc(NbrCl*sizeof(Client));

    while(out==0) // Boucle de niveau 0
    {             // Sort de la boucle de niveau si out = 0

        do //Boucle de niveau 1
            // Si OutMenuLvL1 != 0
        {
            OutMenuLvL1=1;
            printf("\nMenu 1 :");
            printf("\n Menu d'acceuil");
            printf("\n 1. Nouveau client");
            printf("\n 2. Client enregistre ");
            printf("\n 3. Choisir client depuis liste client");
            printf("\n 4. Quitter\n Entrez votre choix :\t");
            fflush(stdin);scanf("%d",&choicea);


            switch(choicea)
            {
                case 1: // Ajouter un  Nouveau client
                    EncoderClient(&customer);   // on encode le client dans la structure de type Client
                    position = 0;
                    position = AjouterClientBin(FichierClientsbin,customer);  // calcul de position du client
                    EcrireDonneesClient(&customer, FichierClientstxt);      // On écrit dans un fichier txt les données du client
                    NbrCl = NombreClients(FichierClientsbin);
                    ptr = (Client *) malloc(NbrCl*sizeof(Client));
                    ptrI=(Client **)malloc(NbrCl*sizeof(Client *));
                    RecupererClients(FichierClientsbin, ptr, NbrCl);
                    Tri(ptr, ptrI, NbrCl);
                    SauvegarderClientsTxt(FichierClientstxt, ptrI, NbrCl);

                    OutMenuLvL1=0; // On reste dans le premier menu

                    break;

               case 2:  // Choisir un client enregistre ( entrer les donnees )
                    printf("\n  Nom du client: ");
                    fflush(stdin);gets(NomClient);
                    printf("  Prenom du client: ");
                    fflush(stdin);gets(PrenomClient);
                    if ((position=RechercherClient(FichierClientsbin,&customer,NomClient,PrenomClient))==-1)// client pas trouvé
                    {
                        printf("\n\tLe client n'est pas dans le fichier client!");
                        OutMenuLvL1=0;  // on reste dans la boucle de premier niveau car on a pas trouvé le client
                    }
                    else
                    {
                      NbrCl = NombreClients(FichierClientsbin);
                      ptr = (Client *) malloc(NbrCl*sizeof(Client));
                      ptrI = (Client **)malloc(NbrCl*sizeof(Client));
                      RecupererClients(FichierClientsbin, ptr, NbrCl);
                      Tri(ptr,ptrI,NbrCl);
                      SauvegarderClientsTxt(FichierClientstxt, ptrI, NbrCl);
                      OutMenuLvL1=1;
                      InLvL2 = 1;
                      printf("\n\n Client %s %s est trouve\n",NomClient,PrenomClient);
                    }
                   break;
                default:
                    printf("\nErreur, veuillez réessayer");
                    break;
               case 3:  // Afficher la liste de client
                   {
                        NbrCl=NombreClients(FichierClientsbin);        //calcul nombre de client
                        printf("\n Nombre de Client : %d\n",NbrCl);
                        ptr=(Client *)malloc(NbrCl*sizeof(Client));   //allocation tableau dyn
                        ptrI= (Client **)malloc(NbrCl*sizeof(Client*));
                        RecupererClients(FichierClientsbin, ptr, NbrCl); // récup les client dans le ptr
                        Tri(ptr,ptrI,NbrCl);
                        numC=AfficherListeClients(ptrI,NbrCl); // on désigne lequel client on veut

                        if (numC < 0)
                          {
                              OutMenuLvL1 = 1; // on SORT de la boucle de premier niveau
                              InLvL2=0;        // on NE rentre PAS dans la boucle de deuxième niveau
                              break;
                          }
                        else
                            {
                                position = RechercherClient (FichierClientsbin, &customer, (*(ptrI+numC))->Nom, (*(ptrI+numC))->Prenom);
                                OutMenuLvL1 = 1; // on sort de la boucle de premier niveau
                                InLvL2=1;       // on RENTRE dans la boucle de second niveau
                            }
                        break;
                    }
                case 4:
                    InLvL2=0;           // on NE rentre PAS dans la boucle de second niveau
                    OutMenuLvL1=1;      // On SORT de la boucle de premier niveau
                    out=1;              // On SORT de la boucle de niveau 0
                    break;
            }
        }while(OutMenuLvL1==0); // on reste boucle si diff de 1
    if ( InLvL2==1)
    {
        do  // boucle de niveau 2
        {
            printf("\nmenu 2");
            printf("\n  A. Donnees du client"); //affiche donnees client
            printf("\n  B. Modifier Client");   // Modifie les données client
            printf("\n  C. Facture");           // complete facture
            printf("\n  D. Autre client");      // permet d'encoder un autre client
            printf("\n  E. Quitter\n");           // leave
            fflush(stdin);scanf("%c",&choiceb);

            switch (choiceb)
            {
                case 'A':   //Affiche le client
                case 'a':AffichierClient(&customer);

                        break;
                case 'B':
                case 'b':ModifierClient(&customer);
                        NbrCl = NombreClients(FichierClientsbin);
                        ptr = (Client *) malloc(NbrCl*sizeof(Client));
                        ptrI=(Client **)malloc(NbrCl*sizeof(Client *));
                        EnregistrerClient(FichierClientsbin, position, &customer);
                        RecupererClients(FichierClientsbin, ptr, NbrCl);
                        Tri(ptr, ptrI, NbrCl);
                        SauvegarderClientsTxt(FichierClientstxt, ptrI, NbrCl);
                        break;

                case 'C':
                case 'c': // Menu 3
                        OutMenuLvl3 = 0;
                        NbPd = 0;
                        do // boucle de niveau 3
                        {
                            printf("\nmenu 3");
                            printf("\n 1. Ajouter un article\n 2. Supprimer un article\n 3. Modifier un article \n 4. Liste des articles \n 5. Generer la facture\n 6. Retour au menu 2\n 7. Quitter\n Entrez votre choix\n");
                            fflush(stdin);scanf("%d",&choicec);
                            OutMenuLvl3 = 0;

                            switch (choicec)
                            {
                                case 1: //Ajouter un article dans la liste
                                        printf("\n on est dans le bon choix ");
                                        InsererElementListe(&ListeProduits);
                                        OutMenuLvl3 = 0;
                                        NbPd++;         // incrémentation du nombre de produit si on ajoute un article
                                        printf(" \n========================================\nVotre nombre d'articles total est  : %d\n========================================\n",NbPd);


                                        break;
                                case 2: // Supprimer un arcticle de la liste
                                        if (NbPd==0)
                                        printf("erreur Nombre de produit nulle");

                                        else
                                        {
                                        SupprimerElementListe(&ListeProduits);
                                        NbPd--; // décrementation  du nombre de produit si on retire un article
                                        }
                                        break;
                                case 3:
                                        PresenterListe(ListeProduits);
                                        printf("Veuillez entrer le nom de l'article\n");
                                            if ( NbPd== 0)
                                                printf("\n ! erreur ! pas d'article present dans votre liste d'achat !\n");
                                            else
                                            {
                                            do
                                            {
                                                fflush(stdin);scanf("%s", Designation);
                                                items = RechercherElementliste(ListeProduits, Designation, &zero);
                                                if (items == NULL) // article pas trouve
                                                    {
                                                        check = 0;
                                                        printf("\n ! erreur ! l'article n'est pas dans la liste !");
                                                    }
                                                else        //article trouve
                                                    check = 1;

                                            } while (check == 0);
                                            ModifierArticle (&items->Donnees);
                                            }
                                            break;

                                case 4: //Liste des articles
                                        if (NbPd==0)
                                            printf("erreur Nombre de produit nulle\n");
                                        else
                                            AfficherListe (ListeProduits);
                                        OutMenuLvl3 = 0;
                                        break;

                                case 5://Generer la facture
                                        if (NbPd==0)
                                            printf("\n ! erreur ! Nombre de produit nulle !\n");
                                        else
                                        {
                                            Fichier = (CreerFichier(DateFact,customer));
                                            GenererFacture(Fichier, DateFact, &customer,ListeProduits);
                                            OutMenuLvl3 = 0;
                                            EnregistrerClient( FichierClientsbin, position, &customer);

                                            NbrCl = NombreClients(FichierClientsbin);
                                            ptr = (Client *) malloc(NbrCl*sizeof(Client));
                                            ptrI=(Client **)malloc(NbrCl*sizeof(Client *));
                                            RecupererClients(FichierClientsbin, ptr, NbrCl);
                                            Tri(ptr, ptrI, NbrCl);
                                            SauvegarderClientsTxt(FichierClientstxt, ptrI, NbrCl);

                                            printf("\n Voulez conserver la liste de produit ? \n 1-OUI / 2-NON \n Votre choix ? \t ");
                                            fflush(stdin); scanf("%d",&check);
                                            if (check == 2)
                                            {
                                                DetruireListe(&ListeProduits);
                                                NbPd= NbPd-NbPd;
                                            }
                                        }
                                        break;
                                case 6: //retour au menu d'accueil

                                        DetruireListe(&ListeProduits);
                                        OutMenuLvl3 = 1;
                                        break;

                                case 7: // Quitter
                                        OutMenuLvl3 = 1;
                                        choiceb = 'd';
                                        InLvL2 = 0;
                                        out = 1;
                                        break;
                            }
                        }while (OutMenuLvl3 == 0);
                case 'D':
                case 'd':break;
                        InLvL2 =1;
                case 'E':
                case 'e':out=1;
                    free (ptr);
                    DetruireListe(&ListeProduits);
                        break;
                default:printf("\n Vous n'avez pas rentrer la bonne valeur \n");
            }
        }while(choiceb!='D' && choiceb!='d' && choiceb!='E' && choiceb!='e');
    }
    }
    printf("\n===============================\n");
    printf("\nFin du programme de facturation\n");
    printf("\n===============================\n\n");
    getch();
     return (0);
     free(ptr);
     free(ptrI);
}

//Fonction
void AffichierClient(Client *customer) //ici on affiche les données client
                                        // on va chercher dans customer les données dont on a besoin ( si structure passage par adresse )
{
    date DateDuJour;
    int age=0;

    printf("\n\nNous affichons ci dessous vos infos Client");
    printf("\n\nNom         : %s",customer->Nom);
    printf("\nPrenom      : %s",customer->Prenom);
    printf("\nRue         : %s",(customer->AdresseClient).Rue);
    printf("\nNumero      : %s",(customer->AdresseClient).Numero);
    printf("\nLocalite    : %s",(customer->AdresseClient).Localite);
    printf("\nCodePostal  : %d",(customer->AdresseClient).CodePostal);
    printf("\nTelephone   : %s",(customer->ContactClient).Telephone);
    printf("\nEmail       : %s",(customer->ContactClient).Email);
    printf("\nDate        : %d/%d/%d",(customer->DateDeNaissance).Jour,(customer->DateDeNaissance).Mois,(customer->DateDeNaissance).Annee);
    if (customer->Fidelite ==0 || customer->Fidelite ==1)
        printf("\nPoint Fidelite : %d ",customer->Fidelite);
    else
    printf("\nPoints Fidelite : %d  ",customer->Fidelite);
    DateSyst(&DateDuJour);
    age=AgeClient( DateDuJour, customer);
    printf("\n                :\t%d ans\n",age);
}
int AgeClient (date DateDuJour ,Client *customer) // fonction de calcul de l'age du client
{
    int Age;

    if ( DateDuJour.Mois <= (customer->DateDeNaissance).Mois && DateDuJour.Jour<= (customer->DateDeNaissance).Jour)
        Age = ((DateDuJour.Annee) - ((customer->DateDeNaissance).Annee)) -1 ;
    else
        Age = ((DateDuJour.Annee) - ((customer->DateDeNaissance).Annee ))  ;
    return Age;
}
long AjouterClientBin(char FichierClients[100],Client customer)
{

    FILE *ficbin;
    int position;

    ficbin=fopen(FichierClients,"ab");
    fwrite(&customer,sizeof(Client),1,ficbin);

    position=(ftell(ficbin)-sizeof(Client));
    fclose(ficbin);

    return position;
}
FILE *CreerFichier(date DateFacturation, Client customer)
{
    FILE *fichier;
    char Path_Folder[100];
    char Path_Fichier[100];
    int i=1;

    do
    {
        printf("\n\nVeuillez specifier le chemin d'acces au dossier:\n");
        fflush(stdin); gets(Path_Folder);

        do
        {
            strcpy(Path_Fichier, Path_Folder);
            sprintf(Path_Fichier, "%s/%s%d%02d%02d_%d.txt", Path_Folder, customer.Nom, DateFacturation.Annee, DateFacturation.Mois, DateFacturation.Jour, i);
            i++;
        }
        while(ExisteFichier(Path_Fichier)!=0);

        fichier=fopen(Path_Fichier, "w");            // Remplacer "a" par "w" pour ouvrir en ECRITURE SEULE

        if(fichier!=NULL)
            printf("\nOuverture reussie !");
        else
            printf("\nChemin d'acces erronne ! Veuillez reessayer !\n");
    }
    while(fichier==NULL);

    return(fichier);
}
int ComparerNomPrenom(char nom1[40],char prenom1[20],char nom2[40],char prenom2[20])
{

  int ValeurNom;
  int ValeurPrenom;

  ValeurNom = strcmp(nom1, nom2);
  ValeurPrenom = strcmp(prenom1, prenom2);

  if (ValeurNom < 0)
    return -1;
  else if (ValeurNom == 0) // nom identique
  {
    if (ValeurPrenom < 0)
      return -1;
    else if (ValeurPrenom == 0)
      return 0;
    else
      return 1;
  }
  else
    return 1;}

void DateSyst (date *Date)
{
    time_t tsec = time(NULL);
    struct tm t;
    t=*localtime(&tsec);

    Date->Jour = t.tm_mday;
    Date->Mois = t.tm_mon+1;
    Date->Annee = t.tm_year +1900;
}

void DefDateF(date *DateF)
{
    int  choice;
    printf ("\nQuelle date voulez-vous utilisez ? ");
    printf (" \n1. date actuelle\n2. date choisie\n\n");
    fflush(stdin);scanf ("%d", &choice);

    if(choice==1)
        DateSyst(DateF);
    else
        EncoderDate(DateF, "Facture du");
}
int DateValide(date Date)
{
    int bi,Jmax=0 ;
    switch(Date.Mois)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12: Jmax=31; break;
        case 4:
        case 6:
        case 9:
        case 11:Jmax=30; break;
        case 2:
            if(Date.Annee%4==0)
            {
                bi=1;
                    if (Date.Annee%100==0)
                    {
                        bi=0;
                        if (Date.Annee%400==0)
                            bi=1;
                    }
                    else ;
            }
            else bi=0;

            if (bi==0)
            Jmax=28;
            else
            Jmax=29;
            break;
    }
    if (Date.Jour <=Jmax)
    return 0;
    else
    return 1;
}


void EcrireDonneesClient(Client *customer,char FichierClientstxt[100])
{
    FILE *fichier;

    fichier=fopen(FichierClientstxt,"a");

    fprintf(fichier,"\nAffichier donnees client");
    fprintf(fichier,"\n\nNom         : %s",customer->Nom);
    fprintf(fichier,"\nPrenom      : %s",customer->Prenom);
    fprintf(fichier,"\nRue         : %s",(customer->AdresseClient).Rue);
    fprintf(fichier,"\nNumero      : %s",(customer->AdresseClient).Numero);
    fprintf(fichier,"\nLocalite    : %s",(customer->AdresseClient).Localite);
    fprintf(fichier,"\nCodePostal  : %d",(customer->AdresseClient).CodePostal);
    fprintf(fichier,"\nTelephone   : %s",(customer->ContactClient).Telephone);
    fprintf(fichier,"\nEmail       : %s",(customer->ContactClient).Email);
    fprintf(fichier,"\nDate        : %d/%d/%d",(customer->DateDeNaissance).Jour,(customer->DateDeNaissance).Mois,(customer->DateDeNaissance).Annee);

    fclose(fichier);
    printf("\nDonnée client enregistree\n");
}
void EncoderClient(Client *customer)                                   // on remplit customer des données que l'on souhaite
{                                                                      // passage par adresse
    customer->Fidelite =0 ;                                            //systeme de point de fidelite

    printf("\nVeuillez entrer votre Nom        :\t");
    fflush(stdin);gets(customer->Nom);
    printf("\nVeuillez entrer votre Prenom     :\t");
    fflush(stdin);gets(customer->Prenom);
    printf("\nVeuillez entrer votre Rue        :\t");
    fflush(stdin);gets((customer->AdresseClient).Rue);
    printf("\nVeuillez entrer votre Numero     :\t");
    fflush(stdin);gets((customer->AdresseClient).Numero);
    printf("\nVeuillez entrer votre Localite   :\t");
    fflush(stdin);gets((customer->AdresseClient).Localite);
    printf("\nVeuillez entrer votre CodePostal :\t");
    fflush(stdin);scanf("%d",&(customer->AdresseClient).CodePostal);

    printf("\nVeuillez entrer votre Telephone   :\t ");
    fflush(stdin); gets ((customer->ContactClient).Telephone);
    printf("\nVeuillez entrer votre Email       :\t");
    fflush(stdin); gets ((customer->ContactClient).Email);
    printf("\n\n");
    EncoderDate(&(customer->DateDeNaissance), "naissance");

    printf("\n Donnee client encodee avec succes\n");
}
void EncoderDate(date *Date, char invite[])
{
    int test=1;
    do
    {
    printf("\n\n Encodez votre date de %s (format (dd/mm/yyyy))\t :\n", invite);
    scanf("%d/%d/%d",&(Date->Jour),&(Date->Mois),&(Date->Annee));
    test=DateValide(*Date);
    } while (test==1);
}

void EncoderFacture(article items[])                    // ici items est un tableau, donc on passage directement par l'adresse pour l'initialiser
{
    int i=0;
    for ( i=0;i<3;i++)
    {
        printf("Entrez la designation de votre article\n");
        fflush(stdin);gets(items[i].designation);
        printf("Entrez le Prix HTVA de votre article\n");
        fflush(stdin);scanf("%f",&(items[i].PrixHTVA));
        printf("Entrez la Quantite de votre article\n");
        fflush(stdin);scanf("%d",&(items[i].Quantite));
    }
}

int ExisteFichier(char pathFichier[])       // fct permettant de savoir si votre fichier existe (1) ou non(0)
{
    FILE *fichier;
    fichier = fopen(pathFichier,"r");

    if (fichier ==NULL)
        {
            fclose(fichier);
            return 0;
        }

    else
    {
        fclose(fichier);
        return 1;
    }
    return 0;

}

void EnregistrerClient(char FichierClientsbin[100],long position,Client *customer)
{
    FILE *fic_bin;
    fic_bin=fopen(FichierClientsbin,"r+b");
    fseek(fic_bin,position,0);
    fwrite(customer,sizeof(Client),1,fic_bin);
    fclose(fic_bin);
}


void LocaliserFichiersClients( char FichierClientsbin[100],char FichierClientstxt[100]) // attention vérifier
{               //fonction permettant de localiser nos deux fichiers bin et txt et savoir s'il existre ou non INDEPEDAMENT
    char path[100];
    FILE *ficbin;
    FILE *fictxt;
    int test1=0,test2=0;
    int choix;

    do
    {
        printf("\nVeuillez entrer le chemin d'acces au dossier:\n");
        fflush(stdin); scanf("%s", path);
        sprintf(FichierClientsbin, "%s\\clients.dat", path); // concatenation pour l'adresse du fichier .dat
        sprintf(FichierClientstxt, "%s\\clients.txt", path);
        test1=ExisteFichier(FichierClientsbin);// on vérifie que le fichier existe
        test2=ExisteFichier(FichierClientstxt);
        if(test1==0)
        {
            printf("\nImpossible de trouver le fichier. Souhaitez-vous:\n");
            printf("1. Creer le fichier\n2. Chercher dans un autre dossier\n");
            fflush(stdin); scanf("%d", &choix);
            // puts(FichierClientsbin);   test pour voir notre adresse

            if(choix==1)
            {
                ficbin=fopen(FichierClientsbin, "ab");       //NULL si erreur
                if(ficbin!=NULL)    // diff de null donc existe
                    test1=1;         // on peut alors augmenter le test
                fclose(ficbin);        //fichier crée
            }
        }
         if(test2==0)
        {
           /* printf("\nImpossible de trouver le fichier. Souhaitez-vous:\n");
            printf("1. Creer le fichier\n2. Chercher dans un autre dossier\n");
            fflush(stdin); scanf("%d", &choix);
            puts(FichierClientstxt); */

            if(choix==1)
            {
                fictxt=fopen(FichierClientstxt, "a");       //NULL si erreur
                if(fictxt!=NULL)    // diff de null donc existe
                    test2=1;         // on peut alors augmenter le test
                fclose(fictxt);        //fichier crée


            }
        }
    }
    while(test1==0 || test2==0); // si l'un OU (excl) l'autre est nulle, on s'assure ainsi de la bonne ouverte des deux fichiers
    printf("\nFichier clients (.dat et .txt) trouve !\n");
}


void GenererFacture(FILE *fichier, date DateFacturation, Client *customer , liste *ListeProduits)
{
    float redT=0,red;   // reduction totale et reduction
    float PHT=0, PTHT=0; // Prix (Totale) Hors Taxe
    float TVAT=0;  // TVA comprise - TVA Total
    float  PTTTC=0;      //Prix Total Toute Taxe Comprise
    liste *parcours=NULL;

        fprintf(fichier,"Client   : %s %s\n",customer->Nom, customer->Prenom);
        fprintf(fichier,"Date Achat : " "%d/%d/%d\n",DateFacturation.Jour,DateFacturation.Mois,DateFacturation.Annee);
        fprintf(fichier,"                                            Facture \n");
        fprintf(fichier,"===================================================================================================\n\n");
        fprintf(fichier,"Quantite\t   Prix\t\t   Total      \t   TVA\t\treduction\tTotal TVAc\n");
        fprintf(fichier,"        \t  Unitaire\t   hTVA  \n\n   ");

        parcours = ListeProduits;
        while ( parcours != NULL)
        {
            fprintf(fichier,"%s ref:%d \n",(parcours->Donnees).designation, (parcours->Donnees).reference );
            fprintf(fichier,"%4.d\t\t",(parcours->Donnees).Quantite);     //Quantite
            fprintf(fichier,"%7.2f\t\t",(parcours->Donnees).PrixHTVA); //Prix unite
            fprintf(fichier,"%7.2f\t", ((parcours->Donnees).PrixHTVA*(parcours->Donnees).Quantite)); //Prix total HTVA
            PHT=(parcours->Donnees).PrixHTVA*(parcours->Donnees).Quantite;

                if ((parcours->Donnees).Quantite>=20)
                    {
                        red=PHT/10;
                    }
                else
                    {
                        red=0;
    //printf("%.2f\n",((float)TVA/100*(objet[i].PrixHTVA*objet[i].Quantite) + (objet[i].PrixHTVA*objet[i].Quantite)));
                    }

            redT=redT+red;
            fprintf(fichier,"\t%7.2f (%d%%)\t",((PHT-red)/100*21),TVA); TVAT= TVAT+((PHT-red)/100*TVA); //TVA total AVEC REMISE
            fprintf(fichier,"%7.2f\t\t",red); //REDUCTION
            PTTTC = PTTTC +(PHT-red)+((PHT-red)/100*TVA);
            fprintf(fichier,"%7.2f\t\n\n   ",(PHT-red)+((PHT-red)/100*TVA));
            PTHT=PTHT+PHT;
            parcours = parcours->suite;
        }
        fprintf(fichier,"\n\n===================================================================================================\n");
        fprintf(fichier,"Prix Total Hors Taxe                = %4.2f euro\n",PTHT); //VP : =0 car division entière
        fprintf(fichier,"Prix Total                          = %4.2f euro\n",PTTTC);
        fprintf(fichier,"TVA Totale (21%%)                    = %4.2f euro\n",TVAT);
        fprintf(fichier,"reduction totale                    = %4.2f euro\n",redT);

        customer->Fidelite+=PointsFidelite(PTTTC);

        if (customer->Fidelite ==0 || customer->Fidelite == 1)
        fprintf (fichier,"\n %d NOUVEAU POINT DE FIDELITE", PointsFidelite(PTTTC));
        else fprintf (fichier,"\n %d NOUVEAUX POINTS DE FIDELITE", PointsFidelite(PTTTC));

        if (customer->Fidelite ==0 || customer->Fidelite == 1)
        fprintf(fichier,"\nPoint de fidelite ( TOTAL ):                  = %d point\n",customer->Fidelite);
        else
        fprintf(fichier,"\nPoints de fidelite ( TOTAL ):                  = %d points\n",customer->Fidelite);
        fclose(fichier);
        printf("\nFacture validee et enregistree avec succes");
        getch();
}
void ModifierClient(Client *customer)
{
    int choix=0;
    printf("Modification Client:\n");
    printf("\t\t\tMenu");
    printf("\n\t 1.Adresse\n\t2.Telephone\n\t 3.E-mail \n\t4. Quitter \n\t 5.Nom/Prenom\n");
    do
            {
                fflush(stdin);scanf("%d",&choix);
                if (choix!=1 && choix!=2 && choix!=3 && choix!=4 && choix!=5)
                    printf("Mauvaise valeur ");
            }while(choix!=1 && choix!=2 && choix!=3 && choix!=4 && choix!=5);

    switch(choix)
    {
        case 1:
        {
            printf("Attribution d'une nouvelle adresse\n");
            printf("Rue         :");
            fflush(stdin);gets(customer->AdresseClient.Rue);
            printf("Numero      :");
            fflush(stdin);gets(customer->AdresseClient.Numero);
            printf("Code postal :");
            fflush(stdin);scanf("%d",&customer->AdresseClient.CodePostal);
            printf("Localite    :");
            fflush(stdin);gets(customer->AdresseClient.Localite);
            break;
        }
        case 2:
        {
            printf("Attribution d'un nouveau numero de telephone: \n");
            fflush(stdin);gets(customer->ContactClient.Telephone);
            break;
        }
        case 3:
        {
            printf("Attribution d'un nouvel e-mail: \n");
            fflush(stdin);gets((customer->ContactClient).Email);
            break;
        }

        case 4:
            printf("Mauvaise Valeur rentree\n Entrez une bonne valeur ");
            break;
        case 5 :
            printf("\nVeuillez entrer votre Nom        :\t");
            fflush(stdin);gets(customer->Nom);
            printf("\nVeuillez entrer votre Prenom     :\t");
            fflush(stdin);gets(customer->Prenom);
    }
    printf("\nModification(s) enregistree(s) avec succes !\n");
}
int NombreClients(char FichierClients[100]) // retourne le nombre de fiches client enregistés dans FichierClients
{
    int cb;
    FILE *fic;

    fic=fopen(FichierClients,"rb");             // ouverture fichier Read Binary
    cb=filelength(fileno(fic))/sizeof(Client);  // longueur fichier, fileno donne la longueur de fichier en nbre d'octet. On divise par size
    if (cb<0)
    printf("erreur nbre client negatif");
    fclose(fic);
    return cb;
}
int PointsFidelite (float PTTTC)
{
    int PF ;
    PF = PTTTC/10;
    return PF;
}

long RechercherClient ( char FichierClients[100], Client *customer, char NomClient[40] , char PrenomClient[20] )
{
    FILE *fic;
    int position=-1;

    fic=fopen(FichierClients, "rb");
    while(fread(customer, sizeof(Client), 1, fic))
    {
        if(ComparerNomPrenom(customer->Nom, customer->Prenom, NomClient, PrenomClient)==0)
        {
            position=ftell(fic)-sizeof(Client);
            break;
        }
    }
    fclose(fic);
    return position;
}
void RecupererClients (char NomFichier[100],Client *ptr,int size)
{
    FILE *fic;
    int i;

    fic=fopen(NomFichier,"rb");     // ouverture fichier lecture binaire
    for (i=0;i<size;i++)            //boucle qui nous permet de découper le .dat et ainsi arriver au bon client.
        fread(ptr+i, sizeof(Client),1,fic);
    fclose(fic);
}
void SauvegarderClientsTxt( char NomFichier[100],Client **ptrI, int size)
{               // réecriture des donnees dans le fichier client
    FILE *fic_txt;
    int i;
    fic_txt=fopen(NomFichier,"w");              // ouverture en mode write du txt
    fprintf(fic_txt,"facturation modifiée\n");
    for(i=0;i<size;i++)
    {
        fprintf(fic_txt,"\tNom: %s\n",(*(ptrI+i))->Nom);
        fprintf(fic_txt,"\tPrenom: %s\n",(*(ptrI+i))->Prenom);
        fprintf(fic_txt,"\tDate de naissance: %02d/%02d/%d\n",(*(ptrI+i))->DateDeNaissance.Jour,(*(ptrI+i))->DateDeNaissance.Mois,(*(ptrI+i))->DateDeNaissance.Annee);
        fprintf(fic_txt,"\t%s   ",((*(ptrI+i))->AdresseClient).Rue);
        fprintf(fic_txt,"N %s\n",((*(ptrI+i))->AdresseClient).Numero);
        fprintf(fic_txt,"\t%d   ",((*(ptrI+i))->AdresseClient).CodePostal);
        fprintf(fic_txt,"%s\n",((*(ptrI+i))->AdresseClient).Localite);
        fprintf(fic_txt,"\tTel: %s\n",((*(ptrI+i))->ContactClient).Telephone);
        fprintf(fic_txt,"\tEmail: %s\n",((*(ptrI+i))->ContactClient).Email);
        fprintf(fic_txt,"\tPoints de fidelite: %d pts\n\n",(*(ptrI+i))->Fidelite);
    }
    printf("\n\nSauvegarde de Client\n");

    fclose(fic_txt);

}
int AfficherListeClients(Client **ptrI,int size)
{
    int i, test=0;

    printf("\nListe des clients\n");
    for (i=0;i<size;i++)
        printf("\nClient num%d\t Nom : %s\t\t Prenom : %s", i+1,(*(ptrI+i))->Nom,(*(ptrI+i))->Prenom);  // ptrI+ i, si on ne fait pas (+i) on n'est pas à la bonne position

    printf(" \n\tEntrez num client\n 0 pour cancel (revenir au Menu 1)\n");
    do
    {
        fflush(stdin); scanf("%d", &i);
        if (i>size)
        {
            printf("\n Attention vous avez rentrez un numéro trop grand ! \n le max etant %d", size);
            test =0;
        }
        else
            test =1;
    } while (test ==0);

    i-=1;
    return i;
}
void Tri(Client *ptr, Client **ptri, int NbrCl)
{
    int i=0,j=0,cond=1,k=0;
    for(i=0;i<NbrCl;i++)
    {
        for(j=0;j<i;j++)
        {
            if(ComparerNomPrenom((ptr+i)->Nom, (ptr+i)->Prenom, (*(ptri+j))->Nom, (*(ptri+j))->Prenom)<0)
            {
                for(k=i-1;k>=j;k--)
                    {
                        (*(ptri+k+1))=(*(ptri+k));
                    }
                (*(ptri+j))=(ptr+i);
                cond=0;
                j=i;
            }
        }
        if(cond) (*(ptri+i))=(ptr+i);
        cond=1;
    }
}
void EncoderArticle (article *items)    // permet d'encoder dans une structure "items"
{
    printf("Entrez la designation de votre article\n");
    fflush(stdin);gets(items->designation);
    printf("Entrez le Prix HTVA de votre article\n");
    fflush(stdin);scanf("%f",&(items->PrixHTVA));
    printf("Entrez la Quantite de votre article\n");
    fflush(stdin);scanf("%d",&(items->Quantite));
    printf("Entrez la Reference de votre article\n");
    fflush(stdin);scanf("%d",&(items->reference));
    printf(" \n verification ref : %d",(items->reference));
}
void AfficherArticle(article items)     // Afficher les items de structures articles
{
    printf("\nReference         : %d ",(items.reference));
    printf("\nDesignation       : %s",(items.designation));
    printf("\nPrix Article HTVA : %f",(items.PrixHTVA));
    printf("\nQuantite          : %d",(items.Quantite));
}
int ListeVide(liste *deb) // test boléen pour savoir si la liste est vide
{
    if (deb == NULL)      // Si adresse null
        return 0;
    else
        return 1;
}
void NouvelleListe (liste *deb) // fournit une liste vide en supprimant si nécessaire les éléments existants à l'adresse
{
    if (ListeVide(deb)!=0)
        DetruireListe(&deb);
}
void DetruireListe (liste **deb) // supprime les éventuels éléments de la liste transmise en argument
{
    liste *aliberer;
    while(*deb!=NULL)
    {
        aliberer=NULL;
        aliberer=*deb;
        *deb=(*deb)->suite;
        if(*deb)
            free(aliberer);
    }
    *deb=NULL;
}
void InsererElementListe (liste **deb) // modif ici
{
    int trouve = 0, check;
    liste *parcours = NULL;
    liste *inserer = NULL;
    liste *precedent = NULL;
    parcours = *deb; //parcours = deb
    inserer = (liste*) malloc(sizeof(liste));

    do
    {
        EncoderArticle(&inserer->Donnees); //insérer = produit
        trouve = 0;

        while(parcours != NULL && !trouve) // Si liste existante
        {
            if (CompareritemsNum(((inserer->Donnees).reference), ((parcours->Donnees).reference)) < 0)
            {
                trouve = 1;
                check = 1;
            }
            else if (CompareritemsNum(((inserer->Donnees).reference), ((parcours->Donnees).reference)) == 0)
            {
                printf("\n ! La reference de l'article existe deja ! \n\n");
                trouve = 1;
                check = 0;
            }
            else
            {
            precedent = parcours;
            parcours = parcours->suite;
            check = 1;
            }
        }
    } while (!check);

  inserer->suite = parcours;
  if (precedent == NULL)
    *deb = inserer;
  else
    precedent->suite = inserer;
}

void AfficherListe(liste *deb)
{
  liste *parcours;
  int i=1;
  int c;

  parcours = deb;
  while (parcours !=NULL) // liste n'est pas vide
  {
    printf("Article ref: %d",(parcours->Donnees).reference);  // affichage du nom des articles
     printf("\t num %d",i++);  // affichage du nom des articles
      printf("\t %s\n",(parcours->Donnees).designation);  // affichage du nom des articles
    parcours = parcours->suite;
  }
    printf("\nVeuillez entrer le numero de l'article  (0 pour Cancel) \n");
    fflush (stdin); scanf ("%d", &c);
    c = c-1;

    parcours = deb;
    if (c>=0)
    {
    for(i=0; i<c; i++)
        parcours = parcours->suite;

    AfficherArticle(parcours->Donnees);
    }
}

void PresenterListe(liste *deb)
{
    int i=1;
    liste *parcours;

    parcours = deb;
    while (parcours !=NULL) // liste n'est pas vide
    {
        printf("Article ref:%d\t num %d:\t%s\n",(parcours->Donnees).reference, i++, (parcours->Donnees).designation);  // affichage du nom des articles
        parcours = parcours->suite;
    }
}
int CompareritemsNum(int nom1, int nom2)
{
  if (nom2>nom1) // nom2> 1
    return -1;
  if (nom2 == nom1) //nom2 = 1
    return 0;
  if (nom1 > nom2)     // nom1>2
    return 1;
}
int Compareritems(char nom1[], char nom2[])
{
  int test;
  test = strcmp(nom1, nom2);
  if (test < 0) // nom2> 1
    return -1;
  else if (test == 0) //nom2 = 1
    return 0;
  else      // nom1>2
    return 1;
}
void SupprimerElementListe (liste **deb)
{
  liste *precedent = NULL;
  liste *parcours = NULL;
  char designation[25];
  int check;

  parcours = *deb;

  do
  {
    printf("Veuillez entrer le nom de l'article\n");
    fflush(stdin);scanf("%s", designation);
    parcours = RechercherElementliste(*deb, designation, &precedent);
    if (parcours == NULL)
    {
        check = 0;
        printf("! erreur ! Nom errone !\n");
    }
    else
        check = 1;
  }
  while (!check);
    if (precedent == NULL)
        *deb = parcours->suite;
    else
        precedent->suite = parcours->suite;
free (parcours);
    printf("\nelement supprime\n");
}

liste* RechercherElementliste(liste* deb, char nom[], liste **precedent)
{
  int comparaison;
  int trouve = 0;
  liste *parcours;

  parcours = deb;
  while(parcours != NULL && !trouve) // Si liste existante
{
  comparaison = Compareritems(nom,((parcours->Donnees).designation));
    if (comparaison == 0)
    trouve = 1;
  else if (comparaison < 0)
      trouve = 1;
    else
  {
    *precedent = parcours;
    parcours = parcours->suite;
  }
}
return parcours;
}


void ModifierArticle (article *Produit)
{
  int choix;
  printf("Que voulez-vous modifier?\n 1. Prix\n 2. Quantite\n 3. Retour\n");
  fflush(stdin); scanf("%d", &choix);
  switch (choix)
  {
    case 1 :
            printf("Nouveau prix : ");
            fflush(stdin);
            scanf("%f", (&Produit->PrixHTVA));
            printf("Modification prise en compte\n");
            break;
    case 2 :
            printf("Nouvelle quantite :\n");
            fflush(stdin);
            scanf("%d", (&Produit->Quantite));
            printf("Modification prise en compte\n");
            break;
    case 3 :
            return;
            break;
    default :
            printf(" \n!erreur ! Mauvaise valeur rentree !\n");
            ModifierArticle(Produit);
            break;
  }
}
