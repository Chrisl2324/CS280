#include <iostream>
#include <string>
#include <stack>

using namespace std;

int BalancedBrackets(string str) {
    stack<char> stack;
    bool brackets = false;

    for (char ch : str) {
        if (ch == '{' || ch == '[' || ch == '(') {
            stack.push(ch);
            brackets = true;
        } 
        else if (ch == ']') {
            brackets = true;
            if (stack.empty()) {
                cout << "Case 4: There are more closing brackets than opening ones for one or more bracket types in the input string." << endl;
                return -1;  // Case 4
            }
            if (stack.top() != '[') {
                cout << " Case 3: Mismatching bracket type between an opening and a closing bracket." << endl;
                return -1;  // Case 3
            }
            stack.pop();
        } 
        else if (ch == '}') {
            brackets = true;
            if (stack.empty()) {
                cout << "Case 4: There are more closing brackets than opening ones for one or more bracket types in the input string." << endl;
                return -1;  // Case 4
            }
            if (stack.top() != '{') {
                cout << "Case 3: Mismatching bracket type between an opening and a closing bracket." << endl;
                return -1;  // Case 3
            }
            stack.pop();
        } 
        else if (ch == ')') {
            brackets = true;
            if (stack.empty()) {
                cout << "Case 4: There are more closing brackets than opening ones for one or more bracket types in the input string." << endl;
                return -1;  // Case 4
            }
            if (stack.top() != '(') {
                cout << "Case 3: Mismatching bracket type between an opening and a closing bracket." << endl;
                return -1;  // Case 3
            }
            stack.pop();
        }
    }

    if (!stack.empty()) {
        cout << "Case 2: There are more opening brackets than closing ones for one or more bracket types in the input string." << endl;
        return -1;  // Case 2
    }

    if (!brackets) {
        cout << "Case 5: Scanning the input string is completed successfully." << endl;
        return 0;  // Case 5
    }

    cout << "Case 1: Scanning the input string is completed successfully." << endl;
    return 1;  // Case 1
}

// Test the function
