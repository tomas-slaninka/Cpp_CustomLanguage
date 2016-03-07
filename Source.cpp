/*********************************************************
*														 *
*			Riesitel: Tomas Slaninka, 53928				 *
*														 *
*********************************************************/
#include <stdio.h>
#include <iostream>
#include <fstream>

const int BUFFER_SIZE = 100000;

using namespace std;

//Returns the size of the file - also checks brackets correctness - returns -1 if the brackets are incorrect
int fileSize(char *input, bool* hasRead) {
	fstream inputFile;
	int fileCounter = 0;
	int bracketCount = 0;
	char tmpChar[1];

	inputFile.open(input, ios::in);
	inputFile.read(tmpChar, 1);
	while(!inputFile.eof()) {
		if (tmpChar[0] == '[')
			bracketCount++;
		else if (tmpChar[0] == ']') {
			if (bracketCount > 0)
				bracketCount--;
			else {
				cout << "Brackets do not match. Program will terminate." << endl;
				return -1;
			}
		}
		else if (tmpChar[0] == 'R')
			hasRead[0] = true;
		fileCounter++;
		inputFile.read(tmpChar, 1);
	}

	inputFile.close();
	return fileCounter;
}

//Loads file and return array of chars with stored input
char* loadFile(char *input, int size) {
	fstream inputFile;
	inputFile.open(input, ios::in);
	char* sourceCode = (char*)malloc(size * sizeof(char));

	for (int i = 0; i < size; i++) {
		inputFile >> sourceCode[i];
	}

	inputFile.close();
	return sourceCode;
}

//Finds the corresponding right bracket in sourceCode
int findFightBracket(char* sourceCode, int size) {
	int tmp = 1;
	for (int i = 0; i < size; i++) {
		if (sourceCode[i] == '[')
			tmp++;
		else if (sourceCode[i] == ']') {
			if (--tmp == 0)
				return i;
		}
	}
	return 0;
}

//Finds the corresponding left bracket in sourceCode
int findLeftBracket(char* sourceCode, int position) {
	int tmp = 1;
	for (int i = position - 1; i > -1; i--) {
		//cout << i << " " << sourceCode[i] << endl;;
		if (sourceCode[i] == ']')
			tmp++;
		else if (sourceCode[i] == '[') {
			if (--tmp == 0) {
				return i;
			}
		}
	}
	return 0;
}

int main(int argc, char *argv[]) {
	char* sourceCode;
	unsigned char* inputCode;
	int size, size2, pointer = 0; 
	int counter = 0;
	unsigned char memory[BUFFER_SIZE];
	fstream input, output;
	bool hasRead = false;

	cout << endl << endl;
	if (argc != 2) {
		cout << "Wrong number of args - enter only name of the program and name of file with source code!" << endl;
		cout << "Application will terminate.." << endl;
		return 0;
	}
	input.open(argv[1], ios::in);
	if (!input) {
		cout << "File " << argv[1] << " was not found or opened." << endl;
		cout << "Application will terminate.." << endl;
		return 0;
	}
	input.close();

	output.open("output.bin", ios::binary | ios::out);
	//sets memory = 0 for all elements
	memset(memory, 0, BUFFER_SIZE);
	//get size of file and check bracket correctness
	size = fileSize(argv[1], &hasRead);

	if (hasRead) {
		input.open("input.bin", ios::binary | ios::in);
		input.close();
		size2 = fileSize("input.bin", &hasRead);
		if (!input) {
			cout << "File input.bin not found." << endl;
			cout << "Application will terminate.." << endl;
			return 0;
		}
		else {
			input.open("input.bin", ios::binary | ios::in);
			int i = 0;
			inputCode = (unsigned char*)malloc(size2 * sizeof(unsigned char));
			while (!input.eof()) {
				input >> inputCode[i++];
			}
		}
	}
	if (size == -1)
		return 0;
	//load file
	sourceCode = loadFile(argv[1], size);
	hasRead = true;
	for (int i = 0; i < size; i++) {
		switch (sourceCode[i]) {
		case '+': memory[pointer]++; break;
		case '-': memory[pointer]--; break;
		case '<': if (--pointer < 0) pointer += BUFFER_SIZE; break;
		case '>': if (++pointer >= BUFFER_SIZE) pointer -= BUFFER_SIZE; break;
		case 'W': output << memory[pointer]; break;
		case 'R': if (counter < size2) {
					memory[pointer] = inputCode[counter]; 
					counter++;
					}
				  else if (hasRead) {
					  cout << "Input size isnt enough for R requests. Program will use default value 0." << endl;
					  hasRead = false;
				  }
				  break;
		case 'N': memory[pointer] = 0; break;
		case '!': memory[pointer] = 255 - memory[pointer]; break;
		case '[': if (memory[pointer] == 0) 
					i = i + findFightBracket(&sourceCode[i + 1], size - i - 1); 
				  break;
		case ']': if (memory[pointer] != 0) 
					i = findLeftBracket(&sourceCode[0], i) - 1;  
				  break;
		default: break;
		}
	}
	cout << "Application run was successful. Output was saved in file output.bin." << endl << endl;
	return 0;
}
