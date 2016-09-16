#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef unsigned char cell_t;

const int TOKEN_PLUS = '+';
const int TOKEN_MINUS = '-';
const int TOKEN_NEXT = '>';
const int TOKEN_PREVIOUS = '<';
const int TOKEN_OUTPUT = '.';
const int TOKEN_INPUT = ',';
const int TOKEN_BEGIN = '[';
const int TOKEN_END = ']';

class Tape{
    int index;
    std::string program;

public:
    Tape(std::istream& input){
        char symbols[] = { TOKEN_PLUS, TOKEN_MINUS, TOKEN_NEXT, TOKEN_PREVIOUS,
            TOKEN_OUTPUT, TOKEN_INPUT, TOKEN_BEGIN, TOKEN_END };
        this->index = 0;

        while(true){
            char c;
            input >> c;

            if(input.eof()){
                break;
            }

            for(int i = 0; i < 8; i++){
                if(c == symbols[i]){
                    this->program += c;
                    break;
                }
            }
        }
    }

    void move(){
        this->index += 1;
    }

    bool eof(){
        return this->index >= program.length();
    }

    char read(){
        return this->program[this->index];
    }

    bool goForward(){
        while(this->program[this->index] != TOKEN_END){
            this->index += 1;
            if(this->index < 0 && this->index >= this->program.size()){
                return false;
            }
        }
        return true;
    }

    bool goBackward(){
        while(this->program[this->index] != TOKEN_BEGIN){
            this->index -= 1;
            if(this->index < 0 && this->index >= this->program.size()){
                return false;
            }
        }
        return true;
    }
};

class Memory{
    int index;
    std::vector<cell_t> array;
public:
    Memory(unsigned int max = 30000U) : array(std::vector<cell_t>(max, (cell_t)0)){
        this->index = 0;
    }

    void plus(){
        this->array[this->index] += (cell_t)1;
    }
    
    void minus(){
        this->array[this->index] -= (cell_t)1;
    }

    bool next(){
        this->index += 1;
        return this->index >= 0 && this->index < this->array.size();
    }

    bool previous(){
        this->index -= 1;
        return this->index >= 0 && this->index < this->array.size();
    }

    cell_t output(){
        return this->array[this->index];
    }

    void input(cell_t c){
        this->array[this->index] = c;
    }

    bool isZero(){
        return this->array[this->index] == (cell_t)0;
    }
};

int execute(Tape &tape, Memory &memory, std::istream& input, std::ostream& output){
    while(! tape.eof()){
        char c = tape.read();
        
        switch(c){
        TOKEN_PLUS:
            memory.plus();
            break;
        TOKEN_MINUS:
            memory.minus();
            break;
        TOKEN_NEXT:
            if(! memory.next()){
                std::cerr << "index out of bounds" << std::endl;
                return 1;
            }
            break;
        TOKEN_PREVIOUS:
            if(memory.previous()){
                std::cerr << "index out of bounds" << std::endl;
                return 1;
            }
            break;
        TOKEN_OUTPUT:
            output.put(memory.output());
            break;
        TOKEN_INPUT:
            memory.input(input.get());
            break;
        TOKEN_BEGIN:
            if(memory.isZero()){
                if(! tape.goForward()){
                    std::cerr << "Error: no corresponded ] for [" << std::endl;
                    return 1;
                }
            }
            break;
        TOKEN_END:
            if(! memory.isZero()){
                if(! tape.goBackward()){
                    std::cerr << "Error: no corresponded [ for ]" << std::endl;
                    return 1;
                }
            }
            break;
        default:
            break;
        }

        tape.move();
    }
}

int main(int argc, char *argv[]){
    if(argc < 2){
        std::cerr << "wrong number of arguments" << std::endl;
        return 1;
    }
    std::ifstream ifs(argv[1]);
    Tape tape(ifs);
    Memory memory;
    return execute(tape, memory, std::cin, std::cout);
}
