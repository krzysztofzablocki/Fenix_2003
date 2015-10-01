#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <d3d9.h>
#include "FNX_Vector.h"

class FNX_FileOperator
{
private:
	std::fstream file;
	std::string buffer;
public:
	FNX_FileOperator(std::string& sFileName);
	~FNX_FileOperator(){file.close();};
	int ReadIntFromFile(std::string Key);
	std::string ReadStringFromFile(std::string Key);
	FNX_Vector ReadVectorFromFile(std::string Key);///Key=(x,y)	
	D3DCOLOR ReadColorARGBFromFile(std::string Key);///Key=(Alpha,Red,Green,Blue);
};
