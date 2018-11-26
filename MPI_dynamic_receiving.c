/*
 ============================================================================
 Name        : MPI_dynamic_receiving.c
 Author      : Mirco Meazzo
 Version     :
 Copyright   : Your copyright notice
 Description : Hello MPI World in C 
 ============================================================================
 */
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <stdlib.h>


int main( int argc, char** argv) {
	
	MPI_Init( &argc, &argv );
	
	int size, rank;
	
	MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	
	int handshake= 0;
	int counter_1;
	MPI_Status status;

	printf("I'm processor %d of %d\n", rank, size);

	if (rank == 0) {

		MPI_Send( &handshake, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD );
		//----Determina le stats del messaggio con MPI_Probe, con MPI_Get_count ottiene la lunghezza------
		//----del messaggio e con il malloc alloca, ad un puntatore, la memoria necessaria al messaggio---

		MPI_Probe( (size-1), 0, MPI_COMM_WORLD, &status);
		MPI_Recv( &handshake, 1 , MPI_INT, (rank-1 +size)%size, 0, MPI_COMM_WORLD,
						  MPI_STATUS_IGNORE);

		MPI_Get_count( &status, MPI_INT, &counter_1);
		//------------------------------------------------------------------------------------------------
		printf("\nProcessor %d received from %d a message long %d, with tag %d\n", rank,
				status.MPI_SOURCE, counter_1, status.MPI_TAG );
		printf("The message is: %d \n", handshake);


	}
	else {
			MPI_Recv( &handshake, 1, MPI_INT, (rank-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
			handshake = handshake +10;
			printf("Processor %d received and passed the message to %d\n", rank, (rank+1)%size);
			MPI_Send( &handshake, 1, MPI_INT, (rank+1)%size, 0, MPI_COMM_WORLD );
	}


	MPI_Finalize();
	
	return 0;
}
