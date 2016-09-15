#include <iostream>
#include <string>
#include <vector>

struct Tape{
    int index;
    std::string program;

    Tape(std::istream& input){
        std::string symbols = "><+-.,[]";
        this->index = 0;

        while(true){
            char c;
            input >> c;

            if(input.eof()){
                break;
            }

            for(unsigned int i = 0; i < symbols.size(); i++){
                if(c == symbols[i]){
                    this->program += c;
                    break;
                }
            }
        }
    }
};

std::string readProgram(){
    std::string s;
    std::string symbols = "><+-.,[]";

    while(true){
        char c;
        std::cin >> c;

        if(std::cin.eof()){
            break;
        }
        
        for(unsigned int i = 0; i < symbols.size(); i++){
            if(c == symbols[i]){
                s += c;
                break;
            }
        }
    }

    return s;
}

int main(int argc, char *argv[]){
    int memory_index = 0;
    std::vector<unsigned char> memory;
    Tape tape(std::cin);

    std::cout << tape.program << std::endl;
    
    return 0;
}
