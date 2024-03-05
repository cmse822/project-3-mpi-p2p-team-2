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
    double start, stop;

    std::string file_name;
    std::fstream file_stream;
    std::ofstream outfile("mpi_status.log", std::ios_base::app); // Open the file in append mode

    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Status status;

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

    // Allocate memory
    message = new char[S];

    for (int i = 0; i < N; i++){
        // Start timer
        start = MPI_Wtime();

        // Send/receive message
        if (rank == 0){
            MPI_Send(message,S,MPI_CHAR,1,0,comm);
            int status_code_r0 = MPI_Recv(message,S,MPI_CHAR,1,0,comm,&status);

            // check the successful communication of MPI_send and receive
            if (status_code_r0 == MPI_SUCCESS) {
                outfile << "MPI_Send operation was successful for rank == " << rank << std::endl;
            }

        } else if (rank == 1){
            int status_code_r1 = MPI_Recv(message,S,MPI_CHAR,0,0,comm,&status);
            MPI_Send(message,S,MPI_CHAR,0,0,comm);

            // check the successful communication of MPI_send and receive
            if (status_code_r1 == MPI_SUCCESS) {
                outfile << "MPI_Send operation was successful for rank == " << rank << std::endl;
            }
        }

        

        // Stop timer
        stop = MPI_Wtime();

        if (rank == 0){
            std::cout << stop-start << std::endl;
            file_stream << i << "," << S << "," << stop-start << std::endl;
        }
    }

    // Deallocate memory
    delete[] message;

    outfile.close(); // close the file

    // End communication
    MPI_Finalize();

    return 0;
}
