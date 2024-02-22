#include <iostream>
#include <string>

int main(int argc, char *argv[]){

    // check if the correct number of arguments is passed
    if (argc != 4)
    {
        std::cout << "You need to parse in the necessary input"<< std::endl;
        std::cout << "Usage: compiled_file N, s, <file>"<< std::endl;
        return 1;
    }
    
    // parsing the arguments
    std::string N = argv[1];
    std::string S = argv[2];
    std::string filename = argv[3];

    //output the parsed arguments
    std::cout << "The number of iterations is: " << N << std::endl;
    std::cout << "The size of the message is: " << S << std::endl;
    std::cout << "The file name entered is: " << filename << std::endl;

    return 0;
}