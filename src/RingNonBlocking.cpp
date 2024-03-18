#include <iostream>
#include <string>
#include <mpi.h>    // Include MPI
#include <fstream>

int main(int argc, char* argv[]){
    // Initialize variables
    int S, N;
    int num_tasks;
    int rank;
    char* message;
    double start, time, mean;

    std::string file_name;
    std::fstream file_stream;

    MPI_Comm comm = MPI_COMM_WORLD;

    // Validate parameters
    if(argc != 4){
	    std::cout << "Wrong number of inputs. Exiting program" << std::endl;
	
	    return -1;
    } else{
        // Store variables
        S = std::stoi(argv[1]);
        N = std::stoi(argv[2]);
        file_name = argv[3];

        std::cout << "Size of message: " << S << std::endl;
        std::cout << "Number of measurements: " << N << std::endl;
        std::cout << "Output file: " << file_name << std::endl; 
    }

    // Open file stream
    file_stream.open(file_name, std::ios::out | std::ios::app);

    if (!file_stream.is_open()){
        std::cerr << "Error opening the file. " << std::endl;
        return 1;
    }

    // Initiate communication
    MPI_Init(&argc, &argv);

    // Get Size and Rank
    MPI_Comm_size(comm, &num_tasks);
    MPI_Comm_rank(comm, &rank);

    // Declare request object
    MPI_Request request;

    for (int i = 0; i < N; i++){

        // Allocate memory
        message = new char[S];

        // Start timer
        start = MPI_Wtime();

        // Send message to the next rank
        MPI_Isend(message, S, MPI_CHAR, (rank+1) % num_tasks, 0, comm, &request);
        MPI_Request_free(&request);

        // Receive message from the previous rank
        MPI_Irecv(message, S, MPI_CHAR, (rank-1) % num_tasks, 0, comm, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);

        // Stop timer
        time = MPI_Wtime() - start;

        // Add all of the measurements
        MPI_Reduce(&time, &mean, 1, MPI_DOUBLE, MPI_SUM, 0, comm);

	if (rank == 0){
	    // Calculate the mean
	    mean = mean / num_tasks;
   
	    // Print to file
	    file_stream << i << "," << S << "," << num_tasks << "," << mean << std::endl;
	}
    }

    // Close file stream
    file_stream.close();

    // Deallocate memory
    // delete[] message;

    // End communication
    MPI_Finalize();

    return 0;
}
