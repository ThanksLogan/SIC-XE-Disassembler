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
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <cctype>
//For Every instruction with a displacement, you have to get/check TA for symbol/constant checks
using namespace std;

//all helper functions
class Poo {
    //private vs public
public: //access specifier
    string locCtr;
    string symName;
    string opCode;
    string varName;
    string objCode;
    /*TODO:
     * void getters;
     * void setters/printers
    */
    void printer() {
        cout << "printing from Poo class" << endl;
    }

};

string hexToBinary = "0123456789ABCDEF";
int HexStringtoDec(string A){ //translates a given hex string to its decimal value for computations.
    int out;
    stringstream strm;
    strm << std::hex << A;
    strm >> out;
    return out;
}
string DecToHex(int A){ //translates a given decimal number into a 4-digit, all uppercase, hex number
    stringstream strm;
    strm<< std::hex << A; // int decimal_value
    string out ( strm.str() );
    if(out.length() == 1)
        out.insert(0,3, '0');
    if(out.length() == 2)
        out.insert(0,2, '0');
    if(out.length() == 3)
        out.insert(0,1, '0');
    transform(out.begin(), out.end(), out.begin(), ::toupper);
    return out;
}
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
        aMode = "PC_Indexed";
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
string printDat(string loctr, string ins, string at, string am, string form, string objct, unordered_map<string, vector<string>> imap){
    //this is where we host all of our ridiculous if statements
    //skiipping line 1 header
    //storing line into vector array
    vector<string> printedLine = {"", "", "", "", ""};
    vector<string> test1;
    //string disp;
    int intLoc; //integer type of location counter in decimal
    string varName;
    int valPC;
    int valBase;
    intLoc = HexStringtoDec(loctr);
    //string  = s4.str();
    if(form == "2")
        valPC = (intLoc + 2);
    if(form == "3")
        valPC = (intLoc + 3);
    if(form == "4")
        valPC = (intLoc + 4);

    if(imap.count(loctr) > 0){
        //found a match, so we print the imap.at(loctr)
        //printedLine.insert(1, imap.at(loctr, 2));
    }
    if(form == "2"){

    }
    if(form == "4"){
        printedLine.at(2) += '+';
    }
    if(at == "Immediate"){
        printedLine.at(3) += '#';
    }
    if(at == "Indirect")
        printedLine.at(3) += '@';
    if(am == "Indexed" || am == "Base_Indexed" ||am == "PC_Indexed"){
        printedLine.at(3) += ', X';
    }
    //to find symbol name
    //we take addressing type, and figure out TA and displacement to get the address of what to write.
    //example: 0004         STL     RETADR        1722BF
    //we see its simple and PC relative. Thus, we take the value of PC (which is adding 2,3,or 4 to locctr)
    //we'll make a quick guide: simple and PC: disp = TA + PC = 02C6 (which is where RETARD is hosted) in table
    //thus, we'll add it to column 3 of the printed line vector
    //a couple other conditons: if its simple, but not pc or base it could just be TA

    if(at == "Simple"){
        if(am == "PC"){
            //disp = (TA) + (PC)
            //TA will be in 2s complement form
            //TODO: make a conversion from 2s complement for PC instructions
            string iTA = objct.substr(3,3);
            int intTA = HexStringtoDec(iTA);
            int intDisp = intTA + valPC;
            string disp = DecToHex(intDisp);
            vector<string> test2;
            if(imap.count(disp) > 0){
                test2 = imap.at(disp);
                string varName = test2.at(0);
                printedLine.at(3) += varName;
            }
        }
        if(am == "Base"){
            string iTA = objct.substr(3,3);
            int intTA = HexStringtoDec(iTA);
            int intDisp = intTA + valBase;
            string disp = DecToHex(intDisp);
            vector<string> test2;
            if(imap.count(disp) > 0){
                test2 = imap.at(disp);
                string varName = test2.at(0);
                printedLine.at(3) += varName;
            }
        }
        //TODO
        if(am == "PC_Indexed"){
            //TA will be in 2s complement form
            //TODO: make a conversion from 2s complement for PC instructions
            string iTA = objct.substr(3,3);
            int intTA = HexStringtoDec(iTA);
            int intDisp = intTA + valPC;
            string disp = DecToHex(intDisp);
            vector<string> test2;
            if(imap.count(disp) > 0){
                test2 = imap.at(disp);
                string varName = test2.at(0);
                printedLine.at(3) += varName;
            }
        }
        //TODO
        if(am == "Base_Indexed"){
            string iTA = objct.substr(3,3);
            int intTA = HexStringtoDec(iTA);
            int intDisp = intTA + valBase;
            string disp = DecToHex(intDisp);
            vector<string> test2;
            if(imap.count(disp) > 0){
                test2 = imap.at(disp);
                string varName = test2.at(0);
                printedLine.at(3) += varName;
            }
        }
    }
    if(at == "Immediate"){
        if (am == "PC"){
            //disp = TA + (PC)
            string iTA = objct.substr(3,3);
            int intTA = HexStringtoDec(iTA);
            int intDisp = intTA + valPC;
            string disp = DecToHex(intDisp);
            vector<string> test2;
            if(imap.count(disp) > 0){
                test2 = imap.at(disp);
                string varName = test2.at(0);
                printedLine.at(3) += varName;
            }
        }
        if(form == "4"){
            string iTA = objct.substr(4,4);
            int intTA = HexStringtoDec(iTA);
            int intDisp = intTA;
            string disp = DecToHex(intDisp);
            vector<string> test2;
            if(imap.count(disp) > 0){
                test2 = imap.at(disp);
                string varName = test2.at(0);
                printedLine.at(3) += varName;
            }
        }
    }
    if(at == "Indirect"){
        if (am == "PC"){
            //disp = ((TA)) + (PC)
            string iTA = objct.substr(3,3);
            int intTA = HexStringtoDec(iTA);
            int intDisp = intTA + valPC;
            string disp = DecToHex(intDisp);
            vector<string> test2;
            if(imap.count(disp) > 0){
                test2 = imap.at(disp);
                string varName = test2.at(0);
                printedLine.at(3) += varName;
            }
        }
        if (am == "Base"){
            string iTA = objct.substr(3,3);
            int intTA = HexStringtoDec(iTA);
            int intDisp = intTA + valBase;
            string disp = DecToHex(intDisp);
            vector<string> test2;
            if(imap.count(disp) > 0){
                test2 = imap.at(disp);
                string varName = test2.at(0);
                printedLine.at(3) += varName;
            }
        }
    }
    if(imap.count(loctr) > 0 ){
        test1 = imap.at(loctr);
        //TODO: check up on this literal statement later
        if(imap.count(loctr)>0 & test1.at(1).front()=='='){
            printedLine.at(0) += " LTORD ";
        }
    }
    //checks if the variable defined in the symbol table hashmap matches a literal

    if(imap.count(loctr) > 0){
        printedLine.at(1) = test1.at(0);
    }
    loctr = DecToHex(intLoc);
    printedLine.at(0) = loctr;
    printedLine.at(1) += "";
    printedLine.at(2) += ins;
    printedLine.at(3) += "";
    printedLine.at(4) = objct;

    string finalLine;
    for(int i=0; i < printedLine.size(); i++){
        finalLine += printedLine.at(i);
        finalLine += "    ";
    }
    if(ins == "LDB"){
        finalLine += '\n';
        finalLine += "                BASE    " + printedLine.at(3);
    }



    return finalLine;

}

/*string Dissassembler(string T){
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
 */

/*string TextRecordHandler(string *T){//this is where we'll be calling all of our previously made algorithms
    //First handling the location counter
    //TODO: fix these iterators of for loops
    for (int i=0; i < T[i].length(); i++){
        Dissassembler(T[i]);
    }
    //return statement will be the locctr, info, opcode, and other info, and object code
}*/

int main(int argc, char *argv[]) {
    std::cout << "Hello, World!" << std::endl;


    //std::cout << std::setfill('0') << std::setw(4) << poo + 12 << endl;

    string line;
    ifstream test2obj_file;

    //test2obj_file.open("C:\\Users\\forem\\CLionProjects\\PA2\\cmake-build-debug\\CMakeFiles\\test2.obj");

    //for(int i = 1; i < argc; i++) {
    //    ifstream test2obj_file(argv[i]);
    //    ifstream test2sym_file(argv[i+1]);
    //}

    /*
     * THIS WILL READ in the file and put the data into string array or string variables to store
     *
     */
    string Header;
    string HName;
    string HStart;
    string HEnd;
    vector<string> Text;  //TODO: make size of this array explicit to prevent memory leak
    string* M;
    string EndRecord;
    int tlineCount=0;
    test2obj_file.open("test21.obj");

    if(test2obj_file.is_open()){
        for (int i = 1; !test2obj_file.eof() ; i++){ //parses through entire file
            getline(test2obj_file, line); //reads file line by line
            char hLineIndicator = 'H';
            char tLineIndicator = 'T';
            //char mLineIndicator = 'M'; unneeded for now
            char eLineIndicator = 'E';
            if (hLineIndicator == line.front()){ //checks for H being first char
                Header += line; //headerLine becomes line with H
                char A[line.length() + 1];
                strcpy(A, line.c_str());
                int j=1;
                int index2=1;
                while(A[j]!='0'){
                    HName += A[j];
                    index2++;
                    j++;
                }
                HStart = line.substr(index2, 6);
                HEnd = line.substr(index2 + 6, 6);

            }
            if (tLineIndicator == line.front()){ //checks for H being first char
                Text.push_back(line); //TextLine becomes line with T
                tlineCount++;
            }
            //if (mLineIndicator == line.front()){ //checks for H being first char
            //   M[i] = line; //TextLine becomes line with T
            //}
            if (eLineIndicator == line.front()){ //checks for H being first char
                EndRecord += line; //TextLine becomes line with T
            }
        }
        test2obj_file.close(); //stops file stream
    }
    else cout << "unable to open test.obj";

    //string* Symbols;
    //string* Literals;
    /*
     * hashmap is started with a key being a string(locCtr), then data being a vector in the form:
     * vector<string> = {0[name], 1[lit_const], 2[length], 3[address], 4[objcode]}
     */
    ifstream test2sym_file;
    string locCtr = "0000";
    //locCtr << setfill('0') << setw(4);
    string iName = "";
    string iLit = "";
    string iLength = "";
    string iAddress = "";
    string iObjCode = "";
    int cnt = 0;
    int cnt2 = 0;
    unordered_map<string, vector<string>> map;
    test2sym_file.open("test2.sym");
    if(test2sym_file.is_open()) {
        for (int i=1; !test2sym_file.eof(); i++ ){
            iName = "";
            iLit = "";
            iLength = "";
            iAddress = "";
            iObjCode = "";
            getline(test2sym_file, line); //reads file line by line
            int j = 0;
            cnt = 0;
            cnt2 = 0;
            vector<string> v;
            //to skip past header/dashes/unimportant stuff in file
            //cout << line << endl;
            if (line.back() != ':' & line.back() != '-' & !line.empty()){


                while (j <= line.length()) { //counter to read char by char in the line
                    //for symbol name, checks if isUpper and makes sure its the first variable
                    if (isupper(line[j]) & line[j] != 'X' & line[j] != ' ' & line[j] != line.back()) {
                        int k = 0;

                        while (line[k] != ' ') {
                            iName = iName + line[k];
                            k++;
                            cnt++;
                        }
                        j+=cnt;
                    }
                        //for lit_const, check if char begins with = or X then go from there
                    else if (line[j] == 'X' || line[j] == '=') {
                        int p = j;
                        while (line[p] != ' ') {
                            iLit = iLit + line[p];
                            if ((line[p] != '\'') & (line[p] != '=') & (line[p] != 'X')) {
                                iObjCode = iObjCode + line[p];
                            }
                            p++;
                            cnt2++;
                        }
                        j+=cnt2;
                    }
                        //for length var, we check if its isolated number
                    else if (line[j] >= '1' & line[j] <= '6' & line[j - 1] == ' ' & line[j + 1] == ' ') {
                        iLength = iLength + line[j];
                        j+=1;
                    }
                        //for address var we base it on the first two digits being 0
                    else if (line[j] == '0' & line[j + 1] == '0') {
                        iAddress = iAddress + line.substr(j, 6);
                        j+=6;
                    }
                    j++;
                }
                //cout << iName << endl;
                // cout << iAddress << endl;
                //cout << iLit << endl;
                //cout << iLength << endl;
                // cout << iObjCode << endl;
                try {
                    //here we will add the specified info from each line into a vector
                    locCtr = iAddress.substr(2, 4);
                    v.insert(v.begin(), iObjCode); // [4]
                    v.insert(v.begin(), iAddress); // [3]
                    v.insert(v.begin(), iLength); // [2]
                    v.insert(v.begin(), iLit); //[1]
                    v.insert(v.begin(), iName); //[0]

                    map[locCtr] = v;
                }
                catch(std::bad_alloc & ba){
                    std::cerr << "bad_alloc caught: " << ba.what();
                }

            }

        }
    }else{
        cout << "cannot open";
    }
    test2sym_file.close(); //stops file stream
    for(auto it : map){
        std::cout << it.first << ": ";

        for(string key : it.second){
            std::cout << key << " ";
        }
        std::cout << std::endl;
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

    //while(i < lineCount)
    ofstream logan;
    logan.open("out4.lst");
    logan << " UR A SHITHEAD  " << endl;
    string HeaderLine;
    HeaderLine+= HStart.substr(2,4) + "  START 0";
    HeaderLine += '\n';
    logan << HeaderLine;

    string finalLine;
    locCtr = "0000";

    int j = 0;
    int i = 0;
    int q = 0;
    while (j < tlineCount){
        int LIndex = 7;
        int infoIndex = 9;
        string fieldLen = Text.at(j).substr(LIndex, 2);
        uint32_t lengthDec;
        lengthDec = HexStringtoDec(fieldLen);
        int numDigits = 2*lengthDec;
        int i = 0;
        string iLine = Text.at(j).substr(infoIndex, numDigits);
        string startingAddress = Text.at(q).substr(3, 4);
        int startAddy = HexStringtoDec(startingAddress);
        char A[iLine.length() + 1];
        strcpy(A, iLine.c_str());
        string instruction, format, oat, taam = "";
        //locCtr = DecToHex(intLC);
        int intLC = HexStringtoDec(locCtr);
        while (i < iLine.length()) {
            //TODO: before we start reading lines of instruction, we gotta check for char and symbol declarations...
            locCtr = DecToHex(intLC);

            startAddy = HexStringtoDec(startingAddress);
            if (map.count(locCtr) > 0 && startingAddress != locCtr  ) {
                //will print info from symtable on instruction sheet if theres no correspondinb iinstruction to it
                vector<string> getRekt = map.at(locCtr);
                finalLine = locCtr;
                finalLine += "  ";
                finalLine += getRekt.at(0);
                if (getRekt.at(2) == "" || getRekt.at(2) == " ") {
                    finalLine += "  RESB  ";
                    //logic to get amount of size reserved
                    //how we gon do dat?
                    //search through the hashtable to find the subsequent key after the one we're on
                    //if t has not yet been written, we'll use it.
                    unordered_map<string, vector<string>>::iterator it = map.begin();
                    string saved;
                    // Iterating over the map using Iterator till map end.
                    while (it != map.end()) {
                        // Accessing the key
                        string word = it->first;
                        int wWord = HexStringtoDec(word);
                        if (wWord < startAddy && wWord >= intLC) {
                            //if there exists a hashed addy less than startaddy but greater than locctr then theres 2
                            //for ex this is the 02C6 RETARD
                            //saved += word + ' ';
                            //if saved exists, we'll perform subtraction to find exact length of the two
                            if(wWord == intLC){
                                int temp1 = HexStringtoDec(word);
                                int temp2 = startAddy;
                                int temp3 = temp2 - temp1;
                                string s2 = to_string(temp3);
                                intLC += temp3;
                                finalLine += s2;
                                finalLine += '\n';
                                locCtr = DecToHex(intLC);
                            }else{
                                int temp1 = HexStringtoDec(word);
                                int temp2 = HexStringtoDec(locCtr);
                                int temp3 = temp1 - temp2;
                                //this'll give us (in decimal) the amount bytes stored
                                string s2 = to_string(temp3);
                                intLC += temp3;
                                finalLine += s2;
                                finalLine += '\n';
                                locCtr = DecToHex(intLC);
                            }
                        }
                        if (tlineCount - j == 1 || getRekt.at(0).front() == 'E') {
                            //if its near the end it might be a final storage like EADR
                            saved += word + ' ';
                            //if saved exists, we'll perform subtraction to find exact length of the two
                            int temp1 = HexStringtoDec(locCtr);
                            int temp2 = HexStringtoDec(HEnd);
                            int temp3 = temp1 - temp2;
                            //this'll give us (in decimal) the amount bytes stored
                            string s = to_string(temp3);

                            finalLine += s;
                            finalLine += '\n';
                        }
                        // iterator incremented to point next item
                        it++;
                    }
                    logan << finalLine << endl;


                } else {
                    finalLine += "  BYTE  ";
                    finalLine += getRekt.at(1);

                    if(map.count(locCtr) > 0){
                        vector<string> poop = map.at(locCtr);
                        string temp4 = poop.at(2);
                        int temp5 = HexStringtoDec(temp4);
                        intLC += temp5;
                        locCtr = DecToHex(intLC);


                        i+= HexStringtoDec(temp4);
                    }
                    logan << finalLine << endl;
                }
            } else {


                locCtr = DecToHex(intLC);
                bool e; //'e' flag bit bool
                int z = int_hex_char_to_bin(A[i + 2]); //helper function with hex table
                if (z & 0001) //bitmask operation to check for odd number
                    e = true;
                else e = false;

                string obj;
                if (e) {//format 4
                    for (int k = i; k < i + 8; k++) {
                        obj = obj + A[k];
                    }
                    i += 8;
                    cout << obj << endl;
                    instruction = findInstr(obj);
                    oat = findOAT(obj);
                    taam = findTAAM4(obj);
                    format = "4";
                    locCtr = DecToHex(intLC);
                    finalLine = printDat(locCtr, instruction, oat, taam, format, obj, map);
                    intLC += 4;
                } else {//format 2 or 3
                    bool z = false; //bool z: indicates in if format 2 or not
                    if (A[i] == '9' || A[i] == 'A' || A[i] == 'B') {
                        for (int k = i; k < i + 4; k++) {
                            obj += A[k];
                            z = true;
                        }
                        i+=4;
                        format = "2";
                        //intLC += 2;
                        instruction = findInstr(obj);
                        oat = findOAT(obj);
                        taam = findTAAM3(obj);


                        locCtr = DecToHex(intLC);
                        finalLine = printDat(locCtr, instruction, oat, taam, format, obj, map);
                        intLC += 2;
                    }else if (z == false) {
                        for (int k = i; k < i + 6; k++) {
                            obj = obj + A[k];
                        }
                        //intLC += 3;
                        i+=6;
                        format = "3";
                        cout << obj << endl;
                        instruction = findInstr(obj);
                        oat = findOAT(obj);
                        taam = findTAAM3(obj);


                        locCtr = DecToHex(intLC);
                        finalLine = printDat(locCtr, instruction, oat, taam, format, obj, map);
                        intLC += 3;

                    }

                }
                //do something here to print the current line were on

                //string printedLine = "";
                //printedLine += locCtr;
                //0000 FIRST   +LDB    #RETADR       691002C6
                logan << finalLine << endl;//***what well write to fle
                //j++;
            }
        }
        finalLine = "";
        locCtr = DecToHex(intLC);
        q++;
        j++;
    }


    //string info = TextRecordHandler(Text);
    //loop until end of text record or end records line{
    /*
     * each iteration do the following:
     * print symbols:
     * -LOCCTR
     * -symbols / literals if matches locctr\
     * -opcode
     * -name of variable or instruction
     * -object code
     *
     *
     */
    //for(int i = 0; i < Text->length(); i++){
    //   cout <<
    //}

    logan << "              END   " + HName;
    logan.close();
    return 0;
}
