#include "testdata.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <cstdint>

namespace gif {
using std::ios;

auto read_file(const std::string &path) -> std::vector<uint8_t> {
  std::ifstream f(path, ios::binary);
  if (!f) {
    throw std::runtime_error("failed reading file: " + path);
  }

  f.seekg(0, ios::end);
  size_t size = f.tellg();
  f.seekg(0, ios::beg);

  std::vector<uint8_t> data(size);
  f.read(reinterpret_cast<char *>(data.data()), size);

  f.close();
  return data;
}
} // namespace gif
