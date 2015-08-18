#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

void truePredictor(ifstream& traces, ofstream& out);

void singleBitBimodal(string in, ofstream& out);

void twoBitBimodal(string in, ofstream& out);

void Gshare(string in, ofstream& out);

void tournamentPredictor(string in, ofstream& out);

int main(int argc, char** argv){
	ifstream in1(argv[1]);
	string in(argv[1]);
	ofstream out(argv[2]);
	truePredictor(in1, out);
	singleBitBimodal(in, out);
	twoBitBimodal(in, out);
	Gshare(in, out);
	tournamentPredictor(in, out);
	return 0;
}

void truePredictor(ifstream& traces, ofstream& out){
	int numTrue = 0;
	int numFalse = 0;
	int numTotal = 0;
	string currentLine;
	while(getline(traces, currentLine)){
		numTotal++;
		int pos = currentLine.find(' ');
		if(currentLine[pos+1] == 'T'){
			numTrue++;
		}
	}
	numFalse = numTotal - numTrue;
	out << numTrue << ", " << numTotal << ";" << endl << endl;
	out << numFalse << ", " << numTotal << ";" << endl << endl;
}
	
void singleBitBimodal(string in, ofstream& out){
	int numTotal = 0, numCorrect = 0;
	int tableSize;
	string currentLine;
	char* junk;
	for(tableSize = 16; tableSize <= 2048; tableSize *= 2){
		if(tableSize == 64){continue;}
		ifstream traces(in.c_str()); 
		numTotal = 0;
		numCorrect = 0;
		int table[tableSize];
		for(int i=0; i<tableSize; i++){
			table[i] = 1;
		}
		int index, address;
		while(getline(traces, currentLine)){
			numTotal++;
			int pos = currentLine.find(' ');
			string temp = currentLine.substr(2, 8);
			address	= strtol(temp.c_str(), &junk, 16);
			//cout << temp << "   " << address << endl;
			index = address & (tableSize-1);
			int check = (currentLine[pos+1] == 'T') ? 1 : 0;
			if(table[index] == check){
				numCorrect++;
			}
			else{
				table[index] = check;
			}
		}	
		out << numCorrect << ", " << numTotal << "; ";
	}
	out << endl << endl;
}

void twoBitBimodal(string in, ofstream& out){
	int numTotal = 0, numCorrect = 0;
	int tableSize;
	string currentLine;
	char* junk;
	for(tableSize = 16; tableSize <= 2048; tableSize *= 2){
		if(tableSize == 64){continue;}
		ifstream traces(in.c_str()); 
		numTotal = 0;
		numCorrect = 0;
		int table[tableSize];
		for(int i=0; i<tableSize; i++){
			table[i] = 3;
		}
		int index, address;
		while(getline(traces, currentLine)){
			numTotal++;
			int pos = currentLine.find(' ');
			string temp = currentLine.substr(2, 8);
			address	= strtol(temp.c_str(), &junk, 16);
			//cout << temp << "   " << address << endl;
			index = address & (tableSize-1);
			int check = (currentLine[pos+1] == 'T') ? 1 : 0;
			/*if(table[index] == check){
				numCorrect++;
			}
			else{
				table[index] = check;
			}*/
			if(table[index] > 1 && check == 1){
				numCorrect++;
				table[index]++;
			}
			else if(table[index] < 2 && check == 0){
				numCorrect++;
				table[index]--;
			}
			else if(table[index] < 2 && check == 1){
				table[index]++;
			}
			else if(table[index] > 1 && check == 0){
				table[index]--;
			}
			if(table[index] > 3){table[index] = 3;}
			if(table[index] < 0){table[index] = 0;}
		}	
		out << numCorrect << ", " << numTotal << "; ";
	}
	out << endl << endl;
}

void Gshare(string in, ofstream& out){
	string currentLine;
	char* junk = NULL;
	int tableSize = 2048;
	for(int numBits = 3; numBits < 12; numBits++){
		int numTotal=0, numCorrect=0;
		ifstream traces(in.c_str());
		int bits = 0;
		int base = (int)pow(2, numBits);
		//cout << base << endl;
		int table[tableSize];
		for(int i=0; i<tableSize; i++){
			table[i] = 3;
		}
		while(getline(traces, currentLine)){
			numTotal++;
			int pos = currentLine.find(' ');
			string temp = currentLine.substr(2, 8);
			long address	= strtol(temp.c_str(), &junk, 16);
			int index = (address ^ bits) % tableSize;
			int check = (currentLine[pos+1] == 'T') ? 1 : 0;
			if(table[index] > 1 && check == 1){
				numCorrect++;
				table[index]++;
			}
			else if(table[index] < 2 && check == 0){
				numCorrect++;
				table[index]--;
			}
			else if(table[index] < 2 && check == 1){
				table[index]++;
			}
			else if(table[index] > 1 && check == 0){
				table[index]--;
			}
			if(table[index] > 3){table[index] = 3;}
			if(table[index] < 0){table[index] = 0;}
			bits = bits << 1;
			bits = bits % base;
			bits += check;
		}	
		out << numCorrect << ", " << numTotal << "; ";
		traces.close();
	}
	out << endl << endl;
}

void tournamentPredictor(string in, ofstream& out){
	string currentLine;
	bool bRight = false;
	bool gRight = false;
	char* junk = NULL;
	int tableSize = 2048;
	int numBits = 11;
	int numTotal=0, numCorrect=0;
	ifstream traces(in.c_str());
	int bits = 0;
	int base = (int)pow(2, numBits);
	//cout << base << endl;
	int gTable[tableSize];
	int bTable[tableSize];
	int tTable[tableSize];
	for(int i=0; i<tableSize; i++){
		tTable[i] = 0;
		gTable[i] = 3;
		bTable[i] = 3;
	}
	while(getline(traces, currentLine)){
		bRight = false;
		gRight = false;
		numTotal++;
		int pos = currentLine.find(' ');
		string temp = currentLine.substr(2, 8);
		long address	= strtol(temp.c_str(), &junk, 16);
		long indexg = (address ^ bits) % tableSize;
		long index = address % tableSize;
		int check = (currentLine[pos+1] == 'T') ? 1 : 0;
		if(gTable[indexg] > 1 && check == 1){
			//numCorrect++;
			gTable[indexg]++;
			gRight = true;
		}
		else if(gTable[indexg] < 2 && check == 0){
			//numCorrect++;
			gTable[indexg]--;
			gRight = true;
		}
		else if(gTable[index] < 2 && check == 1){
			gTable[indexg]++;
		}
		else if(gTable[indexg] > 1 && check == 0){
			gTable[indexg]--;
		}
		if(gTable[indexg] > 3){gTable[indexg] = 3;}
		if(gTable[indexg] < 0){gTable[indexg] = 0;}
		bits = bits << 1;
		bits = bits % base;
		bits += check;


		if(bTable[index] > 1 && check == 1){
			//numCorrect++;
			bTable[index]++;
			bRight = true;
		}
		else if(bTable[index] < 2 && check == 0){
			//numCorrect++;
			bTable[index]--;
			bRight = true;
		}
		else if(bTable[index] < 2 && check == 1){
			bTable[index]++;
		}
		else if(bTable[index] > 1 && check == 0){
			bTable[index]--;
		}
		if(bTable[index] > 3){bTable[index] = 3;}
		if(bTable[index] < 0){bTable[index] = 0;}


		if(tTable[index] > 1){
			if(bRight){numCorrect++;}
		}
		else if(tTable[index] < 2){
			if(gRight){numCorrect++;}
		}
		if(gRight && (!bRight)){
			tTable[index]--;
		}
		else if((!gRight) && bRight){
			tTable[index]++;
		}
		if(tTable[index] > 3){tTable[index] = 3;}
		if(tTable[index] < 0){tTable[index] = 0;}
	}	
	out << numCorrect << ", " << numTotal << "; ";
	traces.close();
	out << endl << endl;
}
