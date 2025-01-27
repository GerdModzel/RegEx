#include "storageUtils.h"

#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

std::string loadTestText() {
  return "";
}


std::string loadWikiTestFile() {
  const fs::path filePath = fs::current_path().parent_path().parent_path() / RES_PATH / RES_WIKI_REGEX_FILE;

  std::ifstream file{filePath};
  const std::string text{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

  return text;
}
