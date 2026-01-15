#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <optional>
#include <string>
#include <vector>

enum class TokenType { //defining different token types for my language
    _return,
    int_lit,
    semi
};

struct Token //defining a token structure
{
    TokenType type; 
    std::optional<std::string> value; //optional value for tokens like int_lit
};


std::vector<Token> tokenize(const std::string& input)
{
    std::vector<char> buf = {}; //buffer to hold characters for multi-character tokens
    std::vector<Token> tokens;
    for(int i=0;i<input.size();i++)
    {
        char c = input[i];
        if(std::isalpha(c))
        {
            
        }
    }
    return tokens;
}



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
    
    std::cout<<"run";

    return 0;
}