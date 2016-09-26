//Einbindung von Standard-Bibliotheken
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>

//Einbindung eigener, lokaler Bibliotheken
#include "conv.h"
#define MAX 100000 // Unendlich

//Definition eigener Funktionen
int TopSort(int anzahlKnoten, int ** AdjMat, int ** Kantengewicht, int * Eingangsgrad, int * TopologischeSortierung);
int DAG ( int anzahlKnoten, int ** AdjMat, int ** Kantengewicht, int * Eingangsgrad, int startKnoten, int zielKnoten, int * Vorgaenger, int * WeglaengeZuKnoten, int printDetails );
int ShowShortestPath ( int anzahlKnoten, int ** AdjMat, int startKnoten, int zielKnoten, int * Vorgaenger, int * WeglaengeZuKnoten );

//Definition der main-Funktion
int main (int  argc, char ** argv )
{
	system("cls");
	//Überprüfe Argumente
	if ( 3 != argc ) 
	{
		printf("\nProgrammaufruf: DAG.exe Graph.txt startKnoten\n"); 
		return 1; 
	}

	int i;

	//Einlesen aus Datei
	FILE * file_Graph = fopen( argv[ 1 ], "r" );
	if( file_Graph == 0 )
	{
		printf("\n Problem: Datei %s konnte nicht geoeffnet werden.\n", argv[ 1 ] );
		return 1;
	}	
	
	int anzahlKnoten, anzahlKanten;
	fscanf( file_Graph, "%d", &anzahlKnoten );
	fscanf( file_Graph, "%d", &anzahlKanten );

	int ** AdjMat = GenerateIntMat( anzahlKnoten+1, anzahlKnoten+1 );
	int ** Kantengewicht = GenerateIntMat( anzahlKnoten + 1, anzahlKnoten + 1 );
	int * Eingangsgrad = GenerateIntVect( anzahlKnoten+1 );

	int von, zu, gewicht;
	
	
	for( i = 1; i <= anzahlKanten; i++ ) 
	{
		fscanf( file_Graph, "%d %d %d", &von, &zu, &gewicht );
		Kantengewicht[von][zu]= gewicht;
		AdjMat[zu][0]++;
		Eingangsgrad[zu]++;  // Wozu eingangsgrad wenn spalte 0 dafür schon genutz wird ????
		AdjMat[zu][AdjMat[zu][0]]=von;
	}	
	fclose( file_Graph );
//	ShowIntMat ( 1, anzahlKnoten, 1, anzahlKnoten, AdjMat, "Start AdjMat" );
//	ShowIntMat ( 1, anzahlKnoten, 1, anzahlKnoten, Kantengewicht, "Kantengewicht" );
//	ShowIntVect( 1, anzahlKnoten, Eingangsgrad, "Eingangsgrad" );
	
//DAG
	int startKnoten = atoi ( argv[ 2 ] );
	int * WeglaengeZuKnoten = GenerateIntVect ( anzahlKnoten + 1 ); //entspricht Vektor C im Quellcode auf Vorlesungsfolien
	int * Vorgaenger = GenerateIntVect ( anzahlKnoten+1 );	//entspricht Vektor v im Quellcode auf Vorlesungsfolien

	DAG ( anzahlKnoten, AdjMat, Kantengewicht, Eingangsgrad, startKnoten, -1, Vorgaenger, WeglaengeZuKnoten, 1 );

	for (i = 1; i <= anzahlKnoten; i++)
	{
		if (i != startKnoten)
		{
//			ShowShortestPath ( anzahlKnoten, AdjMat, startKnoten, i, Vorgaenger, WeglaengeZuKnoten );
		}
	}


	//4. HIER: KÜRZESTE WEGE IN DATEI SPEICHERN



	//Speicherfreigabe	
	if (Vorgaenger != NULL) free ( Vorgaenger );
	if (WeglaengeZuKnoten != NULL) free ( WeglaengeZuKnoten );
	if (Eingangsgrad != NULL) free ( Eingangsgrad );
	FreeIntMat( anzahlKnoten, AdjMat );
	FreeIntMat( anzahlKnoten, Kantengewicht );

	return 0; 
}

//2. TOP-SORT IMPLEMENTIEREN, ERGEBNIS SOLL IN VEKTOR TopologischeSortierung STEHEN
int TopSort ( int anzahlKnoten, int ** AdjMat, int ** Kantengewicht, int * Eingangsgrad, int * TopologischeSortierung )
{
	int i, j;
	int knoten;

	int anzahlDerKnotenInTopSort = 0;
	while (anzahlDerKnotenInTopSort!=anzahlKnoten){
		knoten=0;
		for ( i = 1; i <= anzahlKnoten; i++ ){
			if ( Eingangsgrad[ i ] == 0 ){
				anzahlDerKnotenInTopSort++;
				TopologischeSortierung[ anzahlDerKnotenInTopSort ] = i;	
				knoten=i;
				break;
			}
		}
		for ( i = 1; i <= anzahlKnoten; i++ ){
			for ( j = 1; j <= anzahlKnoten; j++ ){
				if(knoten==AdjMat[i][j]){
				AdjMat[i][j]=0;
				Eingangsgrad[knoten]=(-1);
				Eingangsgrad[ i ]--;
				}
			}
		}
//		ShowIntVect ( 1, anzahlKnoten, TopologischeSortierung, "TopologischeSortierung" );
	}
//Break();


 


//	if (KnotenMitEingangsgradNull != NULL) free ( KnotenMitEingangsgradNull );
	return 0;
}
//3. HIER DAG-ALGORITHMUS IMPLEMENTIEREN
int DAG ( int anzahlKnoten, int ** AdjMat, int ** Kantengewicht, int * Eingangsgrad, int startKnoten, int zielKnoten, int * Vorgaenger, int * WeglaengeZuKnoten, int printDetails )
{
	int i, j, k;

	int * TopologischeSortierung;	//entspricht Vektor t im Quellcode auf Vorlesungsfolien
	TopologischeSortierung = GenerateIntVect ( anzahlKnoten+1 );

	TopSort ( anzahlKnoten, AdjMat, Kantengewicht, Eingangsgrad, TopologischeSortierung );
//	if (printDetails == 1) ShowIntVect ( 1, anzahlKnoten, TopologischeSortierung, "TopologischeSortierung" );

// setzte AdjMat unendlich
	for(i=0;i<=anzahlKnoten;i++){
		for(j=1;j<=anzahlKnoten;j++){
			AdjMat[i][j]=MAX;	
		}
	}
	AdjMat[startKnoten][startKnoten]=0;
//ermittel Startposition und setzte in j
	for(i=1;i<=anzahlKnoten;i++){
		if (TopologischeSortierung[i]==startKnoten){
								j=i; 
		}
	}
		
	for (j;j<=anzahlKnoten;j++)	{
		printf("Position = %d\n",j);
		k=TopologischeSortierung[j];
		for(i=1;i<=anzahlKnoten;i++){
			if(Kantengewicht[k][i]!=0 && AdjMat[k][i]==MAX){
				AdjMat[k][i]=Kantengewicht[k][i];
			}else if(Kantengewicht[k][i]!=0 && Kantengewicht[k][i]<AdjMat[k][i]){
				AdjMat[k][i]=AdjMat[k][i]+Kantengewicht[k][i];
			}
		}
		ShowIntVect ( 1, anzahlKnoten, TopologischeSortierung, "TopologischeSortierung" );
		ShowIntMat ( 1, anzahlKnoten, 1, anzahlKnoten, AdjMat, "AdjMat" );
		printf("\n\n********************\n\n");
	}

//kürzestenwege zum knoten.vorgänger in Zeile 0 Packen
	for(i=1;i<=anzahlKnoten;i++){ //spalte
		k=MAX;
		for(j=1;j<=anzahlKnoten;j++){  //zeile
			if(AdjMat[j][i]<MAX && AdjMat[j][i]<k){
			AdjMat[0][i]=j;
			k=AdjMat[j][i];
			}
				
		}
		
	}
	
	ShowIntMat ( 0, anzahlKnoten, 1, anzahlKnoten, AdjMat, "ENDE" );


	if (TopologischeSortierung != NULL) free ( TopologischeSortierung );

	return 1;
}


int ShowShortestPath ( int anzahlKnoten, int ** AdjMat, int startKnoten, int zielKnoten, int * Vorgaenger, int * WeglaengeZuKnoten )
{
	if ( WeglaengeZuKnoten[ zielKnoten ] < MAX )
	{	
		printf ( "\nKuerzester Weg von Startknoten %d zu Zielknoten %d mit Weglaenge %d:  ", startKnoten, zielKnoten, WeglaengeZuKnoten[ zielKnoten ] );
		int vorg = zielKnoten;
		int from_tmp = startKnoten;
		int vorg_tmp;
		while (from_tmp != zielKnoten)
		{
			vorg = zielKnoten;

			while (from_tmp != vorg)
			{
				vorg_tmp = vorg;
				vorg = Vorgaenger[ vorg ];
			}
			printf ( "%d -> %d, ", vorg, vorg_tmp );
			from_tmp = vorg_tmp;
		}
		printf ( "\n" );
	}
	else
	{
		printf ( "\nEs exisitiert kein Weg von Startknoten %d zu Zielknoten %d!\n", startKnoten, zielKnoten );
	}

	return 1;
}
