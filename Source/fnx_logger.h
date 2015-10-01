#pragma once
/*
#ifndef _FNX_Logger 
	#define _FNX_Logger
*/

#include "FNX_Vector.h"
#include<iostream>
#include<fstream>
#include<vector>
#include<d3d9.h>
#include<d3dx9.h>


class FNX_LineManager;
class FNX_TextureManager;

#define D3DFVF_FNX_Sprite (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define D3DFVF_FNX_Line   (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define SafeRelease(x) if(x){x->Release();x=NULL;};


struct FVF_FNX_Line
{
	float x,y,z;
	D3DCOLOR Color;
};
struct FVF_FNX_Sprite
{
	float x,y,z;
	D3DCOLOR Color;
	float u,v;
};

enum FNX_Result //temporally
{
	FNX_OK=0,
	FNX_ERROR=1
};

std::string FNX_ResultToStr(FNX_Result Result);
std::string StrToUpper(const std::string& x);

struct FNX_Line
{
	FNX_Vector start,end;
	D3DCOLOR Color;
	FNX_Line(FNX_Vector a,FNX_Vector b,D3DCOLOR sColor){start=a;end=b;Color=sColor;};
};



class FNX_Logger//logging to HTML file, easy to color etc.
{
private:
	std::ofstream file;//writable file
	std::vector<std::string>Sections;
	int SectionDepth,ActualSection;
	int DivWidth;

	//Pointers to main devices //almost all modules uses d3d device and uses logger so nicer is to set pointers in logger and use it from here
	LPDIRECT3DDEVICE9 D3DDevice;
	FNX_TextureManager* TextureManager;
	FNX_LineManager* LineManager;
	HWND hWnd;
	HINSTANCE hInstance;
public:
	FNX_Logger(int Divs=10);
	~FNX_Logger();
	FNX_Result Log(std::string,FNX_Result);//returned result is always the same we give to it
	HRESULT Log(std::string,HRESULT);//the same for hresult
	void Log(std::string);
	std::ofstream& GetStream(){return file;};
	void BeginSection(std::string);
	void EndSection();//ending last section
	

	void SetLineManager(FNX_LineManager* LM){LineManager=LM;};
	void SetTextureManager(FNX_TextureManager* Manager){TextureManager=Manager;};
	void SetD3DDevice(LPDIRECT3DDEVICE9 Device){D3DDevice=Device;};
	void SethWnd(HWND shWnd){hWnd=shWnd;};
	HWND GethWnd(){return hWnd;};
	void SethInstance(HINSTANCE sHINSTANCE){hInstance=sHINSTANCE;};
	HINSTANCE GethInstance(){return hInstance;};
	LPDIRECT3DDEVICE9 GetD3DDevice(){return D3DDevice;};
	
	
	FNX_TextureManager* GetTextureManager(){if(TextureManager==NULL)Log("Calling GetTextureManager() before creating TextureManager",FNX_ERROR); return TextureManager;};
	FNX_LineManager* GetLineManager(){if(TextureManager==NULL)Log("Calling GetTextureManager() before creating LineManager",FNX_ERROR);return LineManager;};



	void Release(){delete this;};

};



//#endif