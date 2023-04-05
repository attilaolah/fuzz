#include "test_util.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

#include <cstdint>
#include <cstring>

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

auto read_span(GifFileType *gif, GifByteType *dst, int size) -> int {
  auto *src = static_cast<std::unique_ptr<std::span<uint8_t>> *>(gif->UserData);
  if (src == nullptr) {
    return 0;
  }

  size = std::min(static_cast<int>((*src)->size()), size);
  std::memcpy(dst, (*src)->data(), size);
  src->reset(new std::span<uint8_t>((*src)->subspan(size)));
  return size;
}
} // namespace gif
