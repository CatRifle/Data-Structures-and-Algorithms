/*
 * PQueue.h
 *
 *  Created on: Jul 29, 2020
 *      Author: MSI
 */

#include "WGraph.h"
#include <stdbool.h>

#define MAX 10000

void   PQueueInit();
void   joinPQueue(Vertex);
Vertex leavePQueue(int[]);
bool   PQueueIsEmpty();
