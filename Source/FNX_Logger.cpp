
#include "FNX_Logger.h"
#include <string>
#include <algorithm>
#include <DxErr.h>




std::string FNX_ResultToStr(FNX_Result Result)
{
	switch(Result)
	{
	case FNX_OK:return "FNX_OK";
	case FNX_ERROR:return "FNX_ERROR";
	};

	return "FNX_Unknown";

};

std::string StrToUpper(const std::string& x)
{
	std::string result = x; 
	std::transform(result.begin(), result.end(), result.begin(), toupper); 
	return result; 
};


FNX_Logger::FNX_Logger(int Divs)
{	
	DivWidth=Divs;
	TextureManager=NULL;
	D3DDevice=NULL;
	
	file.open("Log.html");
	 file.setf(std::ios_base::unitbuf);
	 file.setf(std::ios::floatfield,std::ios_base::fixed);
		
	 file<<"Logging Started\n";
	 SectionDepth=1;
	 ActualSection=0;
	 
};



FNX_Logger::~FNX_Logger()
{	
		for(;SectionDepth>1;SectionDepth--)
			EndSection();

	file<<"Logging ended";
	file.close();
}



FNX_Result FNX_Logger::Log(std::string Description,FNX_Result Result)
{																			
	std::string Color;
	switch(Result)
		{
			case FNX_OK:Color="#006400;";break;//green
			case FNX_ERROR:Color="#FF0000;";//red
		};

		file << "<div style=\"padding-left:" 
			<<SectionDepth*DivWidth<< "px; color:" 
			 << Color.c_str() <<"\">"<<Description.c_str()<<':'
			  <<FNX_ResultToStr(Result).c_str()<<"</div>";
return Result;
}

void FNX_Logger::Log(std::string Description)
{																			
	std::string Color="#FF0000;";
	

	file << "<div style=\"padding-left:" 
		<<SectionDepth*DivWidth<< "px; color:" 
		<< Color.c_str() <<"\">"<<Description.c_str()<<"</div>";
}

HRESULT FNX_Logger::Log(std::string Description,HRESULT Result)
{																			
	std::string Color;
	if(SUCCEEDED(Result)) Color="#006400;";else//green
	Color="#FF0000;";//red
	

	file << "<div style=\"padding-left:" 
		<<SectionDepth*DivWidth<< "px; color:" 
		<< Color.c_str() <<"\">"<<Description.c_str()<<':'
		<<DXGetErrorString(Result)<<"</div>";
	return Result;
}


void FNX_Logger::BeginSection(std::string Name)
{
	Sections.push_back(Name);
	
	file << "<div style=\"padding-left:"<<SectionDepth*DivWidth<< "px;\">"<<"Section Begin: "<<Name.c_str()<<"</div>";
	++SectionDepth;
}



void FNX_Logger::EndSection()
{
	SectionDepth--;
	file << "<div style=\"padding-left:"<<SectionDepth*DivWidth<< "px;\">"<<"Section End: "<<Sections[Sections.size()-1].c_str()<<"</div>";
	Sections.pop_back();
}