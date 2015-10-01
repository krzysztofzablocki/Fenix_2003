#include "FNX_FileOperations.h"
#include <sstream>

FNX_FileOperator::FNX_FileOperator(std::string& sFileName)
{
	file.open(sFileName.c_str());


	std::string sfile;
	getline(file,sfile);
	while (file) {
		buffer += sfile+' ';
		getline(file,sfile);
	}


}

std::string FNX_FileOperator::ReadStringFromFile(std::string Key)
{
	std::string::size_type Index=buffer.find('=',buffer.find(Key,0));
	Index++;

	std::string v;
	for(;Index<buffer.size();++Index)
	{
		if(buffer[Index]=='©')
				return v;
		
	v.push_back(buffer[Index]);

	}

	return v;
};


int FNX_FileOperator::ReadIntFromFile(std::string Key)
{
	std::string::size_type Index=buffer.find('=',buffer.find(Key,0));
	Index++;

	std::string v;
	for(;Index<buffer.size();++Index)
	{
		if(buffer[Index]==' ')
			break;
		
		v.push_back(buffer[Index]);

	};

	return atoi(v.c_str());
};


FNX_Vector FNX_FileOperator::ReadVectorFromFile(std::string Key)
{
	std::string::size_type Index=buffer.find('(',buffer.find(Key,0));
	Index++;

//Key=(x,y)
int x,y;

	std::string v;

	for(;Index<buffer.size();++Index)
	{
		if(buffer[Index]==',')
			break;
		
		v.push_back(buffer[Index]);
	};

	x=atoi(v.c_str());
	

	v.clear();
	Index++;

	for(;Index<buffer.size();++Index)
	{
		if(buffer[Index]==')')
			break;
		
		v.push_back(buffer[Index]);
	};


	y=atoi(v.c_str());

	

	return FNX_Vector((float)x,(float)y);
};



D3DCOLOR FNX_FileOperator::ReadColorARGBFromFile(std::string Key)
{
	std::string::size_type Index=buffer.find('(',buffer.find(Key,0));
	Index++;

///Key=(Alpha,Red,Green,Blue);
int a,r,g,b;

	std::string v;

	for(;Index<buffer.size();++Index)
	{
		if(buffer[Index]==',')
			break;
		
		v.push_back(buffer[Index]);
	};

	a=atoi(v.c_str());
	v.clear();
	Index++;



	for(;Index<buffer.size();++Index)
	{
		if(buffer[Index]==',')
			break;
		
		v.push_back(buffer[Index]);
	};

	r=atoi(v.c_str());
	v.clear();
	Index++;



	for(;Index<buffer.size();++Index)
	{
		if(buffer[Index]==',')
			break;
		
		v.push_back(buffer[Index]);
	};

	g=atoi(v.c_str());
	v.clear();
	Index++;


	for(;Index<buffer.size();++Index)
	{
		if(buffer[Index]==')')
			break;
		
		v.push_back(buffer[Index]);
	};

	b=atoi(v.c_str());
	Index++;


	return D3DCOLOR_ARGB(a,r,g,b);
};