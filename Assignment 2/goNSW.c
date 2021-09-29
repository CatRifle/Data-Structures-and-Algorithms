/*
 * 9024ass.c
 *
 *  Created on: Jul 29, 2020
 *      Author: MSI
 */


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "PQueue.h"

#define VERY_HIGH_VALUE 999999

// time complexity: O(n), n represents number of stops
// get stop index through stop name
int obtainStopIndex(char **totalStops, char *Name, int TotalStopsNum){
	int index = -9;
	for(int i=0; i<TotalStopsNum; i++){
		if (strcmp(Name, totalStops[i]) == 0){
			index = i;
			break;
		}
	}
	return index;
}

// time complexity: O(n) , n represents number of stops
// show path, and directly prints it
void showPath(int v, int pred[], char **totalStops, char ** timeAr ,char ** timeArRe, char *endstop ){
	if (pred[v] == -1){
		printf("%s %s\n", timeAr[v], totalStops[v]);
	}
	else{
		showPath(pred[v],pred, totalStops,  timeAr, timeArRe, endstop);
        // the stop is not the destination and the stop is the stop where user changes at
		if (atoi(timeArRe[v])>0 && atoi(timeAr[v]) < atoi(timeArRe[v]) && strcmp(endstop, totalStops[v])!=0){

			printf("%s %s\n", timeAr[v], totalStops[v]);
			printf("Change at %s\n", totalStops[v]);
			printf("%s %s\n", timeArRe[v], totalStops[v]);
		}
		// the stop is not the destination and the stop is the stop where user changes at
		if(atoi(timeArRe[v])>0 && atoi(timeAr[v]) > atoi(timeArRe[v]) && strcmp(endstop, totalStops[v])!=0 ){

			printf("%s %s\n", timeArRe[v], totalStops[v]);
			printf("Change at %s\n", totalStops[v]);
			printf("%s %s\n", timeAr[v], totalStops[v]);
		}
		// not transit
		else{
		printf("%s %s\n", timeAr[v], totalStops[v]);
		}
	}
}

// time complexity O(n^2), n represents number of stops
// dijkstra algorithm
void dijkstra(Graph g, Vertex source, Vertex end, char **stopNames, char ** timeAr, char ** timeArRe, char *endstop, int depart) {
   int  dist[MAX];
   int  predce[MAX];
   bool VerSet[MAX];  // VerSet[v] = true <=> vertice has not been processed
   int s, e;

   PQueueInit();
   int nV = numOfVertices(g);
   for (s = 0; s < nV; s++) {
      joinPQueue(s);
      dist[s] = VERY_HIGH_VALUE;
      predce[s] = -1;
      VerSet[s] = true;
   }
   dist[source] = 0;
   while (!PQueueIsEmpty()){
	   s = leavePQueue(dist);
	   VerSet[s]= false;
	   for (e=0; e<nV; e++){
		   if( VerSet[e]){
			   int weight = adjacent(g,s,e);
			   if(weight >0 && dist[s] + weight < dist[e]){
				   dist[e] = dist[s] + weight;
				   predce[e] = s;
			   }
		   }
	   }
   }
   // if depart late
   if(atoi(timeAr[source])< depart && atoi(timeArRe[source])< depart ){
	   printf("No connection found.\n\n");

   }
   // has at least path
   else{
   if(dist[end]< VERY_HIGH_VALUE){
	  showPath(end, predce, stopNames, timeAr, timeArRe, endstop);
	  putchar('\n');
     }
   }
 }


// time complexity: O(n + m*k)
// if n <= m*k, O(m*k)
// if n >= m*k, O(m*k)
int main(void) {
   setvbuf(stdout,NULL,_IONBF,0);
   int  n, source;
   char Name [32];
   // get stop number
   printf("Enter the total number of stops on the network: ");
   scanf("%d", &n);
   assert(n >0);
   char **totalStops = malloc(n * sizeof(char *));
   for(int i=0; i < n; i++){
		if(scanf("%s", Name) == 1){
			totalStops[i] = malloc( 32 * sizeof(char));
			strcpy(totalStops[i], Name);
			int length = strlen(Name);
			totalStops[i][length] = '\0';

            }
   }
   // initialize graph with n vertices
   Graph g = newGraph(n);

   // define input types
   int num_sche;
   char time[5];
   //char time = malloc(5 *sizeof(char));
   char name[32];
   int numOfstops;
   Edge edge;
   // restore time
   char **TimeAr= malloc(n * sizeof(char *));
   for (int i=0; i<n; i++){
	   TimeAr[i] = malloc( 5 * sizeof(char));
	   TimeAr[i][0] = '\0';
   }

   // restore same index different time
   char **TimeArReplace = malloc(n * sizeof(char *));
   for (int i=0; i<n; i++){
	   TimeArReplace[i] = malloc( 5 * sizeof(char));
	   TimeArReplace[i][0] = '\0';
   }

   printf("Enter the number of schedules: ");
   if (scanf("%d", &num_sche) == 1 && num_sche >0){

	   for(int j=0; j<num_sche; j++){
		   printf("Enter the number of stops: ");
		   scanf("%d", &numOfstops);
           // document each input: name(index) and char(time)
		   int Array[numOfstops][2];
		   for(int k=0; k<numOfstops; k++){

			   scanf("%s", time);
			   int Num = atoi(time);
			   scanf("%s", name);
			   int Index = obtainStopIndex(totalStops, name, n);
			   Array[k][0] = Index;
			   Array[k][1] = Num;

			   // restore Time index
               if(strlen(TimeAr[Index])>0){
            	   strcpy(TimeArReplace[Index], TimeAr[Index]);
            	   TimeArReplace[Index][4] = '\0';
                   }
               strcpy(TimeAr[Index], time);
               TimeAr[Index][4] = '\0';
		       }

		   for(int i =0; i < numOfstops-1 ; i++){

			   edge.v = Array[i][0];
			   edge.w = Array[i+1][0];
			   edge.weight = Array[i+1][1] - Array[i][1];
			   insertEdge(g, edge);
		   }
	   }
	   printf("\n");
   }
    // define start, end and depart time
	char startStop[32];
	char endStop[32];
	char departAt[5];

  for (int i=1; i>0; i++){
	printf("From: ");
	scanf("%s", startStop);
	// check if start equals to "done".
	if (strcmp(startStop, "done")==0){
		printf("Thank you for using goNSW.\n");
		break;
	}
	// get input
	char *End = malloc(32 * sizeof(char));
	printf("To: ");
	scanf("%s", endStop);
	printf("Depart at: ");
	scanf("%s", departAt);
    int Depart = atoi(departAt);
    printf("\n");
    source = obtainStopIndex(totalStops, startStop, n);
    int desti = obtainStopIndex(totalStops, endStop, n);
    strcpy(End, endStop);

    // put parameters into dijkstra algorithm
    if (Depart ){
	   dijkstra(g, source, desti, totalStops, TimeAr, TimeArReplace, End, Depart);
   }

  }
    freeGraph(g);
    return 0;

 }

