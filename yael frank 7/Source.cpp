#include <fstream>
#include "Family.h"
#include <iostream>
#include <cstring> 
#include <queue>
using namespace std;

void setFile(std::fstream& file) {
    // Creating new Family objects and initializing the array of pointers
    Family f;
    for (int i = 0; i < 100; ++i) {
        file.write((char*)&f, sizeof(Family));
    }
}

void add(std::fstream& file) {
    int familyNumber;
    cout << "Enter family number: ";
    cin >> familyNumber;
    if (familyNumber < 1 || familyNumber > 100) {
        throw("Error: Invalid family number");
        return;
    }

    file.seekg((familyNumber - 1) * sizeof(Family));
    Family existingFamily;
    file.read((char*)&existingFamily, sizeof(Family));

    if (existingFamily.familyNumber != 0) {
        throw "ERROR: Family already exists in the file";
    }

    Family f;
    f.familyNumber = familyNumber;
    cout << "Enter family name: ";
    cin >> f.familyName;
    cout << "Enter count of people in the family: ";
    cin >> f.numOfPeople;
    cout << "Enter family phone: ";
    cin >> f.phoneNumber;

    file.seekp((familyNumber - 1) * sizeof(Family));
    file.write((char*)&f, sizeof(Family));
}

void del(std::fstream& file, int index) {
    if (index < 1 || index > 100)
    {
        throw("Erorr :Invalid family number");
        return;
    }
    Family emptyFamily;

    file.seekp((index - 1) * sizeof(Family));
    file.write((char*)&emptyFamily, sizeof(Family));

}
void update(std::fstream& file, int index, std::queue<Family>& waitingQueue) {
    if (index < 1 || index > 100) {
        throw "ERROR: Invalid family number";
    }
    file.seekp((index - 1) * sizeof(Family));

    Family existingFamily;
    file.read((char*)&existingFamily, sizeof(Family));
    if (existingFamily.familyNumber == 0) {
        throw "ERROR: Family is not in the file";
    }

    bool clubs[6];

    for (int i = 0; i < 6; i++) {
        cout << "Enter true/false (1/0) for club number:  " << i + 1 << endl;
        cin >> clubs[i];
    }
    bool flug = false;
    for (int i = 0; i < 6; i++) {
        int countOnClub = 0;
        if (clubs[i]) {
            file.seekg(0, std::ios::beg); // Reset file position to the beginning
            Family f;
            for (int j = 0; j < 100; j++) {
                file.read((char*)&f, sizeof(Family));
                if (f.familyNumber != 0) {
                    if (f.activeClabs[i] == true)
                        countOnClub++;
                }
            }
            if (countOnClub < 10) {
                existingFamily.activeClabs[i] = true;
                clubs[i] = false;
            }
            else {
                
                cout << "there is more then 10 to club number: "<<i<<endl;
                existingFamily.activeClabs[i] = false;
                flug = true;
            }
        }
    }
    
    file.seekp((index - 1) * sizeof(Family));
    file.write((char*)&existingFamily, sizeof(Family));
    if (flug)//ישנו חוג שהמשפחה לא הצליחה להכנס אליו...
    {
        for (int i = 0; i < 6; i++)
        {
            existingFamily.activeClabs[i] = clubs[i];
        }
        waitingQueue.push(existingFamily);
    }
}

void waiting(std::fstream& file, std::queue<Family>& waitingQueue)
{
    queue<Family> newWaitingQueue;
    Family f;
    bool flug = false;
    while (!waitingQueue.empty())
    {
        Family waitingFamily = waitingQueue.front();
        waitingQueue.pop();    
        file.seekp((waitingFamily.familyNumber - 1) * sizeof(Family));
        Family existingFamily;
        file.read((char*)&existingFamily, sizeof(Family));
        for (int i = 0; i < 6; i++)
        {
            int countOnClub = 0;
            if (waitingFamily.activeClabs[i])
            {
                file.seekg(0, std::ios::beg); // Reset file position to the beginning
                Family f;
                for (int j = 0; j < 100; j++) {
                    file.read((char*)&f, sizeof(Family));
                    if (f.familyNumber != 0) {
                        if (f.activeClabs[i] == true)
                            countOnClub++;
                    }
                }
                if (countOnClub < 10) {
                    cout << "family: " << existingFamily.familyName << endl;
                    cout << "Do you want to enter the class number " <<i<<" Y/N " <<endl;
                    char c;
                    cin >> c;
                    if (c == 'y' || c == 'Y')

                    {
                        existingFamily.activeClabs[i] = true;
                        waitingFamily.activeClabs[i] = false;
                    }
                    else {
                        existingFamily.activeClabs[i] = false;
                        flug = true;
                    }
                }
                else {
                    //צריך להשאר בתור עדיין כי יש חוג שעדיין לא מצליח להכנס אליו
                    existingFamily.activeClabs[i] = false;
                    flug = true;
                }
            }
        }

        file.seekp((existingFamily.familyNumber - 1) * sizeof(Family));
        file.write((char*)&existingFamily, sizeof(Family));
        if (flug)
            newWaitingQueue.push(waitingFamily);
    }
    waitingQueue = newWaitingQueue;
}
bool rishum(std::fstream& file, int index, int club) {
    if (index < 1 || index > 100) {
        throw "ERROR: Invalid family number";
    }
    if (club < 1|| club > 6) { // Changed the condition to check if club is between 0 and 5
        throw "ERROR: Invalid class number";
    }
    file.seekg((index - 1) * sizeof(Family));

    Family existingFamily;
    file.read((char*)&existingFamily, sizeof(Family));
    if (existingFamily.familyNumber == 0) {
        throw "ERROR: Family is not in the file";
    }
    return existingFamily.activeClabs[club];
}
void print(std::fstream& file, int index) {
    if (index < 1 || index > 100) {
        throw "ERROR: Invalid family number";
    }
    file.seekg((index - 1) * sizeof(Family));
    Family existingFamily;
    file.read((char*)&existingFamily, sizeof(Family));
    if (existingFamily.familyNumber == 0) {
        throw "ERROR: Family is not in the file";
    }
    cout << "Family: " << existingFamily.familyName << endl;
    cout << "Number of people: " << existingFamily.numOfPeople << endl;
    cout << "Family phone number: " << existingFamily.phoneNumber << endl;
    for (int i = 0; i < 6; i++) {
        if (existingFamily.activeClabs[i]) {
            cout << "Class number: " << i + 1 << " Y" << endl; // Added +1 to display class number correctly
        }
        else {
            cout << "Class number: " << i + 1 << " N" << endl; // Added +1 to display class number correctly
        }
    }
}
void inClass(std::fstream& file, int c) {
    if (c < 0 || c > 5) { // Changed the condition to check if c is between 0 and 5
        throw "ERROR: Invalid class number";
    }
    file.seekg(0, std::ios::beg); // Reset file position to the beginning
    Family f;
    for (int j = 0; j < 100; j++) {
        file.read((char*)&f, sizeof(Family));
        if (f.familyNumber != 0) {
            if (f.activeClabs[c]) {
                cout << "Family: " << f.familyName << endl;
            }
        }
    }
}
int main()
{
    queue<Family> q;
    fstream file;
    file.open("families.txt", ios::binary | ios::in | ios::out);
    if (!file)
    {
        cout << "ERROR: couldn't open file\n";
        return 0;
    }
    setFile(file);
    int choice;
    int snum;
    int cnum;
    cout << "Choices are:\n0 to exit\n1 to add a family\n2 to delete a family\n3 to update rishum to classes\n4 to update waiting to classes \n5 to check rishum for a classas\n6 to print a family\n7 to print all the families that participate in a specific class\n";
    cout << "enter 0-7:\n";
    cin >> choice;
    while (choice)
    {
        switch (choice)
        {
        case 1://add to the file
            try { add(file); }
            catch (const char* msg) { cout << msg; }
            break;
        case 2://delete from file
            cout << "enter number of family to delete:\n";
            cin >> snum;
            try { del(file, snum); }
            catch (const char* msg) { cout << msg; }
            break;
        case 3://update the list of classes of a family
            cout << "enter number of family to update:\n";
            cin >> snum;
            try { update(file, snum,q); }
            catch (const char* msg) { cout << msg; 
            
            }
            break;
        case 4://update the list of classes of a waiting family
            waiting(file, q);
            break;
        case 5://check rishum to a specific class
            cout << "enter number of family to check rishum:\n";
            cin >> snum;
            cout << "enter number of class to check rishum:\n";
            cin >> cnum;
            try
            {
                cout << "The family is" << (rishum(file, snum, cnum) ? " " : " not ") << "taking the class\n";
            }
            catch (const char* msg) { cout << msg; }
            break;
        case 6://print the details of a specific family
            cout << "enter number of family to print:\n";
            cin >> snum;
            try { print(file, snum); }
            catch (const char* msg) { cout << msg; }
            break;
        case 7://print the details of all the families that are taking a specific class
            cout << "enter number of class to check rishum:\n";
            cin >> cnum;
            try { inClass(file, cnum); }
            catch (const char* msg) { cout << msg; }
            break;
        default:
            cout << "ERROR: invalid choice\n";

        }
        cout << "\nenter 0-7:\n";
        cin >> choice;
    }
    file.close();
    return 0;

}