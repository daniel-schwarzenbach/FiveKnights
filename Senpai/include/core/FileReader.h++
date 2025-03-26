#include "./Base.h++"
#include <fstream>

template <typename T> T read_from_file(const String &filename) {
   T toRead;
   std::ifstream file(filename);
   if (!file) {
      std::cerr << "Failed to open file: " << filename << std::endl;
      return toRead;
   }
   file >> toRead;
   file.close();
   return toRead;
}