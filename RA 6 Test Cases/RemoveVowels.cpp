#include <iostream>
#include <string>

using namespace std;

string RemoveVowels(string str, int ind){
    if(ind >= str.length()){
        return "";
    }
    if(tolower(str[ind]) == 'a' || tolower(str[ind]) == 'e' || tolower(str[ind]) == 'i' || tolower(str[ind]) == 'o' || tolower(str[ind]) == 'u'){
        return RemoveVowels(str, ind + 1);
    }
    else {
        return str[ind] + RemoveVowels(str, ind + 1);
    }
}
