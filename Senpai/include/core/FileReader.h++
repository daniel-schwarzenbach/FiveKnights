#include <fstream>
#include <filesystem>

#include "./Base.h++"

template <typename T>
T read_from_file(const String &filename) {
   T toRead{};
   std::ifstream file(filename, std::ios::binary);
   if (!file) {
      std::cerr << "Failed to open file for reading: " << filename << std::endl;
      return toRead;
   }
   
   try {
      // For complex types, implement proper serialization
      file >> toRead;
   } catch (const std::exception& e) {
      std::cerr << "Error reading file " << filename << ": " << e.what() << std::endl;
   }
   
   file.close();
   return toRead;
}

template <typename T>
bool write_to_file(const String &filename, T const &toWrite) {
   // Ensure directory exists
   String directory = filename.substr(0, filename.find_last_of("/\\"));
   if (!directory.empty()) {
      std::filesystem::create_directories(directory);
   }
   
   std::ofstream file(filename, std::ios::binary);
   if (!file) {
      std::cerr << "Failed to open file for writing: " << filename << std::endl;
      return false;
   }
   
   try {
      // For complex types, implement proper serialization
      file << toWrite;
   } catch (const std::exception& e) {
      std::cerr << "Error writing file " << filename << ": " << e.what() << std::endl;
      return false;
   }
   
   file.close();
   return true;
}