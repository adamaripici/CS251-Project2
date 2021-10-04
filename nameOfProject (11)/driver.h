// TODO(AdaPici): CS 251 UIC FALL 2021
// Project 2: Personality Quiz, using map,sets and OCEAN Scores C++, functions
void calculation(int y, int num, char first, int second,
                 std::map<char, int>& scores);

#include <math.h>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <utility>
#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
  string questionText;     // Text of the question
  map<char, int> factors;  // Map from factors to +1 or -1
  friend bool operator<(const Question& lhs, const Question& rhs) {
    return lhs.questionText < rhs.questionText;
  }
  friend bool operator==(const Question& lhs, const Question& rhs) {
    return lhs.questionText == rhs.questionText;
  }
  friend bool operator!=(const Question& lhs, const Question& rhs) {
    return lhs.questionText != rhs.questionText;
  }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
  string name;            // Name of the person
  map<char, int> scores;  // Map from factors to +1 or -1
  friend bool operator<(const Person& lhs, const Person& rhs) {
    return lhs.name < rhs.name;
  }
  friend bool operator==(const Person& lhs, const Person& rhs) {
    return lhs.name == rhs.name;
  }
  friend bool operator!=(const Person& lhs, const Person& rhs) {
    return lhs.name != rhs.name;
  }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
 */
Question randomElement(set<Question>& questions) {
  int ind = randomInteger(0, (int)questions.size() - 1);
  int i = 0;
  for (auto e : questions) {
    if (i == ind) {
      return e;
    }
    i++;
  }
  return {};
}

// Choose a random question from the set, remove it from the set, and
// return it.
Question randomQuestionFrom(set<Question>& questions) {
  if (questions.size() == 0) {
    throw runtime_error("ourvector: empty");
  } else {
    int ind = randomInteger(0, (int)questions.size() - 1);
    int i = 0;
    for (auto e : questions) {
      if (i == ind) {
        questions.erase(e);
        return e;
      }
      i++;
    }
  }
  return {};
}

void calculation(int y, int num, char first, int second,
                 std::map<char, int>& scores) {
  if (num == 1) {
    scores[first] = scores[first] + (y * second);
  } else {
    scores[first] = (second)*y;
  }
}

// code that uses the data in that map to determine the user's OCEAN
// scores. Function that takes as input a map containing the user's answers to
// their personality quiz questions, then returns a map representing their OCEAN
// scores. For each question, weight the factors of that question based on the
// user's answer.
map<char, int> scoresFrom(map<Question, int>& answers) {
  map<char, int> scores;
  for (auto& e : answers) {            //
    for (auto& v : e.first.factors) {  // OCEAN factors
      int num = scores.count(v.first);
      char first = v.first;
      int second = v.second;
      int y;
      if (e.second == 5) {
        y = 2;
        calculation(y, num, first, second, scores);
      } else if (e.second == 4) {
        y = 1;
        calculation(y, num, first, second, scores);
      } else if (e.second == 3) {
        y = 0;
        calculation(y, num, first, second, scores);
      } else if (e.second == 2) {
        y = -1;
        calculation(y, num, first, second, scores);
      } else if (e.second == 1) {
        y = -2;
        calculation(y, num, first, second, scores);
      }
    }
  }
  return scores;
}

// normalize the scores
map<char, double> normalize(map<char, int>& scores) {
  map<char, double> normalize;
  if (scores.size() == 0) {
    throw invalid_argument("normalize(scores) throws an exception.");
  }
  double sum = 0;
  double length;
  double square;
  for (auto& e : scores) {
    square = pow(e.second, 2.0);
    sum += square;
  }
  if (sum == 0) {
    throw invalid_argument("normalize(scores) throws an exception.");
  }
  length = sqrt(sum);
  for (auto& v : scores) {
    normalize[v.first] = (double(v.second)) / length;
  }
  return normalize;
}

// calculate similarity of both normalized scores
double cosineSimilarityOf(const map<char, double>& lhs,
                          const map<char, double>& rhs) {
  double correspond;
  double similarity = 0;
  for (const pair<char, double>& e : lhs) {
    for (const pair<char, double>& v : rhs) {
      if (e.first == v.first) {
        correspond = e.second * v.second;
        similarity += correspond;
      }
    }
  }
  return similarity;
}
// Display a map of char-int pairs
void dumpMap(map<char, int> m1) {
  for (auto pair : m1) {
    cout << " " << pair.first << " -> " << pair.second << endl;
  }
}

// Find Best Match for the user and people set
Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {
  // edge case:
  if (people.size() == 0) {
    throw invalid_argument("empty set");
  }
  if (scores.size() == 0) {
    throw invalid_argument("empty set");
  }
  // normalize users score
  map<char, double> resultUser = normalize(scores);

  // normalize each person in the set score and find similarity
  Person similarCharacter;
  double maxValue = -1;

  for (Person p : people) {
    map<char, double> resultPerson = normalize(p.scores);
    double similarity = cosineSimilarityOf(resultUser, resultPerson);
    if (similarity >= maxValue) {
      maxValue = similarity;
      similarCharacter = {p.name, p.scores};
    }
  }
  return similarCharacter;
}
