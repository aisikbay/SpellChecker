# Spell Checker
Problem: Create a spell checker and word suggestion program using hashing. "Dictionary.txt" is a provided input file of simple words, one per line. The program should ask the user to input a word
* **Case 1:** The program reponds with "true" if the word exists in the dictionary, and suggests words from the dictionary starting with the same first two letters.
* **Case 2:** If the input word does not exist in the dictionary due to user misspelling, assuming the first two letters are correct, the program should return a recommended word from the dictionary. For example, instead of the word sentence, if the user enters setnence, sentens, or setence, etc; the program should return: "do you mean sentence?" If "no", return "false". If "yes", return
"true", and continue as normal, returning words starting with the same first two letters.
* **Case 3:** If the input word does not exist in the dictionary and has no resemblance with an existing word, the program should return "false"<br>

The execution time of each case should be displayed.\
See it run: https://youtu.be/kBcSwrIG3FE
## Example
<img src="https://github.com/aisikbay/SpellChecker/assets/94331548/58b187a8-ae95-458f-9b44-a93f8e3c765f"></img>

## Approach
To store words from "Dictionary.txt", a hash table structure is used. The hash function is based off of the first two letters of an input word, as these are the least likely to be misspelled. The hash function takes careful consideration to not have collisions between flipped cases such as 'AB' and 'BA'.<br>
As there are many words starting with the same first two letters, each bucket of the hash table is a linked list structure, with each node storing the word, its first two letters, and a pointer to the next node (if applicable). The array stores 26*26 buckets to account for all possible two-letter combinations, however nature of the hash function and table make it easier to search for a word.
