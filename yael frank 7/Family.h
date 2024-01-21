#pragma once
#include <cstring> 

class Family
{

public:
    int familyNumber;
    char familyName[20];
    int numOfPeople;
    long phoneNumber;
    bool activeClabs[6];


    Family() {
        familyNumber = 0;
        strcpy_s(familyName, "                   ");        
        numOfPeople = 0;
        phoneNumber = 0;
        for (int i = 0; i < sizeof(activeClabs); i++)
        {
            activeClabs[i] = false;
        }
    }
};

