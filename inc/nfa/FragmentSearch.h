#pragma once

#include "ParseResult.h"

#include <vector>
#include <string_view>

 namespace regex::nfa {

  class Complete;

  /**
   * \brief Executes a search for matches of the NFA fragment in the given text.
   * 
   * This function processes the input text character by character, updating the current state of the NFA
   * and collecting matches as they are found.
   * 
   * \param text The text to search for matches.
   * \param fragment The NFA fragment to use for matching.
   * \return A vector of SearchResult objects representing the matches found in the text.
   */
  std::vector<SearchResult> executeSearch(std::string_view text, Complete* fragment);

}

