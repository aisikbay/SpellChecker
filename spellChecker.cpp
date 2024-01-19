//ARAM ISIKBAY

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cmath>

using Clock=std::chrono::high_resolution_clock;

#define TOTALSIZE 26*26

class Hash {
private:
    struct word {
        std::string letter1;
        std::string letter2;
        std::string fullWord;
        word* next = NULL;
    };
    word* Table[TOTALSIZE];
public:
    Hash();
    int hashFunc(std::string input);
    void insert(std::string input);
    void clearTable();
    void readDict();
    std::string suggest(std::string misspelledInput);
    bool search(std::string input);
    void printList(std::string input);
    void menu();
};

//Hash table constructor
Hash::Hash() {
    for (int i = 0; i < TOTALSIZE; i++) {
        Table[i] = new word;
        Table[i]->fullWord = "NULL";
        Table[i]->letter1 = "NULL";
        Table[i]->letter2 = "NULL";
        Table[i]->next = NULL;
    }
}

//Hash function
int Hash::hashFunc(std::string input) {
    int intL1= (int)input[0];
    int intL2 = (int)input[1];
    //Avoids collisions between AB or BA
    int sum = intL1 * pow(11, 2) + intL2;
    int result = (sum) % (TOTALSIZE);
    return result;
}

//Insert a hashed value into the table
void Hash::insert(std::string input) {
    //Compute hash value from hash function
    int index = hashFunc(input);
    //If index is available, take it
    if (Table[index]->fullWord == "NULL") {
        Table[index]->fullWord = input;
        Table[index]->letter1 = input[0];
        Table[index]->letter2 = input[1];
    }
    //If not, make a new word for the term to add it to the end of the linked list
    else {
        word* newWord = new word;
        newWord->fullWord = input;
        newWord->letter1 = input[0];
        newWord->letter2 = input[1];
        newWord->next = NULL;
        //Point to beginning of list, move to the end for an open spot
        word* wordPtr = Table[index];
        while (wordPtr->next != NULL) {
            wordPtr = wordPtr->next;
        }
        //Put the word at the end of the linked list
        wordPtr->next = newWord;
    }
}

//Delete the hash table of linked lists to avoid memory leaks
void Hash::clearTable() {
    //Delete the linked list at each index of the table
    for (int i = 0; i < TOTALSIZE; i++) {
        word* next;
        word* current = Table[i];
        while (current != NULL) {
            next = current->next;
            delete current;
            current = next;
        }
    }
}

//Input the contents of "Dictionary.txt" to the hash table
void Hash::readDict() {
    std::ifstream inFile;
    inFile.open("Dictionary.txt");
    if (inFile.fail()) {
        std::cout << "Unable to open Dictionary.txt" << std::endl;
        exit(1);
    }
    std::string input;
    while (!inFile.eof()) {
        std::getline(inFile, input);
        insert(input);
    }
    inFile.close();
}

std::string Hash::suggest(std::string misspelledInput) {
    word* suggestion = NULL;
    int maxCount = 0;
    for (int i = 0; i < TOTALSIZE; i++) {
        //Only check words with the same first letter
        if (toupper(Table[i]->fullWord[0]) == toupper(misspelledInput[0])) {
            //Point to the beginning of linked list and determine which word is most similar
            word* wordPtr = Table[i];
            while (wordPtr != NULL) {
                int tempCount = 0;
                //Avoid repeat checks
                std::string charList = "";
                std::string ptrString = wordPtr->fullWord;
                //If strings differ in length, subtract length from tempCount
                int lengthDiff = ptrString.length() - misspelledInput.length();
                if (lengthDiff > 0) {
                    tempCount -= (lengthDiff);
                }
                else if (lengthDiff < 0){
                    tempCount += lengthDiff;
                }
                for (int i = 0; i < ptrString.length(); i++) {
                    //If the misspelled input contains the char in the pointer string, and the char hasn't already been accounted for
                    if (misspelledInput.find(tolower(ptrString[i])) != std::string::npos && charList.find(ptrString[i] == std::string::npos)) {
                        //Increase the count and append the character
                        tempCount++;
                        char* ch = &ptrString[i];
                        charList.append(ch);
                        //Check is max similarity has changed, and update if need be
                        if (tempCount > maxCount) {
                            maxCount = tempCount;
                            suggestion = wordPtr;
                        }
                    }
                }
                wordPtr = wordPtr->next;
            }
        }
    }
    return suggestion->fullWord;
}

//Search function - returns true or false
bool Hash::search(std::string input) {
    int index = hashFunc(input);
    //If the words indexs' linked list is empty, the word cannot exist
    if (Table[index]->fullWord == "NULL") {
        return false;
    }
    //If word is at beginning of its indexs' linked list
    else if (Table[index]->fullWord == input) {
        return true;
    }
    //Otherwise, search the linked list for the word
    else{
        word* wordPtr = Table[index];
        while (wordPtr->next != NULL) {
            wordPtr = wordPtr->next;
            if (wordPtr->fullWord == input) {
                return true;
            }
        }
    }
    //Word does not exist
    return false;
}

void Hash::printList(std::string input) {
    int index = hashFunc(input);
    word* wordPtr = Table[index];
    std::cout << wordPtr->fullWord << ", ";
    while (wordPtr->next != NULL) {
        wordPtr = wordPtr->next;
        std::cout << wordPtr->fullWord << ", ";
    }
    std::cout << std::endl;
}

void Hash::menu() {
    bool keepGoing = true;
    while (keepGoing) {
        std::string userInput;
        std::cin.clear();
        std::cout << "Enter a search term: ";
        //Check if user is trying to exit
        if (std::cin.peek() != '\n') {
            std::getline(std::cin, userInput);;
            //Start timer of search
            auto time1 = Clock::now();
            //When found, end timer and print similar words
            if (search(userInput)) {
                //End timer, calculate time
                auto time2 = Clock::now();
                auto timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count();
                std::cout << "TRUE (" << timeElapsed << " microseconds)" << std::endl;
                printList(userInput);
            }
            //If word isn't found, find a similar word (suggestion)
            else if (userInput.length() >= 2){
                std::string suggestion = suggest(userInput);
                //End timer, calculate time
                auto time2 = Clock::now();
                auto timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count();
                //If a similar word is found, ask the user if they meant that word
                if (suggestion != "NULL") {
                    std::cout << "Did you mean " << suggestion << " (" << timeElapsed << " microseconds) ? Y/N: ";
                    std::string ynInput;
                    std::getline(std::cin, ynInput);
                    //If yes, print similar words
                    if (toupper(ynInput[0]) == 'Y') {
                        std::cout << "TRUE" << std::endl;
                        printList(suggestion);
                    }
                    else if (ynInput == "") {}
                    else {
                        std::cout << "FALSE" << std::endl;
                    }
                }
            }
            //If a similar word can't be found, or the user entered a single character that wasn't found
            else {
                auto time2 = Clock::now();
                auto timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count();
                std::cout << "FALSE (" << timeElapsed << " microseconds)" << std::endl;
            }
        }
        //If user is trying to exit
        else {
            std::cout << "Goodbye now! :)" << std::endl;
            keepGoing = false;
        }
    }
}

int main() {
    std::cout << "Welcome to Dictionary! Press ENTER to exit" << std::endl;
    Hash hashTable;
    //Read dictionary.txt
    hashTable.readDict();
    //Start user menu
    hashTable.menu();
    //Delete hash table when user is done
    hashTable.clearTable();
    return 0;
}