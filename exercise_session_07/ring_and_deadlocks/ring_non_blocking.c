#include "mpi.h" 
#include <stdio.h> 
#include <stdlib.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes and rank of the process
    int size, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Request reqs [2*size];

    int my_sum = 0;

    int send_rank = my_rank;  // Send    buffer
    int recv_rank = 0;        // Receive buffer

    // Compute the ranks of left/right neighbours 
    int left_rank, right_rank;

	left_rank = ((my_rank - 1)%size + size)%size;
	right_rank = ((my_rank + 1)%size + size)%size;

    // Loop over the number of processes
    //     send to right, receive from left
    //     update the send buffer
    //     update the local sum
	
	for (int i = 0 ; i < size; i++) {
		MPI_Irecv(&recv_rank, 1, MPI_INT, left_rank, 0, MPI_COMM_WORLD, &reqs[i]);
		MPI_Isend(&send_rank, 1, MPI_INT, right_rank, 0, MPI_COMM_WORLD, &reqs[i + size]);
		MPI_Wait(&reqs[i], MPI_STATUS_IGNORE);
		MPI_Wait(&reqs[i + size], MPI_STATUS_IGNORE);
		// printf("Task %d, I received %d from %d and sent %d to %d\n", my_rank, recv_rank, left_rank, send_rank, right_rank);
		my_sum += recv_rank;
		send_rank = recv_rank;

	}

	printf("I am processor %d out of %d, and the sum is %d\n", my_rank, size, my_sum);

    // Finalize the MPI environment.
    MPI_Finalize();
}
