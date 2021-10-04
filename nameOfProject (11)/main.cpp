// TODO(AdaPici): CS 251 UIC FALL 2021
// Project 2: Personality Quiz, using map,sets and OCEAN Scores C++
void printFiles();

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "driver.h"
using namespace std;

// go through each line in the questions file
void parseLine(string line, Question &newQuestion) {
  stringstream ss(line);
  getline(ss, newQuestion.questionText, '.');
  while (!ss.eof()) {
    // extract ocean character
    string oceanChar;
    getline(ss, oceanChar, ':');
    // cout << oceanChar ;
    vector<char> ocean;

    string factorNum;

    getline(ss, factorNum, ' ');  // 1

    for (char &c : oceanChar) {
      if (c != ' ') {
        ocean.push_back(c);
        newQuestion.factors[c] = stoi(factorNum);
      }
    }
  }
}

// parse through the person text files
void parseLine2(string line, Person &newPerson) {
  stringstream ss(line);
  getline(ss, newPerson.name, '.');

  while (!ss.eof()) {
    // extract ocean character
    string oceanChar;
    getline(ss, oceanChar, ':');  // E
    // cout << oceanChar ;
    vector<char> ocean;

    string factorNum;

    getline(ss, factorNum, ' ');  // 8

    for (char &c : oceanChar) {
      if (c != ' ') {
        ocean.push_back(c);
        newPerson.scores[c] = stoi(factorNum);
      }
    }
  }
}

// insert data into set question
void loadData(string filename, set<Question> &question) {
  ifstream inFile(filename);
  string line;

  while (!inFile.eof()) {
    getline(inFile, line, '\n');
    if (line.size() == 0) {
      break;
    }
    Question newQuestion;
    parseLine(line, newQuestion);
    question.insert(newQuestion);
  }
}

// insert data into set person
void loadData2(string &filename2, set<Person> &person) {
  person = {};
  ifstream inFile(filename2);
  string line;
  while (!inFile.eof()) {
    getline(inFile, line, '\n');
    if (line.size() == 0) {
      break;
    }
    Person newPerson;
    parseLine2(line, newPerson);
    person.insert(newPerson);
  }
}

void menu(int &testNumber, Person similar, map<char, int> &oceanScores,
          set<Person> &person) {
  string filename2;
  while (testNumber != 0) {
    if (testNumber == 1) {
      filename2 = "BabyAnimals.people";
    } else if (testNumber == 2) {
      filename2 = "Brooklyn99.people";
    } else if (testNumber == 3) {
      filename2 = "Disney.people";
    } else if (testNumber == 4) {
      filename2 = "Hogwarts.people";
    } else if (testNumber == 5) {
      filename2 = "MyersBriggs.people";
    } else if (testNumber == 6) {
      filename2 = "SesameStreet.people";
    } else if (testNumber == 7) {
      filename2 = "StarWars.people";
    } else if (testNumber == 8) {
      filename2 = "Vegetables.people";
    } else if (testNumber == 9) {
      filename2 = "mine.people";
    }
    loadData2(filename2, person);
    similar = mostSimilarTo(oceanScores, person);
    cout << "You got " << similar.name << "!" << endl << endl;
    printFiles();
    cout << "Choose test number (1-9, or 0 to end): ";
    cin >> testNumber;
  }
}
// output the test number menu and return the most similar character
void peopleFile(set<Question> &question, set<Person> &person,
                map<char, int> &oceanScores, Person similar) {
  int testNumber;
  cout << "Choose test number (1-9, or 0 to end): ";
  cin >> testNumber;
  string filename2;
  menu(testNumber, similar, oceanScores, person);
  cout << "Goodbye!" << endl;
  return;
}

void printFiles() {
  cout << "1. BabyAnimals" << endl;
  cout << "2. Brooklyn99" << endl;
  cout << "3. Disney" << endl;
  cout << "4. Hogwarts" << endl;
  cout << "5. MyersBriggs" << endl;
  cout << "6. SesameStreet" << endl;
  cout << "7. StarWars" << endl;
  cout << "8. Vegetables" << endl;
  cout << "9. mine" << endl;
  cout << "0. To end program." << endl << endl;
}

// loop that asks user questions
void askQuestions(int numberOfQuestions, set<Question> &question,
                  map<char, int> &user, map<Question, int> &answers,
                  map<char, int> &oceanScores) {
  for (int i = 0; i < numberOfQuestions; i++) {
    Question statement;
    cout << "How much do you agree with this statement?" << endl;
    statement = randomQuestionFrom(question);  // returns random set
    cout << "\"" << statement.questionText << ".\"" << endl << endl;
    cout << "1. Strongly disagree" << endl;
    cout << "2. Disagree" << endl;
    cout << "3. Neutral" << endl;
    cout << "4. Agree" << endl;
    cout << "5. Strongly agree" << endl << endl;
    cout << "Enter your answer here (1-5): " << endl;
    int answer;
    cin >> answer;
    answers[statement] = answer;
    oceanScores = scoresFrom(answers);
  }
}

int main() {
  map<char, int> user;
  map<Question, int> answers;
  set<Question> question;
  set<Person> person;
  Person similar;
  map<char, int> oceanScores;
  string filename = "questions.txt";
  loadData(filename, question);
  int numberOfQuestions;
  cout << "Welcome to the Personality Quiz!" << endl << endl;
  cout << "Choose number of questions: " << endl;
  cin >> numberOfQuestions;

  askQuestions(numberOfQuestions, question, user, answers, oceanScores);
  printFiles();

  peopleFile(question, person, oceanScores, similar);
  return 0;
}
