#include <iostream>
#include <mpir.h>
#include <string>

#include "bigint.h"
#include "SSA.h"

using namespace FT42;

int Server::Share_Count = 0;

void FT42::Server::String_to_Int()
{
	iamtcs::BigInt Temp;
	size_t len = Secret_Messsage.length();
	int iTemp;

	for (int iCounter = 0; iCounter < len; iCounter++)
	{
		iTemp = Secret_Messsage[iCounter];
		Temp.assign(iTemp);

		Bigint_Message.push_back(Temp);
	}
}

void FT42::Server::Calculate_Maximum_Shares()
{
	size_t iTemp = Secret_Messsage.size();

	Maximum_Shares.assign(iTemp * 2);

	Print(Maximum_Shares, "Maximum_Shares");
}

void FT42::Server::Calculate_Minimum_Shares()
{
	size_t iTemp = Secret_Messsage.size();

	Minimum_Shares.assign(iTemp);

	Print(Minimum_Shares, "Minimum_Shares");
}

int FT42::Server::getShare_Count()
{
	return Share_Count;
}

int FT42::Server::getMaximum_Shares()
{
	char* str = nullptr;
	unsigned int len;
	long long int RetVal;

	Maximum_Shares.toStr(str, len, 10);

	RetVal = std::stoll(str);

	return RetVal;
}

std::vector<std::pair<int, iamtcs::BigInt>> FT42::Server::Create_Share() const
{
	std::vector<std::pair<int, iamtcs::BigInt>> Whole_Share;
	std::pair<int, iamtcs::BigInt> Share;

	iamtcs::BigInt Temp1, Temp2, Temp3, Temp4;

	int num;
	size_t iCount = Bigint_Message.size();
	char* str = nullptr;
	unsigned int len;

	Maximum_Shares.toStr(str, len, 10);
	num = std::stoi(str);

	if (!(Share_Count < num))
	{
		std::cout << "\nMaximum shares created already...Cannot create more shares\n";
		return {};
	}

	Share_Count++;

	//y = 1954 + 43 * 1 + 12 * 1² = 2009;

	for (int iCounter = 0; iCounter < iCount; ++iCounter)
	{
		iamtcs::BigInt Sum(Secret_Messsage[iCounter]);

		Temp1.assign(X);				//43
		Temp2.assign(Share_Count);		//1 ie share number

		Temp1.multiply(Temp2, &Temp3);	//43*1

		Temp1.assign(Y);				//12 
		Temp2.assign(Share_Count);		//1 ie share number

		Temp2.powr(2);					//1^2
		Temp1.multiply(Temp2, &Temp4);	//12 * 1^2 ie share number ^ 2

		Temp3.add(Temp4);				// (43*1) + (12 * 1^2)
		Sum.add(Temp3);					// 1954 + (43*1) + (12 * 1^2)

		Share.first = Share_Count;
		Share.second.assign(Sum);

		Whole_Share.push_back(Share);
	}

	return Whole_Share;
}

std::string FT42::Server::Create_Plain_Message
				(std::vector<std::vector<std::pair<int, iamtcs::BigInt>>> Whole_Share)
{
	size_t iCount = Whole_Share.size(), XLen, iTempCount;
	long double num;
	int iCounter1, iCounter2;
	long long int MinCount;

	char* str = nullptr;
	std::string ReturnMessage;
	unsigned int len;
	char MessageString[1024];
	
	iamtcs::BigInt iFinalDivide(1);
	iamtcs::BigInt iMulTemp(1);			
	iamtcs::BigInt bTemp1, bTemp2;
	iamtcs::BigInt bSum1(1), bSum2(1);
	iamtcs::BigInt BigTemp1(1), BigTemp2;
	iamtcs::BigInt multi;			

	std::vector <iamtcs::BigInt> X;
	std::vector <iamtcs::BigInt> FinalMulti;
	std::vector<iamtcs::BigInt> Lowervec;		//Lower of X ie Lower of Share Number
	std::vector<iamtcs::BigInt> Uppervec;		//Upper of X ie Upper of Share Number

	Minimum_Shares.toStr(str, len, 10);
	MinCount = std::stoll(str);

	if (MinCount > iCount)
	{
		std::cout << "We need more shares to reconstruct message" << std::endl;
		std::cout << "Exiting";
		exit(1);
	}

	iTempCount = Whole_Share[0].size();

	for (iCounter1 = 0; iCounter1 < 1; ++iCounter1)
	{
		for (iCounter2 = 0; iCounter2 < iCount; ++iCounter2)
		{
			X.push_back(Whole_Share[iCounter2][iCounter1].first);
		}
	}

	XLen = X.size();

	for (iCounter1 = 0; iCounter1 < XLen; ++iCounter1)
	{
		for (iCounter2 = 0; iCounter2 < XLen; ++iCounter2)
		{
			if (iCounter1 != iCounter2)
			{
				bTemp1.assign(X[iCounter1]);
				bTemp2.assign(X[iCounter2]);

				bTemp1.subtract(bTemp2);
				bSum1.multiply(bTemp1);

				bSum2.multiply(X[iCounter2]);
			}
		}

		Lowervec.push_back(bSum1);
		Uppervec.push_back(bSum2);

		bSum1.assign(1);
		bSum2.assign(1);
	}

	for (iCounter1 = 0; iCounter1 < XLen; ++iCounter1)
	{
		iMulTemp.assign(Uppervec[iCounter1]);

		iFinalDivide.multiply(Lowervec[iCounter1]);

		for (int iCounter2 = 0; iCounter2 < XLen; ++iCounter2)
		{
			if (iCounter1 != iCounter2)
			{
				iMulTemp.multiply(Lowervec[iCounter2]);				
			}
		}

		FinalMulti.push_back(iMulTemp);
		iMulTemp.assign(1);
	}

	for (iCounter1 = 0; iCounter1 < iTempCount; ++iCounter1)
	{
		for (iCounter2 = 0; iCounter2 < iCount; ++iCounter2)
		{
			BigTemp1.assign(Whole_Share[iCounter2][iCounter1].second);
			multi.assign(FinalMulti[iCounter2]);
			BigTemp1.multiply(multi);
			BigTemp2.subtract(BigTemp1);
		}

		BigTemp2.divideBy(iFinalDivide);

		BigTemp2.toStr(str, len, 10);

		num = std::stoll(str);

		if (num < 0)
		{
			num = num * -1;
		}

		MessageString[iCounter1] = num;

		BigTemp1.assign(1);
		BigTemp2.assign(0);

	}

	MessageString[iCounter1] = 0;
	ReturnMessage.assign(MessageString);

	return ReturnMessage;
}

void FT42::Server::Print(iamtcs::BigInt Obj, std::string ObjName)
{
	char* str = nullptr;
	unsigned int len;

	Obj.toStr(str, len, 10);

	std::cout << ObjName << " = " << str << std::endl;
}

void FT42::User::Get_Share(Server * sPtr)
{
	Share = sPtr->Create_Share();
}

void FT42::User::Print_Share(int ShareNumber)
{
	size_t iCount = Share.size();
	char* str = nullptr;
	unsigned int len;
	iamtcs::BigInt Temp;
	int iTemp;

	std::cout << "\nUser " << ShareNumber + 1 << " has below shares" << std::endl;

	for (int iCounter = 0; iCounter < iCount; ++iCounter)
	{
		iTemp = Share[iCounter].first;
		Temp.assign(Share[iCounter].second);

		Temp.toStr(str, len, 10);

		std::cout << iTemp << " | " << str << std::endl;
	
		str = nullptr;
	}

	std::cout << std::endl;
}