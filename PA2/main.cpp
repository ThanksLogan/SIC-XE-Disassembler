/*
 * Logan Foreman
 * RedID: 825056655
 * CS530 - Professor Kai Li
 */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdint.h>
#include <string.h>
#include <bitset>
using namespace std;
//all helper functions
string hexToBinary = "0123456789ABCDEF";
string BinaryToHex(string binary){
    string tmp;
    int hexIndex;
    string hex;
    for(int i = 0; i < binary.length(); i++){
        hexIndex = 0;
        tmp = binary.substr(i, 4);
        if (tmp.at(0) == '1')
            hexIndex+=8;
        if (tmp.at(1) == '1')
            hexIndex+=4;
        if (tmp.at(2) == '1')
            hexIndex+= 2;
        if (tmp.at(3) == '1')
            hexIndex+=1;

        hex += hexToBinary[hexIndex];
        i = i+3;
    }
    return hex;
}
string hex_char_to_bin(char c)
{
    switch(toupper(c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
    return "could not return";
}
int int_hex_char_to_bin(char c)
{
    switch(toupper(c))
    {
        case '0': return 0000;
        case '1': return 0001;
        case '2': return 0010;
        case '3': return 0011;
        case '4': return 0100;
        case '5': return 0101;
        case '6': return 0110;
        case '7': return 0111;
        case '8': return 1000;
        case '9': return 1001;
        case 'A': return 1010;
        case 'B': return 1011;
        case 'C': return 1100;
        case 'D': return 1101;
        case 'E': return 1110;
        case 'F': return 1111;
    }
    return (int) c;
}
string findInstr(string A){
    const static string ops[]{
            "18", "58", "90", "40", "B4", "28",
            "88", "A0", "24", "64", "9C", "C4",
            "C0", "F4", "3C", "30", "34", "38",
            "48", "00", "68", "50", "70", "08",
            "6C", "74", "04", "D0", "20", "60",
            "98", "C8", "44", "D8", "AC", "4C",
            "A4", "A8", "F0", "EC", "0C", "78",
            "54", "80", "D4", "14", "7C", "E8",
            "84", "10", "1C", "5C", "94", "B0",
            "E0", "F8", "2C", "B8", "DC"
    };
    const static string mnemonics[]{
            "ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP",
            "COMPF", "COMPR", "DIV", "DIVF", "DIVR", "FIX",
            "FLOAT", "HIO", "J", "JEQ", "JGT", "JLT",
            "JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL",
            "LDS", "LDT", "LDX", "LPS", "MUL", "MULF",
            "MULR", "NORM", "OR", "RD", "RMO", "RSUB",
            "SHIFTL", "SHIFTR", "SIO", "SSK", "STA", "STB",
            "STCH", "STF", "STI", "STL", "STS", "STSW",
            "STT", "STX", "SUB", "SUBF", "SUBR", "SVC",
            "TD", "TIO", "TIX", "TIXR", "WD"
    };

    //will figure out mnemonic from the op we find they are 1:1 in the arrays.
    //it'll go something like this
    //bitmask first 8 bits to be 6 bits with 00 remainder on 2 LSB and save it as hex string
    //parse through array until we find matching string.
    //ex: for 050000, we see the in binary its 0000-0101-...
    //                                          last '01' is omitted...
    //                      thus, we get 0000-0100 which is 04 in hex
    //                      parse through array and we find 04, which is index#26
    //                      access mnemonics array at mnemonics[26] and save "LDX"
    //                      to INSTR string variable for this parse.

    //************** INSTR Fetching *****************************
    string preMask = (hex_char_to_bin(A[1]));//call hextobin with second digit of A: which is 5
    string opBin = preMask.replace(2,2,"00");
    string op = A[0] + BinaryToHex(opBin);
    cout << "opcode(2 bits): " << op << endl;
    cout << "\n";

    int index = -1; //stores index to later find mnemonic
    for(unsigned int i=0; i < sizeof(ops) ; i=i+1){
        if(op == ops[i]){
            index = i;
            break;
        }
    }
    string INSTR = mnemonics[index];
    return INSTR;
}
string findOAT(string A){
//this is where we find the operand addressing TYPE
//this is simple, immediate, or indirect
//(n=1, i=1) <= simple
//(n=0, i=1) <= immediate
//(n=1, i=0) <= indirect
//method: take entire usable line into binary, then check individual bits
//use our helper methods
//parse through each digit in A, and convert it to binary
    string objInBinary;
    string aType;
    for(int i =0; i < A.length(); i++){
        char p = A.at(i);
        objInBinary +=  hex_char_to_bin(p);
    }
    //SIMPLE MODE

    if ((objInBinary.at(6) == '1' && objInBinary.at(7) == '1')){
        aType = "Simple";
    }
    //immediate mode
    if ((objInBinary.at(6) == '0' && objInBinary.at(7) == '1')){
        aType = "Immediate";
    }
    //indirect mode
    if ((objInBinary.at(6) == '1' && objInBinary.at(7) == '0')){
        aType = "Indirect";
    }
    //cout << aType;
    return aType;
}
string findTAAM3(string A){
    //strategy: check bits
    //absolute, indexed, pc, base
    //pc and base only for format 3
    //first, check x bit (8)
    string objInBinary;
    string aMode;
    for(int i =0; i < A.length(); i++){
        char p = A.at(i);
        objInBinary +=  hex_char_to_bin(p);
    }

    if (objInBinary.at(8) == '1' && objInBinary.at(9) == '1'){
        aMode = "base_Indexed";
        return aMode;
    }
    if (objInBinary.at(8) == '1' && objInBinary.at(10) == '1'){
        aMode = "PC_indexed";
        return aMode;
    }
    if (objInBinary.at(8) == '1' &&  A.length() > 4){
        aMode = "Indexed";
    }else {if(A.length() > 4) aMode = "Absolute";}
    if (objInBinary.at(9) == '1' &&  A.length() > 4){
        aMode = "Base";
    }
    if (objInBinary.at(10) == '1'){
        aMode = "PC";
    }
    return aMode;


}
string findTAAM4(string A){
    //absolute, indexed
    string objInBinary;
    string aMode;
    for(int i =0; i < A.length(); i++){
        char p = A.at(i);
        objInBinary +=  hex_char_to_bin(p);
    }
    if (objInBinary.at(8) == '1'){
        aMode = "Indexed";
    }
    else {aMode = "Absolute";}
    return aMode;

}

string Dissassembler(string T){
    int lti;
    int infoIndex;
    string fieldLen = T.substr(lti, 2);
    int lengthDec;
    stringstream ss;
    ss << std::hex << fieldLen;
    ss >> lengthDec;
    int numDigits = (2*lengthDec);
    string iLine = T.substr(infoIndex, numDigits);
    char A[iLine.length() + 1];
    strcpy(A, iLine.c_str());
    int i=0;
    string instruction;
    string format;
    string oat;
    string taam;
    while(i < numDigits) {

        bool e;
        int z = int_hex_char_to_bin(A[i + 2]);
        if (z & 0001)
            e = true;
        else e = false;

        string obj;
        if (e) {
            for (int j = i; j < i + 8; j++) {
                obj = obj + A[j];
            }
            i += 8;
            //cout << obj << endl;
            instruction = findInstr(obj);
            oat = findOAT(obj);
            taam = findTAAM4(obj);
        } else {//format 2 or 3
            bool z = false; //bool z: indicates in if format 2 or not
            if (A[i] == '9' || A[i] == 'A' || A[i] == 'B') {
                for (int j = i; j < i + 4; j++) {
                    obj += A[j];
                    z = true;
                }
            }

            if (z == false) {
                for (int j = i; j < i + 6; j++) {
                    obj = obj + A[j];
                }
            }

            i += 6;
            cout << obj << endl;
            instruction = findInstr(obj);
            oat = findOAT(obj);
            taam = findTAAM3(obj);
            format = "3";
        }
    }
    
}

string TextRecordHandler(string *T){//this is where we'll be calling all of our previously made algorithms
    //First handling the location counter
    //TODO: fix these iterators of for loops
    for (int i=0; i < T[i].length(); i++){
        Dissassembler(T[i]);
    }
    //return statement will be the locctr, info, opcode, and other info, and object code
}

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;
    ifstream test2obj_file;
    ifstream test2sym_file;
    string line;
    for(int i = 1; i < argc; i++) {
        ifstream test2obj_file(argv[i]);
        ifstream test2sym_file(argv[i+1]);
    }

    /*
     * THIS WILL READ in the file and put the data into string array or string variables to store
     *
     */
    string Header;
    string* Text; //TODO: make size of this array explicit to prevent memory leak
    string* M;
    string EndRecord;
    if(test2obj_file.is_open()){
        for (int i = 1; !test2obj_file.eof() ; i++){ //parses through entire file
            getline(test2obj_file, line); //reads file line by line

            char hLineIndicator = 'H';
            char tLineIndicator = 'T';
            char mLineIndicator = 'M';
            char eLineIndicator = 'E';
            if (hLineIndicator == line.front()){ //checks for H being first char
                Header += line + "\n"; //TextLine becomes line with T
            }
            if (tLineIndicator == line.front()){ //checks for H being first char
                Text[i] = line; //TextLine becomes line with T
            }
            if (mLineIndicator == line.front()){ //checks for H being first char
                M[i] = line; //TextLine becomes line with T
            }
            if (eLineIndicator == line.front()){ //checks for H being first char
                EndRecord += line + "\n"; //TextLine becomes line with T
            }
        }
        test2obj_file.close(); //stops file stream
    }
    string* Symbols;
    string* Literals;
    if(test2sym_file.is_open()){
        for (int i = 1; !test2obj_file.eof() ; i++){ //parses through entire file
            getline(test2obj_file, line); //reads file line by line
            //TODO: find better way to sort to symbols and literals
            if(!line.front() == '-' && !line.front()== 'S'){
                Symbols[i] = line;
            }
            if(line.front() == '@'){
                //TODO
                Literals[i] = line;
            }
            }
        }

    /*TODO:Now that we have all of our information brought in from the file, we can begin sorting through it.
     * Unofficial plan: get hard part done first: the text record lines. These need to be translated
     * into what the output file asks, and this will include a table with all of the following based on instruction:
     * -location counter
     * -symbols and literals for assembly variables
     * -opcode (with + for format 4)
     * -name of the varaible or what is being executed (TBD)
     * -object code ( can be brought in from previous coding project)
     */
    /*
     * Some kinds of operations we need to get down:
     * -increasing the location counter based on the format of the instruction, and also the reserved bytes based on
     * the instruction
     * -creating and establishing separation between the text lines (based on reserved bytes and diff instructions)
     * -calling the symbol and literal table to establish what is going on, then printing correctly into output.
     */

    //call text record handler function
    //will read through text record arrays, starting from first, then call and read it basically.
    //TextRecordHandler(Text);
    string TextLine;
    string TextLine2;
    string TextLine3;
    string TextLine4; //may not be used
    string info = TextRecordHandler(Text);
    return 0;
}
