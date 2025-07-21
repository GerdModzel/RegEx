This is a regular expression library.

I am going through the algorithms presented on Russ Cox' blog (https://swtch.com/~rsc/regexp/regexp1.html) and implement them as practice and to finally remember the syntax long-term.

Currently the NFA algorithm is implemented with the most common operations: ().*?+

Building should work with cmake without special configuration.

The simplest usage is via the find() function in libRegEx/find.h. Internally, the functions does this:
```
    ExpressionBuilder builder(&buildExpressionArgumentsFirstOperatorLast);
    Expression expr = builder.build(searchString);

    NfaFragment nfa = NfaBuilder::createNfaFragment(expr);
    std::vector<ParseResult> results;

    return executeSearch(text, &nfa);
```
The `ExpressionBuilder` converts the search string into an `Expression` using the function supplied to the constructor. `NfaBuilder` creates a non-deterministic finite automaton (NFA) that is then traversed by `executeSearch` to find matching strings in the text.


folder structure:
- inc: header files
- libRegEx: the library's source files
- RegExCmd: a simple Windows-only applications for manual testing
- res: Files I use for testing
- test: tests using googletest
