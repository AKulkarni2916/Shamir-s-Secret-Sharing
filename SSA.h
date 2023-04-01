#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <utility>

#include "bigint.h"

namespace FT42
{
	class User;

	class Server
	{
		std::string Secret_Messsage;
		std::vector <iamtcs::BigInt> Bigint_Message;	//Initially NULL, modified by String_to_Int

		iamtcs::BigInt Maximum_Shares;			//N
		iamtcs::BigInt Minimum_Shares;			//K
		iamtcs::BigInt X;						//X
		iamtcs::BigInt Y;						//Y

		static int Share_Count;					//From 1 to Maximum_Shares

		void String_to_Int();					//Done

		//How many maximum shares for each letter
		void Calculate_Maximum_Shares();		//Done

		//How many minimum shares for each letter
		void Calculate_Minimum_Shares();		//Done

		Server(Server& Obj)
		{
			/*
			private copy constructor to avoid use of copy constructor
			User should use given constructor only
			*/
		}

	public:

		Server(std::string Message)
		{
			int TempX, TempY;

			Secret_Messsage = Message;

			String_to_Int();

			Calculate_Maximum_Shares();
			Calculate_Minimum_Shares();

			do
			{
				TempX = rand() % 100;
				TempY = rand() % 100;
			} while (!TempX || !TempY);

			X.assign(TempX);
			Y.assign(TempY);
		}

		int getShare_Count();
		int getMaximum_Shares();
		/*
		One Pair means One share of One Letter
		eg - One Pair means One share of letter H from message "HELLO"
		vector of Pairs means One Share of each letter
		eg - vector of pairs means One share of each letter from message
		"HELLO" ie one share of 'H','E','L','L','O'
		*/

		std::vector<std::pair<int, iamtcs::BigInt>> Create_Share() const;	//Create One list as one share
		
		/*
		Similarly here
		vector of vector of pairs means 
		multiple shares of each letter from message "HELLO"
		*/

		std::string Create_Plain_Message(std::vector<std::vector<std::pair<int, iamtcs::BigInt>>>);
		
		void Print(iamtcs::BigInt, std::string);	//Temp Basis
	};


	class User
	{
		User(const User&) = delete;

	public:

		/*
		One Pair means One share of One Letter
		eg - One Pair means One share of letter H from message "HELLO"
		vector of Pairs means One Share of each letter
		eg - vector of pairs means One share of each letter from message
		"HELLO" ie one share of 'H','E','L','L','O'
		*/

		std::vector<std::pair<int, iamtcs::BigInt>> Share;		//Current Share & Share number
		
		User()
		{
			;
		}

		void Get_Share(Server*);
		void Print_Share(int ShareNumber);
	};
};