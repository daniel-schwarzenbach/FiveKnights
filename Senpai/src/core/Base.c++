#include <core/Base.h++>

IS& operator>>(IS& is, Ptr<const char> str) {
   char current;
   uint i = 0;
   bool once = false;
   while (str[i] != '\0') {
      is >> current;
      if (current != str[i] && !once) {
         cerr << "Couldn't read the string:" << str << endl;
         once = true;
      }
      i++;
   }
   return is;
}

// IS& operator>>(IS& is, const char str[]) {
//    char current;
//    uint i = 0;
//    bool once = false;
//    while (str[i] != '\0') {
//       is >> current;
//       if (current != str[i] && !once) {
//          cerr << "Couldn't read the string:" << str << endl;
//          once = true;
//       }
//       i++;
//    }
// }

// IS& operator>>(IS& is, const char* str) {
//    char current;
//    uint i = 0;
//    bool once = false;
//    while (str[i] != '\0') {
//       is >> current;
//       if (current != str[i] && !once) {
//          cerr << "Couldn't read the string:" << str << endl;
//          once = true;
//       }
//       i++;
//    }
// }