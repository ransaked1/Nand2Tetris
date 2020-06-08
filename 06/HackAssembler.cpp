#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <regex>

using namespace std;

string symb = "Symbols.sim"; //File storing default symbols for initialization

//C-instruction data structure
struct cinst
{
	string 		dest;
	string 		comp;
	string 		jump;
};

//Parser object to clean and parse the line of code
class Parser
{
public:

	//Clearing comments and any indentation
	string clearCommentsAndIndent(string str)
	{
		//Clearing every comment until line ends
		size_t pos = str.find("//");
		if (pos <= str.length())
			str.erase(int(pos));

		//Clearing indentations: spaces '\t' '\f' and '\r'
		str.erase(remove(str.begin(), str.end(), ' '), str.end());
		str.erase(remove(str.begin(), str.end(), '\t'), str.end());
		str.erase(remove(str.begin(), str.end(), '\f'), str.end());
		str.erase(remove(str.begin(), str.end(), '\r'), str.end());
		return str;
	}

	//Parsing a C-instruciton
	cinst parseCInst(string inst)
	{
		cinst parsed;
		string tmp = inst;

		//Finding position of '=' and ';' if present
		unsigned int pos1 = inst.find("=");
		unsigned int pos2 = inst.find(";");

		//Check if there is a '=' and/or ';' and parse instruction from string acoardingly
		if (pos1 > inst.size()) //No destination if no '='
		{
			parsed.dest = "null";
			parsed.comp = tmp.erase(pos2);
			parsed.jump = inst.erase(0, pos2 + 1);
		}
		else if (pos2 > inst.size()) //No jump if no ';'
		{
			parsed.dest = tmp.erase(pos1);
			parsed.comp = inst.erase(0, pos1 + 1);
			parsed.jump = "null";
		}
		else //If all present in instruction
		{
			parsed.dest = tmp.erase(pos1);
			parsed.comp = inst.substr(pos1 + 1, pos2 - pos1 -1);
			parsed.jump = inst.erase(0, pos2 + 1);
		}

		return parsed;
	}
};

//Converting parsed instruciton into machine code
class Code
{
public:

	//Converting an A-instruction into machine code
	string aInst(int nr)
	{
		string binary;

		//Converting to 16-bit binary number
		for (int i = 15; i >= 0; i--) 
		{ 
        	int k = nr >> i; 
        	if (k & 1) 
            	binary.append("1");
        	else
            	binary.append("0");
        }

        return binary;
	}

	//Converting an C-instruction into machine code
	string cInst(string dest, string comp, string jump)
	{
		string binary="111"; //Starting code of a C-instruction in binary

		//all comp combinations
		if (comp == "0")
			binary.append("0101010");
		if (comp == "1")
			binary.append("0111111");
		if (comp == "-1")
			binary.append("0111010");
		if (comp == "D")
			binary.append("0001100");
		if (comp == "A")
			binary.append("0110000");
		if (comp == "M")
			binary.append("1110000");
		if (comp == "!D")
			binary.append("0001101");
		if (comp == "!A")
			binary.append("0110001");
		if (comp == "!M")
			binary.append("1110001");
		if (comp == "-D")
			binary.append("0001111");
		if (comp == "-A")
			binary.append("0110011");
		if (comp == "-M")
			binary.append("1110011");
		if (comp == "D+1")
			binary.append("0011111");
		if (comp == "A+1")
			binary.append("0110111");
		if (comp == "M+1")
			binary.append("1110111");
		if (comp == "D-1")
			binary.append("0001110");
		if (comp == "A-1")
			binary.append("0110010");
		if (comp == "M-1")
			binary.append("1110010");
		if (comp == "D+A")
			binary.append("0000010");
		if (comp == "D+M")
			binary.append("1000010");
		if (comp == "D-A")
			binary.append("0010011");
		if (comp == "D-M")
			binary.append("1010011");
		if (comp == "A-D")
			binary.append("0000111");
		if (comp == "M-D")
			binary.append("1000111");
		if (comp == "D&A")
			binary.append("0000000");
		if (comp == "D&M")
			binary.append("1000000");
		if (comp == "D|A")
			binary.append("0010101");
		if (comp == "D|M")
			binary.append("1010101");


		//all dest combinations
		if (dest == "null")
			binary.append("000");
		if (dest == "M")
			binary.append("001");
		if (dest == "D")
			binary.append("010");
		if (dest == "MD")
			binary.append("011");
		if (dest == "A")
			binary.append("100");
		if (dest == "AM")
			binary.append("101");
		if (dest == "AD")
			binary.append("110");
		if (dest == "AMD")
			binary.append("111");

		//all jump combinations
		if (jump == "null")
			binary.append("000");
		if (jump == "JGT")
			binary.append("001");
		if (jump == "JEQ")
			binary.append("010");
		if (jump == "JGE")
			binary.append("011");
		if (jump == "JLT")
			binary.append("100");
		if (jump == "JNE")
			binary.append("101");
		if (jump == "JLE")
			binary.append("110");
		if (jump == "JMP")
			binary.append("111");

		return binary;
		
	}
	
};

// Class for storing and manipulationg the Symbol Table
class SymbolTable
{
public:
	SymbolTable()
	{
		ifstream 	file(symb);
		string 		line;

		//Initializing symbol table with default values
		while (getline(file, line))
		{
			//initialize values
			line = line.c_str();
			int integer = 0;
			string temp_str;
			int temp_int;
			stringstream ss;

			//find spliting space
			size_t pos = line.find(" ");

			//get integer value from string
			ss << line;
			while(!ss.eof())
			{
      			ss >> temp_str;
      			if(stringstream(temp_str) >> temp_int) 
         			integer = temp_int;
      			temp_str = "";
   			}

   			//get name of variable from string 
			line.erase(int(pos));

			table.push_back(make_pair(line, integer));
		}
	};

	//Printing symbol table contents
	void	printTable()
	{
		for (unsigned int i = 0; i < table.size(); i++) 
    		cout << table[i].first << " " << table[i].second << endl;

    	return;
	}

	//Check if symbol present in table
	int		check(string str)
	{
		for (unsigned int i = 0; i < table.size(); i++)
			if (table[i].first == str)
				return 0;
		return 1;
	}

	//Add symbol and value to table
	void	append(string str, int integer)
	{
		table.push_back(make_pair(str, integer));

		return;
	}

	//Get the value of a symbol in table
	int 	find(string str)
	{
		for (unsigned int i = 0; i < table.size(); i++)
			if (table[i].first == str)
				return table[i].second;
		return -1;
	}
	
	vector<pair<string,int>> 	table;
	int 						alocpos = 16;
};

//get all occurences of a character in a string
int nthOccurrence(const string& str, const string& findChar, int nth)
{
    size_t  pos = 0;
    int     cnt = 0;

    while( cnt != nth )
    {
        pos+=1;
        pos = str.find(findChar, pos);
        if ( pos == string::npos )
            return -1;
        cnt++;
    }

    return pos;
};

//Dirver funciton of Assembler
int		main(int argc, char *argv[])
{
	//Declaring variables and initializing objects
	ifstream 	file(argv[1]); //change test file
	ofstream 	rezfile;
	string 		line;
	Parser 		parser;
	Code		code;
	SymbolTable	st;

	//First pass through the code to parse and getting the '()' symbols
	int lineNr = 0;
	while (getline(file, line))
	{
		line = line.c_str();
		string cline = parser.clearCommentsAndIndent(line);
		if (cline != "")
		{
			if (cline[0] != '(')
				lineNr++;
			else
			{
				unsigned int i = 0;
				while(i < cline.size())
				{
    				if(cline[i] == '(' || cline[i] == ')')
        				cline.erase(i,1);
    				i++;
				}
				if (st.check(cline))
					st.append(cline, lineNr);
			}

		}
	}

	//reseting getline and getting all other symbols
	file.clear();
	file.seekg(0, ios::beg);

	while (getline(file, line))
	{
		line = line.c_str();
		string cline = parser.clearCommentsAndIndent(line);
		if (cline != "")
		{

			if (cline[0] == '@' && isalpha(cline[1]) && islower(cline[1]))
			{
				cline.erase(0,1);
				if (st.check(cline))
				{
					st.append(cline, st.alocpos);
					st.alocpos++;
				}
			}

		}
	}

	//reseting getline and generating the machine instructions
	file.clear();
	file.seekg(0, ios::beg);

	string rez = string(argv[1]);
	int poz = rez.find(".asm");

	int poz1 = 0;
	if (rez.find("/") < rez.size())
		poz1 = nthOccurrence(rez, "/", 2);
	else
		poz1 = -1;

	//generate output file
	rez = rez.substr(poz1 + 1, poz-poz1-1) + ".hack";
	rezfile.open(rez);

	while (getline(file, line))
	{
		line = line.c_str();
		string cline = parser.clearCommentsAndIndent(line);
		if (cline != "")
		{
			if (cline[0] == '@' && isalpha(cline[1])) //A-instruciton w/ symbol
			{
				cline.erase(0,1);
				rezfile << code.aInst(int(st.find(cline))) << '\n';
			}
			else if (cline[0] == '@') //A-instruciton no symbol
			{
				cline.erase(0,1);
				rezfile << code.aInst(stoi(cline)) << '\n';
			}
			else if (cline[0] != '(') //C-instruction
			{
				cinst tmp = parser.parseCInst(cline);
				rezfile << code.cInst(tmp.dest, tmp.comp, tmp.jump) << '\n';
			}
		}
	}

	file.close();
	rezfile.close();
	return 0;
}