/*==========================================
Christopher Lombardi
RA2
CS280
09/12/24
============================================*/


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

int main() {
    string filename;
    cout << "Enter the name of a file to read from: " << endl;
    cin >> filename;
    cout << endl;

    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "File cannot be opened " << filename << endl;
        return 1;
    }

    string line;
    int numLines = 0, numWords = 0, numBold = 0, numUnderline = 0, numItalic = 0;
    int numParagraphs = 0, numComments = 0;
    bool inParagraph = false;

    while (getline(file, line)) {
        numLines++;

        if (line.find("#") == 0) {
            numComments++;
            continue;
        }

        istringstream iss(line);
        string word;
        bool firstWord = true;

        while (iss >> word) {
            if (firstWord && word[0] == '%') {
                firstWord = false;
                string command = word.substr(1);
                if (command == "bold") {
                    numBold++;
                    continue;
                } else if (command == "italic") {
                    numItalic++;
                    continue;
                } else if (command == "underline") {
                    numUnderline++;
                    continue;
                } else if (command == "begin") {
                    if (inParagraph) {
                        cout << "Missing end command for defining a paragraph after paragraph " << numParagraphs << endl << endl;
                    } else {
                        inParagraph = true;
                    }
                    continue;
                } else if (command == "end") {
                    if (!inParagraph) {
                        cout << "Missing begin command for defining a paragraph after paragraph " << numParagraphs << endl << endl;
                    } else {
                        inParagraph = false;
                        numParagraphs++;
                    }
                    continue;
                } else {
                    cout << "Incorrect command name at line: " << numLines << endl << endl;
                    continue;
                }
            } else if (word[0] == '%' && !firstWord) {
                string command = word.substr(1);
                if (command == "bold") {
                    numWords++;
                } else if (command == "italic") {
                    numWords++;
                } else if (command == "underline") {
                    numWords++;
                }
            } else {
                numWords++;
            }

            firstWord = false;
        }
    }

    if (inParagraph) {
        cout << "Missing end command for defining a paragraph after paragraph " << numParagraphs << endl << endl;
    }

    cout << "Total lines: " << numLines << endl;
    cout << "Commented lines: " << numComments << endl;
    cout << "Number of Paragraphs: " << numParagraphs << endl;
    cout << "Total number of words: " << numWords << endl;
    cout << "Bold commands: " << numBold << endl;
    cout << "Italic commands: " << numItalic << endl;
    cout << "Underline commands: " << numUnderline << endl;

    file.close();

    return 0;
}
