// ECE 365 DSA2
// Jing Jiang Project 1 spellcheck

#include "hash.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

void loadDict(hashTable &HashTable, ifstream &dictionary);
void spellCheck(hashTable &HashTable, ifstream &input, ostream &output);

int main()
{
  hashTable table(10);
  ifstream dictionary, input;
  ofstream output;

  string dictName;
  cout << "Enter name of dictionary: ";
  cin >> dictName;
  dictionary.open(dictName.c_str());

  loadDict(table, dictionary);

  string inputName, outputName;
  cout << "Enter name of input file: ";
  cin >> inputName;
  input.open(inputName.c_str());

  cout << "Enter name of output file: ";
  cin >> outputName;
  output.open(outputName.c_str());

  spellCheck(table, input, output);

  dictionary.close();
  input.close();
  output.close();

  return 0;
}

// load dictionary
void loadDict(hashTable &HashTable, ifstream &dictionary)
{
  clock_t time1, time2;
  double time;

  string lines;

  time1 = clock();
  while(getline(dictionary, lines)) {
    transform(lines.begin(), lines.end(), lines.begin(), ::tolower);
    HashTable.insert(lines);
  }
  time2 = clock();
  time = (double(time2) - double(time1)) / CLOCKS_PER_SEC;

  cout << "Total time (in seconds) to load dictionary: " << time << endl;
}

// spell check
void spellCheck(hashTable &HashTable, ifstream &input, ostream &output)
{
  enum class outType {inWord, wordSep, longWord};
  outType state = outType::inWord;
  long lineNum = 1;
  char ascii;
  string buffer = "";

  clock_t time1, time2;
  double time;

  time1 = clock();

  while(input.get(ascii)){
    ascii = tolower(ascii);

    switch(state){
      case outType::inWord:
        if((ascii == 39) || (ascii == 45) || ((ascii >= 48) && (ascii <= 57)) || ((ascii >= 97) && (ascii <= 122))){
          if(buffer.length() >= 20){
              output << "Long word at line " << lineNum << ", starts: " << buffer << endl;
              state = outType::longWord;
              buffer = "";
          }else
              buffer.push_back(ascii);
        }else{
          if(!any_of(buffer.begin(), buffer.end(), ::isdigit) && !HashTable.contains(buffer) && (buffer.length() > 0))
              output << "Unknown word at line " << lineNum << ": " << buffer << endl;
          buffer = "";
          state = outType::wordSep;
        }

        break;

      case outType::wordSep:
        if((ascii == 39) || (ascii == 45) || ((ascii >= 48) && (ascii <= 57)) || ((ascii >= 97) && (ascii <= 122))) {
            buffer.push_back(ascii);
            state = outType::inWord;
        }
        break;

      case outType::longWord:
        if(!((ascii == 39) || (ascii == 45) || ((ascii >= 48) && (ascii <= 57)) || ((ascii >= 97) && (ascii <= 122))))
            state = outType::wordSep;
        break;
    }

    if (ascii == '\n')
      lineNum++;
  }
  time2 = clock();

  time = (double(time2) - double(time1)) / CLOCKS_PER_SEC;
  cout << "Total time (in seconds) to check document: " << time << endl;
}
