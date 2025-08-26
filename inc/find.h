#pragma once

#include "ParseResult.h"
#include "Expression.h"

#include <string_view>
#include <vector>

namespace regex {


  /**
   * \brief Searches for all matches to a regular expression within a given text and returns the results.
   *
   * @param text The text in which to search for the regular expression.
   * @param searchString The regular expression to search for within the text.
   * @returns A vector of SearchResult objects representing each match of the regular expression in the text.
   */
  std::vector<SearchResult> search(std::string_view text, std::string_view searchString);
}
