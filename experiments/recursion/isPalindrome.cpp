#include <iostream>


bool isPalindrome(std::string str, int start, int end) {
  if(end <= start) {
    return true;
  }

  if(str[start] != str[end]) {
    return false;
  }

  return isPalindrome(str, start+1, end-1);
}


int main() {
  std::string str{""};
  std::cout << "Type in the string to check if it is a palindrome: ";
  std::cin >> str;

  bool res = isPalindrome(str, 0, str.length()-1);
  if(res == 0) {
    std::cout << "String " << str << " IS NOT a palindrome." << std::endl;
  } else {
    std::cout << "String " << str << " IS a palindrome." << std::endl;
  }


  return 0;
}
