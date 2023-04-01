#include <iostream>

#include "bigint.h"
#include "SSA.h"

using std::cout;
using std::endl;

using namespace FT42;

int main()
{
    std::string NewMessage, ReceivedMessage;
    User* MyPtr = nullptr;
    int iShareCount, GivingShare, MaxShare;
    std::vector<User*> UserVect;
    std::vector<std::vector<std::pair<int, iamtcs::BigInt>>> Group_Share;

    std::cout << "Enter Message to Share - \t";
    std::cin >> NewMessage;

    Server sObj(NewMessage);

    std::cout << "How many shares you want to create - \t";
    std::cin >> iShareCount;

    MaxShare = sObj.getMaximum_Shares();

    if (MaxShare < iShareCount)
    {
        std::cout << "We cannot create " << iShareCount << " shares..." << std::endl;
        std::cout << "Exiting...";
        exit(1);
    }

    for (int iCounter = 0; iCounter < iShareCount; ++iCounter)
    {
        MyPtr = new User();

        if (!MyPtr)
        {
            std::cout << "Memory Creation Failed...Exiting";
            exit(1);
        }

        UserVect.push_back(MyPtr);
        UserVect[iCounter]->Get_Share(&sObj);
    }

    std::cout << "\nFollowing shares has been created." << std::endl;

    for (int iCounter = 0; iCounter < iShareCount; ++iCounter)
    {
        UserVect[iCounter]->Print_Share(iCounter);
    }
    
    std::cout << "How many shares you want to give for reconstruction -\t";
    std::cin >> GivingShare;

    if (iShareCount < GivingShare)
    {
        std::cout << "You dont have " << GivingShare << " shares to give" << std::endl;
        std::cout << "Exiting...";
        exit(1);
    }

    for (int iCounter = 0; iCounter < GivingShare; ++iCounter)
    {
        Group_Share.push_back(UserVect[iCounter]->Share);
    }

    ReceivedMessage = sObj.Create_Plain_Message(Group_Share);

    std::cout << "Reconstructed Message is = " << ReceivedMessage;

    return 0;
}