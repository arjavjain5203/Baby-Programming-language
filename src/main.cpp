#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <optional>
#include <string>
#include <vector>
#include "types.hpp"
#include "tokenizer.hpp"
#include "parser.hpp"
#include "generation.hpp"


int main(int argc, char* argv[]) { //args tells the total size of command line arguments & argv is an array of character pointers listing all the arguments
    if(argc < 2 )
    {
        std::cerr<<"you enter wrong less number of arguments"<<std::endl;
        std::cerr<<"baby <input.by>"<<std::endl;
        return EXIT_FAILURE;
    }
    
    std::string contents;
    {
        std::stringstream contents_stream; //string stream to hold file contents
        std::fstream input(argv[1], std::ios::in); //opening file in read mode
        contents_stream << input.rdbuf(); //reading file contents into string stream
        contents = contents_stream.str(); //converting string stream to string
    }
    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> things=tokenizer.tokenize(); //tokenizing the input source code

    Parser parser(std::move(things));
    std::optional<NodeProgram> prog = parser.parse_prog();
    if(!prog.has_value())
    {
        std::cerr<<"Parsing failed due to syntax error"<<std::endl;
        return EXIT_FAILURE;
    }


    Generator generator(prog.value()); 
    {
        std::fstream output("out.asm", std::ios::out); //opening output file in write mode
        output<<generator.gen_program(); //writing generated assembly code to output file
    }


    system("nasm -felf64 out.asm"); //assembling the generated assembly code into an object file
    system("ld out.o -o out"); //linking the object file to create an executable


    return EXIT_SUCCESS;
}