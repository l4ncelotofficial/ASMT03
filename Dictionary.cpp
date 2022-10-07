#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <set>

using namespace std;

vector<string> split (const string &str, char delim);

void toLowercase (string&);
void loadFile (multimap<string, pair<string, string>>&, ifstream&);
void removeExcessPeriod (string&);
int totalKeys (multimap<string, pair<string, string>>);
bool isPartOfSpeech (string);
bool wordExists (multimap<string, pair<string, string>>, string);
bool definitionExists (multimap<string, pair<string, string>>, string, string);
void parseParameters (vector<string>, string&, bool&, bool&, bool&);
string returnOrdinalNum (int);
string returnInstructions (int);
vector<string> partsOfSpeech {"noun", "verb", "pronoun", "adjective", "adverb", "conjunction", "interjection", "preposition"};

int main() {

    string PATH = "C:\\Users\\MickeyMouse\\AbsolutePath\\DB\\Data.CS.SFSU.txt";

    cout << "! Opening data file..." << PATH << endl;
    ifstream file(PATH);
    while (!file.is_open()) {
        cout << "<!>ERROR<!> ===> File could not be opened." << endl;
        cout << "<!>ERROR<!> ===> Provided file path: " << PATH << endl;
        cout << "<!>Enter the CORRECT data file path: ";
        getline(cin, PATH);
        file.open(PATH);
    }

    multimap<string, pair<string, string>> entry;
    cout << "! Loading data..." << endl;
    loadFile(entry, file);
    cout << "! Loading completed..." << endl;
    cout << "! Closing data file... " << PATH << endl;
    file.close();

    cout << endl << "====== DICTIONARY 340 C++ =====" << endl;
    cout << "------ Keywords: " << totalKeys(entry) << endl;
    cout << "------ Definitions: " << entry.size() << endl << endl;

    for (int i = 1; i != 0; i++) {

        cout << "Search [" << i << "]: ";
        string search;
        getline(cin, search);
        toLowercase(search);
        vector<string> v = split(search, ' ');

        if (search == "!q") {
            cout << endl << "-----THANK YOU-----" << endl;
            break;
        }

        cout << "\t   |" << endl;

        bool exists = true;

        if (!search.empty() && (exists = wordExists(entry, v[0])) && v.capacity() >= 2 && isPartOfSpeech(v[1])) {
            exists = definitionExists(entry, v[0], v[1]);
        }

        if (search.empty() || search == "!help" || v.capacity() > 4 || !exists) {
            if (!exists && search != "!help") {
                cout << "\t	   <NOT FOUND> To be considered for the next release. Thank you." << endl;
                cout << "\t   |" << endl;
                cout << "\t   |" << endl;
            }
            cout << "\t    PARAMETER HOW-TO, please enter:" << endl;
            cout << "\t    1. A search key -then 2. An optional part of speech -then" << endl;
            cout << "\t    3. An optional 'distinct' -then 4. An optional 'reverse'" << endl;
            cout << "\t   |" << endl;
            continue;
        }

        string desiredPOS;
        bool isPart = false;
        bool isDistinct = false;
        bool isReversed = false;
        parseParameters(v, desiredPOS, isPart, isDistinct, isReversed);

        vector<string> definitions;

        auto it = entry.equal_range(v[0]);

        for (auto itr = it.first; itr != it.second; ++itr) {
            string key = static_cast<char>(toupper(itr->first[0])) + itr->first.substr(1, itr->first.length());
            string pos = (&(itr->second))->first;
            string def = (&(itr->second))->second;
            string output = "\t    " + key + " [" + pos + "] : " + def;
            if (isPart && pos != desiredPOS) {
                continue;
            }
            definitions.insert(definitions.begin(), output);
        }

        if (isDistinct) {
            set<string> s;
            int size = definitions.size();
            for (int i1 = 0; i1 < size; ++i1) {
                s.insert(definitions[i1]);
            }
            definitions.assign(s.begin(), s.end());
        }

        if (isReversed) {
            sort(definitions.rbegin(), definitions.rend());
        }

        for (auto itr = definitions.begin(); itr != definitions.end(); ++itr) {
            cout << *itr << endl;
        }

        cout << "\t   |" << endl;
    }
}

vector<string> split (const string &str, char delim) {
    string segment;
    stringstream ss(str);
    vector<string> split;
    while (getline(ss, segment, delim)) {
        split.push_back(segment);
    }
    return split;
}

void toLowercase (string &str) {
    for (auto itr = str.begin(); itr != str.end(); ++itr) {
        if (!islower(*itr)) {
            *itr = tolower(*itr);
        }
    }
}

void loadFile (multimap<string, pair<string, string>> &mm, ifstream &file) {
    string line;
    while (getline(file, line)){
        vector<string> v = split(line, '|');

        for (auto itr = v.begin() + 1; itr != v.end(); ++itr) {

            for (int i = 0; i < (*itr).length(); i++) {
                char c = (*itr)[i];
                if (c == ' ') {
                    if (islower((*itr)[i + 6])) {
                        (*itr)[i + 6] = static_cast<char>(toupper((*itr)[i+6]));
                    }
                    break;
                }
            }
            removeExcessPeriod(*itr);
        }

        sort(v.begin() + 1, v.end());
        for (auto itr = v.begin() + 1; itr != v.end(); ++itr) {
            string key = v[0];
            string pos;
            string def;
            for (int i = 0; i < (*itr).length(); i++) {
                char c = (*itr)[i];
                if (c == ' ')  {
                    def = static_cast<char>(toupper((*itr)[i + 6])) + (*itr).substr(i + 7, (*itr).length());
                    mm.insert(pair(key, pair(pos, def)));
                    break;
                } else {
                    pos += c;
                }
            }
        }
        /*
        while (getline(file, line)) {
            vector<string> v1 = split(line, '|');

            for (auto itr = v1.begin(); itr != v1.end(); ++itr) {
                (*itr) = static_cast<char>(toupper((*itr)[0])) + (*itr).substr(1, (*itr).length());
            }

            for (auto itr = v1.begin() + 1; itr != v1.end(); ++itr) {

                for (int i = 0; i < (*itr).length(); i++) {
                    char c = (*itr)[i];
                    if (c == ' ') {
                        if (islower((*itr)[i + 6])) {
                            toupper((*itr)[i + 6]);
                        }
                        break;
                    }
                }

            }


            cout << "check";

        }
*/
    }
}

void removeExcessPeriod (string &str) {
    int first = 0;
    int second = 0;
    int periods = 0;
    for (int i = str.length() - 1; i > 0; i--) {
        char c = str[i];
        if (c == '.') {
            if (periods == 0) {
                first = i;
            } else if (periods == 1) {
                second = i;
            }
            periods++;
        }
    }
    if (periods == 2 && (first - second == 1)) {
        str[first] = ' ';
    }
}

int totalKeys (multimap<string, pair<string, string>> mm) {
    int count = 0;
    string last;
    for (auto itr = mm.begin(); itr != mm.end(); ++itr) {
        if (itr == mm.begin()) {
            count = 1;
        } else if (itr->first != last) {
            count++;
        }
        last = itr->first;
    }
    return count;
}

bool isPartOfSpeech (string str) {
    for (auto itr = partsOfSpeech.begin(); itr != partsOfSpeech.end(); ++itr) {
        if (*itr == str) {
            return true;
        }
    }
    return false;
}

bool wordExists (multimap<string, pair<string, string>> mm, string str) {
    return mm.count(str) > 0;
}

bool definitionExists (multimap<string, pair<string, string>> mm, string key, string pos) {
    auto it = mm.equal_range(key);
    for (auto itr = it.first; itr != it.second; ++itr) {
        if ((&(itr->second))->first == pos) {
            return true;
        }
    }
    return false;
}

void parseParameters (vector<string> v, string &desiredPart, bool &isPart, bool &isDistinct, bool &isReversed) {
    for (int itr = 1; itr < v.size(); itr++) {
        string arg = v[itr];
        bool part = (isPartOfSpeech(arg));
        bool distinct = (arg == "distinct") && (itr < 3);
        bool reversed = (arg == "reverse");
        if (!part && !distinct && !reversed) {
            if (itr == 1) {
                cout << "\t    <The entered " << returnOrdinalNum(itr) << " parameter '" << arg << "' is NOT a part of speech.>" << endl;
            }
            if (itr < 3) {
                cout << "\t    <The entered " << returnOrdinalNum(itr) << " parameter '" << arg << "' is NOT 'distinct'.>" << endl;
            }
            cout << "\t    <The entered " << returnOrdinalNum(itr) << " parameter '" << arg << "' is NOT 'reverse'.>" << endl;
            cout << "\t    <The " << returnOrdinalNum(itr) << " parameter '" << arg << "' was disregarded.>" << endl;
            cout << "\t    " << returnInstructions(itr) << endl;
            cout << "\t   |" << endl;
            cout << "\t   |" << endl;
        }
        if (!isPart && part) {
            isPart = part;
            desiredPart = arg;
        }
        if (!isDistinct && distinct) {
            isDistinct = distinct;
        }
        if (!isReversed && reversed) {
            isReversed = reversed;
        }
    }
}

string returnOrdinalNum (int num) {
    switch (num) {
        case 2:
            return "3rd";
        case 3:
            return "4th";
        default:
            return "2nd";
    }
}

string returnInstructions (int num) {
    switch (num) {
        case 2:
            return "<The 3rd parameter should be 'distinct' or 'reverse'.>";
        case 3:
            return "<The 4th parameter should be 'reverse'.>";
        default:
            return "<The 2nd parameter should be a part of speech or 'distinct' or 'reverse'.>";
    }
}




/*
 *  Loop:
 *      Open data file
 *      if cant open file:
 *          prompt to enter correct file path
 *          change path to entered path
 *          continue loop
 *      else can open file:
 *          end loop
 *
 *  Load data into multimap
 *  Loop:
 *      Go through the data file
 *      get keyword from the text until first occurrence of "|"
 *      get part of speech until " "
 *      loop:
 *          get definition from after " -=>> " until "|"
 *              get another part of speech and definition with same rules. loop until it ends in ".\n"
 *      repeat Loop but with next line for next keyword, repeat until no more keywords to enter.
 *
 *  Close data file
 *
 *  Display counters for keywords and definitions
 *
 *  Loop:
 *      Prompt user to search, use index of loop as counter
 *          layout of search: [word] [part of speech, distinct, reverse] [distinct, reverse] [reverse]
 *      if input is !q:
 *          break;
 *      if input is empty or !help:
 *          display parameter how to
 *          continue;
 *      if input is not a keyword:
 *          display consideration message
 *          continue;
 *      if input is a keyword:
 *          check parameters, if needed, specify part of speech and set booleans for distinct and reverse
 *              display appropriate error messages if incorrectly input
 *          create a vector
 *          if reverse:
 *              use reverse iterator to iterate through multimap
 *          else:
 *              use normal iterator to iterate through multimap
 *          if specific part of speech:
 *              if that part of speech has no definitions:
 *                  display consideration message
 *                  continue;
 *              iterate through multimap and only store the specific ones in the vector
 *          if distinct:
 *              take from vector and place into map?
 *              empty vector?
 *              place into vector
 *
 *          display definitions using vector
 */