//
//  poll.cpp
//  poll
//
//  Created by Anirudh Veeraragavan on 10/22/16.
//  Copyright © 2016 Anirudh Veeraragavan. All rights reserved.
//

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// Function that checks the validity of the entered poll data string
bool hasCorrectSyntax(string pollData);

// Helper functions for the above function
bool hasOnlyDigitsAndAlphaCharacters(string pollData);
bool isValidStateForecast(string pollData);
bool isValidUppercaseStateCode(string stateCode);

// Function that processes the poll data string
int countVotes(string pollData, char party, int& voteCount);

// Helper functions for the above function
int convertVotesFromCharToInt(string pollNumbers);
bool hasNonzeroElectoralVotes(string pollData);


int main()
{
}


// Function that checks the validity of the entered poll data string
bool hasCorrectSyntax(string pollData)
{
    // Ensures all the characters are alphanumeric
    if(!hasOnlyDigitsAndAlphaCharacters(pollData))
        return false;
    
    // Checks validity of each state forecast
    if(!isValidStateForecast(pollData))
        return false;
    
    return true;
}


// Function that ensures all characters are alphanumeric
bool hasOnlyDigitsAndAlphaCharacters(string pollData)
{
    for(size_t i = 0; i != pollData.size(); i++)
    {
        if(!isalpha(pollData[i]) && !isdigit(pollData[i]))
            return false;
    }
    return true;
}


// Function that checks validity of each state forecast
bool isValidStateForecast(string pollData)
{
    for(size_t k = 0; k != pollData.size();)
    {
        // Sets state code in an empty string for testing
        string stateCode = "";
        while(isalpha(pollData[k]))
        {
            stateCode += toupper(pollData[k]);
            k++;
        }
        
        // Checks collected state code against database to ensure validity
        if(!isValidUppercaseStateCode(stateCode))
            return false;
        
        // Checks that none of the electoral votes have more than 2 digits
        string electoralVotes = "";
        while(isdigit(pollData[k]))
        {
            electoralVotes += pollData[k];
            k++;
        }
        if(electoralVotes.size() > 2)
            return false;
        
        // Checks whether a party was indicated after number of electoral votes
        if(!isalpha(pollData[k]))
            return false;
        else
            k++;
    }
    return true;
}


// Function that checks entered state code against database to ensure validity
bool isValidUppercaseStateCode(string stateCode)
{
    const string codes =
    "AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
    "KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC."
    "ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
    return (stateCode.size() == 2  &&
            stateCode.find('.') == string::npos  &&  // no '.' in stateCode
            codes.find(stateCode) != string::npos);  // match found
}


// Function that processes the poll data string
int countVotes(string pollData, char party, int& voteCount)
{

    // Checks if the entered poll data string has correct syntax
    if(!hasCorrectSyntax(pollData))
        return 1;

    // Checks whether the electoral votes are zero for any state forecast
    if(!hasNonzeroElectoralVotes(pollData))
        return 2;
    
    // Checks that the entered party code is valid
    if(!isalpha(party))
        return 3;
    
    // Resets voteCount since everything is valid
    voteCount = 0;
    
    // Runs through the entered poll data string incrementing voteCount if the state forecast matches the indicated party
    for(size_t pos = 0; pos != pollData.size();)
    {
        while(isalpha(pollData[pos]))
            pos++;
        
        // Adds the electoral votes to a string for conversion
        string electoralVotes = "";
        while(isdigit(pollData[pos]))
        {
            electoralVotes += pollData[pos];
            pos++;
        }
        
        // Checks if the party right after the electoral votes is the same as the party indicated, and if it is, adds the electoral votes to voteCount
        if(pollData[pos] == toupper(party) || pollData[pos] == tolower(party))
        {
            voteCount += convertVotesFromCharToInt(electoralVotes);
            pos++;
        }
        else
            pos++;
    }
    return 0;
}


// Function that checks whether the electoral votes for any state forecast is zero
bool hasNonzeroElectoralVotes(string pollData)
{
    for(size_t s = 0; s != pollData.size(); s++)
    {
        while(isalpha(pollData[s]))
            s++;
        // Adds the electoral votes to a string for conversion
        string electoralVotes = "";
        while(isdigit(pollData[s]))
        {
            electoralVotes += pollData[s];
            s++;
        }
        
        // Converts the string to an int and checks whether that int is zero
        int zeroValue = 0;
        if(convertVotesFromCharToInt(electoralVotes) == zeroValue)
            return false;
    }
    return true;
}


// Function that converts electoral votes from a string to an int
int convertVotesFromCharToInt(string pollNumbers)
{
    int multiplyBy = 0;
    int finalValue = 0;
    const int FACTOR = 10;
    for(size_t x = 0; x != pollNumbers.size(); x++)
    {
        int tempValue = pollNumbers[x] - '0';
        multiplyBy = (multiplyBy * FACTOR) + tempValue;
        finalValue = multiplyBy;
    }
    return finalValue;
}
