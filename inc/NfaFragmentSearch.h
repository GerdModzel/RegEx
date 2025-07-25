#pragma once

#include "ParseResult.h"

#include <vector>
#include <string_view>

 namespace regex {

  class NfaFragment;

  /**
   * \brief Executes a search for matches of the NFA fragment in the given text.
   * 
   * This function processes the input text character by character, updating the current state of the NFA
   * and collecting matches as they are found.
   * 
   * \param text The text to search for matches.
   * \param fragment The NFA fragment to use for matching.
   * \return A vector of ParseResult objects representing the matches found in the text.
   */
  std::vector<ParseResult> executeSearch(std::string_view text, NfaFragment* fragment);

}

