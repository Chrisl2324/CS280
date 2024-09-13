#include <iostream>
#include <string>

using namespace std;

int main(){
	string firstName;
	string lastName;
	int section;
	cout << "Please enter your first name " << endl;
	getline(cin, firstName);
	cout << "Please enter your last name " << endl;
	getline(cin, lastName);
	cout << "Please enter your CS280 section number " << endl;
	cin >> section;
	cout << "Welcome " + firstName + " to CS 280 section " << section << "."<< endl;
	return 0;
}
