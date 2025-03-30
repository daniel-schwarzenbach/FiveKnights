#include <fstream>

#include "./Base.h++"

template <typename T>
T read_from_file(const String &filename) {
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

template <typename T>
bool write_to_file(const String &filename, T const &toWrite) {
   std::ofstream file(filename, std::ios::out);
   if (!file) {
      std::cerr << "Failed to open file: " << filename << std::endl;
      return false;
   }
   file << toWrite;
   file.close();
   return true;
}