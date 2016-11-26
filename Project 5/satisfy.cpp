//
//  satisfy.cpp
//  satisfy
//
//  Created by Anirudh Veeraragavan on 11/7/16.
//  Copyright © 2016 Anirudh Veeraragavan. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstring>
using namespace std;

const int MAX_WORD_LENGTH = 20;
const int MAX_DOCUMENT_LENGTH = 200;

// Returns a set of normalized rules and the number of rules in that normalized set
int normalizeRules(char word1[][MAX_WORD_LENGTH + 1],
                   char word2[][MAX_WORD_LENGTH + 1],
                   int distance[],
                   int nRules);

// Helper functions for the above function
void lowerCaseRules(char word1[][MAX_WORD_LENGTH + 1],
                    char word2[][MAX_WORD_LENGTH + 1],
                    int nRules);
void deleteRule(char word1[][MAX_WORD_LENGTH + 1],
                char word2[][MAX_WORD_LENGTH + 1],
                int distance[],
                int ruleCount,
                int rowPos);
int removeDuplicates(char word1[][MAX_WORD_LENGTH + 1],
                     char word2[][MAX_WORD_LENGTH + 1],
                     int distance[],
                     int ruleCount);

// Returns the satisfaction score of a document based on a set of rules in normal form
int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH + 1],
                          const char word2[][MAX_WORD_LENGTH + 1],
                          const int distance[],
                          int nRules,
                          const char document[]);

// Helper functions for the above function
int createComparisonDoc(const char document[],
                        char comparisonDoc[]);
bool forwardChecker(const char comparisonDoc[],
                    int pos,
                    const char rulesWord[],
                    char rulesWordComparison[],
                    int compDistance);
bool backwardChecker(const char comparisonDoc[],
                     int pos, const char rulesWord[],
                     char rulesWordComparison[],
                     int compDistance);

int main() {
}

// Returns a set of normalized rules and the number of rules in that normalized set
int normalizeRules(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int distance[], int nRules)
{
    int ruleCount = nRules;
    int letterPos1 = 0;
    int letterPos2 = 0;
    
    // Based on the spec, if nRules is non-positive return zero
    if(nRules <= 0)
        return 0;
    
    // Increments through the entire sequence multiple times to catch all errors
    for(int iteration = 0; iteration < nRules; iteration++)
    {
        // Increments through each rule
        for(int rowPos = 0; rowPos < ruleCount; rowPos++)
        {
            // Lower cases all alpha characters for further testing
            lowerCaseRules(word1, word2, nRules);
        
            // Checks if any word in a rule contains no characters
            if(word1[rowPos][0] == '\0' || word2[rowPos][0] == '\0')
            {
                deleteRule(word1, word2, distance, ruleCount, rowPos);
                ruleCount--;
                break;
            }
        
            // Checks if any rule has a nonpositive distance
            if(!(distance[rowPos] > 0))
            {
                deleteRule(word1, word2, distance, ruleCount, rowPos);
                ruleCount--;
                break;
            }
        
            // Checks if any characters within word1 of a rule is non-alpha
            while(word1[rowPos][letterPos1] != '\0' && isalpha(word1[rowPos][letterPos1]))
                letterPos1++;
        
            if(word1[rowPos][letterPos1] != '\0' && !(isalpha(word1[rowPos][letterPos1])))
            {
                deleteRule(word1, word2, distance, ruleCount, rowPos);
                ruleCount--;
                break;
            }
        
            // Checks if any characters within word2 of a rule is non-alpha
            while(word2[rowPos][letterPos2] != '\0' && isalpha(word2[rowPos][letterPos2]))
                letterPos2++;
        
            if(word2[rowPos][letterPos2] != '\0' && !(isalpha(word2[rowPos][letterPos2])))
            {
                deleteRule(word1, word2, distance, ruleCount, rowPos);
                ruleCount--;
                break;
            }
        
            // Resets character positions for next rule
            letterPos1 = 0;
            letterPos2 = 0;
        }
    }
    // Checks if any rules have the same w1 and w1 values and removes the one with a shorter distance
    return removeDuplicates(word1, word2, distance, ruleCount);
}


// Lowercases all alphabetic characters within each rule
void lowerCaseRules(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int nRules)
{
    // Increments through each rule
    for(int rowPos = 0; rowPos < nRules; rowPos++)
    {
        // Lowercases the first word of the rule
        int letterPos1 = 0;
        while(word1[rowPos][letterPos1] != '\0')
        {
            if(isalpha(word1[rowPos][letterPos1]))
                word1[rowPos][letterPos1] = tolower(word1[rowPos][letterPos1]);
            letterPos1++;
        }
        
        // Lowercases the second word of the rule
        int letterPos2 = 0;
        while(word2[rowPos][letterPos2] != '\0')
        {
            if(isalpha(word2[rowPos][letterPos2]))
                word2[rowPos][letterPos2] = tolower(word2[rowPos][letterPos2]);
            letterPos2++;
        }
    }
}


// Deletes any rules that are invalid by moving them to the end of their arrays
void deleteRule(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int distance[], int ruleCount, int rowPos)
{
    // Swaps word1 of the rule to the last position
    for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
    {
        char temp = word1[ruleCount - 1][i];
        word1[ruleCount - 1][i] = word1[rowPos][i];
        word1[rowPos][i] = temp;
    }
    
    // Swaps word2 of the rule to last position
    for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
    {
        char temp = word2[ruleCount - 1][i];
        word2[ruleCount - 1][i] = word2[rowPos][i];
        word2[rowPos][i] = temp;
    }
    
    // Swaps respective distance
    int temp = distance[ruleCount - 1];
    distance[ruleCount - 1] = distance[rowPos];
    distance[rowPos] = temp;
}


// Checks if any rules have the same w1 and w2 values and removes the one with a shorter distance
int removeDuplicates(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int distance[], int ruleCount)
{
    int counter = ruleCount;
    // Increments through the entire sequence multiple times to catch all errors
    for(int iteration = 0; iteration < counter; iteration++)
    {
        for(int rowPos = 0; rowPos < ruleCount; rowPos++)
            for(int rowComparison = rowPos + 1; rowComparison < ruleCount; rowComparison++)
            {
                // Keeps track of swaps to ensure no two values are swapped twice
                bool swapped = false;
                
                // Checks if any two w1 values are the same
                if((strcmp(word1[rowPos], word1[rowComparison]) == 0) && !swapped)
                {
                    // Checks if the same w2 values located at those positions are the same
                    if((strcmp(word2[rowPos], word2[rowComparison]) == 0))
                    {
                        if(distance[rowPos] > distance[rowComparison])
                        {
                            // The following code deletes the rule
                        
                            // Swaps rule in first array to the last position
                            for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
                            {
                                char temp = word1[ruleCount - 1][i];
                                word1[ruleCount - 1][i] = word1[rowComparison][i];
                                word1[rowComparison][i] = temp;
                            }
                        
                            // Swaps rule in second array to last position
                            for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
                            {
                                char temp = word2[ruleCount - 1][i];
                                word2[ruleCount - 1][i] = word2[rowComparison][i];
                                word2[rowComparison][i] = temp;
                            }
                        
                            // Swaps respective distance
                            int temp = distance[ruleCount - 1];
                            distance[ruleCount - 1] = distance[rowComparison];
                            distance[rowComparison] = temp;
                        
                            ruleCount--;
                        
                            swapped = true;
                        }
                        else if(distance[rowPos] < distance[rowComparison])
                        {
                            // The following code deletes the rule
                        
                            // Swaps rule in first array to the last position
                            for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
                            {
                                char temp = word1[ruleCount - 1][i];
                                word1[ruleCount - 1][i] = word1[rowPos][i];
                                word1[rowPos][i] = temp;
                            }
                        
                            // Swaps rule in second array to last position
                            for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
                            {
                                char temp = word2[ruleCount - 1][i];
                                word2[ruleCount - 1][i] = word2[rowPos][i];
                                word2[rowPos][i] = temp;
                            }
                        
                            // Swaps respective distance
                            int temp = distance[ruleCount - 1];
                            distance[ruleCount - 1] = distance[rowPos];
                            distance[rowPos] = temp;
                        
                            ruleCount--;
                        
                            swapped = true;
                        }
                        else
                        {
                            // The following code deletes the rule
                        
                            // Swaps rule in first array to the last position
                            for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
                            {
                                char temp = word1[ruleCount - 1][i];
                                word1[ruleCount - 1][i] = word1[rowComparison][i];
                                word1[rowComparison][i] = temp;
                            }
                        
                            // Swaps rule in second array to last position
                            for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
                            {
                                char temp = word2[ruleCount - 1][i];
                                word2[ruleCount - 1][i] = word2[rowComparison][i];
                                word2[rowComparison][i] = temp;
                            }
                        
                            // Swaps respective distance
                            int temp = distance[ruleCount - 1];
                            distance[ruleCount - 1] = distance[rowComparison];
                            distance[rowComparison] = temp;
                        
                            ruleCount--;
                        
                            swapped = true;
                        }
                    }
                }
            
                // Checks if a w1 and w2 value are the same
                if((strcmp(word1[rowPos], word2[rowComparison])) == 0 && !swapped)
                {
                    // Checks if the other w2 and w1 value at the same position are the same
                    if((strcmp(word2[rowPos], word1[rowComparison])) == 0)
                    {
                        if(distance[rowPos] > distance[rowComparison])
                        {
                            // The following code deletes the rule
                        
                            // Swaps rule in first array to the last position
                            for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
                            {
                                char temp = word1[ruleCount - 1][i];
                                word1[ruleCount - 1][i] = word1[rowComparison][i];
                                word1[rowComparison][i] = temp;
                            }
                        
                            // Swaps rule in second array to last position
                            for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
                            {
                                char temp = word2[ruleCount - 1][i];
                                word2[ruleCount - 1][i] = word2[rowComparison][i];
                                word2[rowComparison][i] = temp;
                            }
                        
                            // Swaps respective distance
                            int temp = distance[ruleCount - 1];
                            distance[ruleCount - 1] = distance[rowComparison];
                            distance[rowComparison] = temp;
                        
                            ruleCount--;
                        
                            swapped = true;
                        }
                        else if(distance[rowPos] < distance[rowComparison])
                        {
                            // The following code deletes the rule
                        
                            // Swaps rule in first array to the last position
                            for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
                            {
                                char temp = word1[ruleCount - 1][i];
                                word1[ruleCount - 1][i] = word1[rowPos][i];
                                word1[rowPos][i] = temp;
                            }
                        
                            // Swaps rule in second array to last position
                            for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
                            {
                                char temp = word2[ruleCount - 1][i];
                                word2[ruleCount - 1][i] = word2[rowPos][i];
                                word2[rowPos][i] = temp;
                            }
                        
                            // Swaps respective distance
                            int temp = distance[ruleCount - 1];
                            distance[ruleCount - 1] = distance[rowPos];
                            distance[rowPos] = temp;
                        
                            ruleCount--;
                        
                            swapped = true;
                        }
                        else
                        {
                            // The following code deletes the rule
                        
                            // Swaps rule in first array to the last position
                            for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
                            {
                                char temp = word1[ruleCount - 1][i];
                                word1[ruleCount - 1][i] = word1[rowComparison][i];
                                word1[rowComparison][i] = temp;
                            }
                        
                            // Swaps rule in second array to last position
                            for(int i = 0; i < MAX_WORD_LENGTH + 1; i++)
                            {
                                char temp = word2[ruleCount - 1][i];
                                word2[ruleCount - 1][i] = word2[rowComparison][i];
                                word2[rowComparison][i] = temp;
                            }
                        
                            // Swaps respective distance
                            int temp = distance[ruleCount - 1];
                            distance[ruleCount - 1] = distance[rowComparison];
                            distance[rowComparison] = temp;
                        
                            ruleCount--;
                        
                            swapped = true;
                        }
                    }
                }
            }
        }
    return ruleCount;
}


// Returns the satisfaction score of a document based on a set of rules in normal form
int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH + 1], const char word2[][MAX_WORD_LENGTH + 1], const int distance[], const int nRules, const char document[])
{
    // Checks nRules and returns 0 if nRules is less than or equal to 0
    if (nRules <= 0)
        return 0;
    
    // Initializes score at 0
    int score = 0;
    
    // Creates a comparison document and sets its size
    char comparisonDoc[MAX_DOCUMENT_LENGTH + 1] = { "" };
    int comparisonDocSize;
    comparisonDocSize = createComparisonDoc(document, comparisonDoc);
    
    // Initializes arrays to compare words from comparison doc to words from rules
    char currentDocWord[MAX_DOCUMENT_LENGTH + 1] = { "" };
    int currentDocWordPos;
    char rulesWord[MAX_WORD_LENGTH + 1] = { "" };
    char rulesWordComparison[MAX_WORD_LENGTH + 1] = { "" };
    int compDistance;
    
    // Increments through each rule, and for each rule runs through the entire document checking if that rule has been satisfied
    for(int rulePos = 0; rulePos < nRules; rulePos++)
    {
        for(int pos = 0; pos < comparisonDocSize;)
        {
            // Resets values for next word
            memset(currentDocWord,'\0', strlen(currentDocWord));
            currentDocWordPos = 0;
            
            memset(rulesWord,'\0', strlen(rulesWord));
            
            memset(rulesWordComparison,'\0', strlen(rulesWordComparison));
            
            compDistance = 0;
            
            // Increments till start of next word
            while(comparisonDoc[pos] == ' ')
                pos++;
            
            // Stores this position for future reference when we check if a rule is satisfied by the previous words of the document
            int backwardsHolder = pos - 1;
            
            // Transfers each word to another array for comparison
            while(isalpha(comparisonDoc[pos]))
            {
                currentDocWord[currentDocWordPos] = comparisonDoc[pos];
                pos++;
                currentDocWordPos++;
            }
            
            // Checks if current word matches either word of current rule, and if so, copies the other word and distance of current rule for future reference
            if(strcmp(word1[rulePos], currentDocWord) == 0)
            {
                compDistance = distance[rulePos];
                strcpy(rulesWord, word2[rulePos]);
            }
            else if(strcmp(word2[rulePos], currentDocWord) == 0)
            {
                compDistance = distance[rulePos];
                strcpy(rulesWord, word1[rulePos]);
            }
            
            // Stores this position, so that after we check if this word abides by the current rule we can reset pos back to its original spot and continue incrementing through the document
            int tempHolder = pos;
            
            // This boolean variable ensures that each rule can only increment score once
            bool passed = false;
            
            // If there was a match, compDistance would have been updated to a positive number
            if(compDistance > 0)
            {
                memset(rulesWordComparison, '\0', strlen(rulesWordComparison));
                
                // Checks the words in front of current word to see if there is a match
                if(forwardChecker(comparisonDoc, pos, rulesWord, rulesWordComparison, compDistance))
                {
                    score++;
                    passed = true;
                }
                
                if(passed == false)
                {
                    pos = backwardsHolder;
                    memset(rulesWordComparison, '\0', strlen(rulesWordComparison));
                    
                    // Checks the words behind of current word to see if there is a match
                    if(backwardChecker(comparisonDoc, pos, rulesWord, rulesWordComparison, compDistance))
                    {
                        score++;
                        passed = true;
                    }
                }
                // Resets pos back to end of current word
                pos = tempHolder;
                
                // If this rule passed, there is no need to continue incrementing through the document and we can move on to the next rule
                if(passed == true)
                    break;
            }
        }
    }
    // Returns score, which would have been incremented for each rule that passed
    return score;
}


// Goes through the document removing all non-alphabetic characters other than spaces, while lowercasing all alphabetic characters
int createComparisonDoc(const char document[], char comparisonDoc[])
{
    int comparisonPos = 0;
    int docPos = 0;
    
    // Increments through document
    while(document[docPos] != '\0')
    {
        if(isalpha(document[docPos]))
        {
            comparisonDoc[comparisonPos] = tolower(document[docPos]);
            comparisonPos++;
        }
        else if(document[docPos] == ' ')
        {
            comparisonDoc[comparisonPos] = document[docPos];
            comparisonPos++;
        }
        docPos++;
    }
    
    // Assigns the \0 char to the comparison document
    comparisonDoc[comparisonPos] = document[docPos];
    
    // This returns the size of the comparison document
    return comparisonPos;
}


// Checks the words in front of current word to see if there is a match
bool forwardChecker(const char comparisonDoc[], int pos, const char rulesWord[], char rulesWordComparison[], int compDistance)
{
    int wordCount = 0;
    bool passed = false;
    while(wordCount < compDistance)
    {
        int rulesWordComparisonPos = 0;
        
        // Increments till start of next word
        while(comparisonDoc[pos] == ' ')
            pos++;
        
        // Sets word into a comparison array
        while(isalpha(comparisonDoc[pos]))
        {
            rulesWordComparison[rulesWordComparisonPos] = comparisonDoc[pos];
            rulesWordComparisonPos++;
            pos++;
        }
        
        // Compares collected word against word in rule
        if(strcmp(rulesWord, rulesWordComparison) == 0)
        {
            passed = true;
            break;
        }
        memset(rulesWordComparison, '\0', strlen(rulesWordComparison));
        wordCount++;
    }
    
    // Returns either true or false based on whether a match was found
    if(passed == true)
        return true;
    else
        return false;
}


// Checks the words behind of current word to see if there is a match
bool backwardChecker(const char comparisonDoc[], int pos, const char rulesWord[], char rulesWordComparison[], int compDistance)
{
    int wordCount = 0;
    bool passed = false;
    while(wordCount < compDistance && pos >= 0)
    {
        int rulesWordComparisonPos = 0;
        
        // Decrements till start of next word
        while(pos >= 0)
        {
            if(comparisonDoc[pos] == ' ')
                pos--;
            else break;
        }
        
        // Sets word into a comparison array
        while(pos >= 0)
        {
            if(isalpha(comparisonDoc[pos]))
            {
                rulesWordComparison[rulesWordComparisonPos] = comparisonDoc[pos];
                rulesWordComparisonPos++;
                pos--;
            }
            else
                break;
        }
        
        // Since the previous word was collected in reverse order, this code will switch it back to proper order
        char tempChar;
        long midPoint = strlen(rulesWordComparison) / 2;
        for(long i = 0; i < midPoint; i++)
        {
            tempChar = rulesWordComparison[i];
            rulesWordComparison[i] = rulesWordComparison[strlen(rulesWordComparison) - (i + 1)];
            rulesWordComparison[strlen(rulesWordComparison) - (i + 1)] = tempChar;
        }
        
        // Compares collected word against word in rule
        if(strcmp(rulesWord, rulesWordComparison) == 0)
        {
            passed = true;
            break;
        }
        memset(rulesWordComparison, '\0', strlen(rulesWordComparison));
        wordCount++;
    }
    
    // Returns either true or false based on whether a match was found
    if(passed == true)
        return true;
    else
        return false;
}
