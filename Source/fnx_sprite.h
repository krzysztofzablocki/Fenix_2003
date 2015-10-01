#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "FNX_Logger.h"
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include "FNX_Rotation.h"




class FNX_Sprite	//never used manually , this is only for Sprite Manager , and user uses sprite manager
{
private:
	D3DCOLOR Color;
	float x,y,ScaleX,ScaleY;
	FNX_Rotation Angle;
	bool Visible;
	FNX_Vector MSLU,MERD;//Mapping Start left up and right down
public:
	LPDIRECT3DTEXTURE9 Texture;

	FNX_Sprite(){Visible=true;Texture=NULL;Angle=0.0f;MSLU=FNX_Vector(0.0f,1.0f);MERD=FNX_Vector(1.0f,0.0f);};

	void WriteToVB(volatile  FVF_FNX_Sprite* pVertex,UINT& Offset,WORD* pIndices,UINT& IndexOffset);//writes calculated vertexes to sprite manager for rendering
	void SetColor(D3DCOLOR Col){Color=Col;};
	D3DCOLOR GetColor(){return Color;};

	void SetPosition(float pX,float pY){x=pX;y=pY;}
	void SetPosition(FNX_Vector& Pos){x=Pos.x;y=Pos.y;};
	FNX_Vector GetPosition(){return FNX_Vector(x,y);};
	void SetRotation(float sA){Angle.Set(sA);};
	void SetRotation(FNX_Rotation v){Angle=v;};
	void SetScale(float Sx,float Sy){ScaleX=Sx;ScaleY=Sy;};
	void SetScale(FNX_Vector& Sc){ScaleX=Sc.x;ScaleY=Sc.y;};
	FNX_Vector GetScale(){return FNX_Vector(ScaleX,ScaleY);};
	FNX_Rotation GetRotation(){return Angle;};
	
	void SetVisible(bool sV){Visible=sV;};
	
	void SetUniqueMapping(FNX_Vector& vMSLU,FNX_Vector& vMERD)///In pixels
	{
		MSLU=vMSLU;MERD=vMERD;

		D3DSURFACE_DESC Surface;
	if(Texture)	Texture->GetLevelDesc(0,&Surface);
	
	
		MSLU.x=MSLU.x/(float)Surface.Width;
		MSLU.y=(MSLU.y)/(float)Surface.Height;
	
		MERD.x=MERD.x/(float)Surface.Width;
		MERD.y=(MERD.y)/(float)Surface.Height;//position to Texture sizes
	};

	bool GetVisible(){return Visible;};
	bool TouchesPoint(FNX_Vector& Point){return Point.x>=(x-ScaleX/2) && Point.y>=(y-ScaleY/2) && Point.x<=(x+ScaleX/2) && Point.y<=(y+ScaleY/2);};
};

struct SortTexture : public std::binary_function<FNX_Sprite*, FNX_Sprite*, bool> {
	bool operator()(FNX_Sprite* s1, FNX_Sprite* s2) { return s1->Texture < s2->Texture; }
};

enum FNX_Sort{FNX_Sort_Texture=0};



struct TexFD
{
	TexFD(LPDIRECT3DTEXTURE9 ptex){Texture=ptex;VertexCount=4;};
	LPDIRECT3DTEXTURE9 Texture;
	UINT VertexCount;
};



class FNX_SpriteManager
{
private:
	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;//VB for our sprites
	LPDIRECT3DINDEXBUFFER9 IndexBuffer;//IB for our sprites
	FNX_Logger* pLogger;
	LPDIRECT3DDEVICE9 pD3DDevice;
	std::vector<FNX_Sprite*> Sprites;
	std::vector<TexFD>TDIP;//info about sorted texture and vertex s that use this textures...

	int MaxVertexCount;//max count of vertex s for our Buffers;
	int CurrentVertexCount;//actual count of vertex s
	UINT VBOffset;
	UINT IBOffset;
	bool Inited;//define if sprite manager is ok
	bool Sorted;//define if sprites are sorted
	
public:
	FNX_SpriteManager(FNX_Logger* Logger,int sMaxVertexCount);
	~FNX_SpriteManager(){Invalidate();};
	FNX_Sprite* Add(std::string TextureFileName,bool sort=true);
	void VSort(FNX_Sort Type);
	void Render();

	void Invalidate();
	HRESULT Restore();
	void Release(){Invalidate();delete this;};

};

void GetVertexsFromSprite(FNX_Sprite* Sprite,FNX_Vector* pVertex);///pVertex is returned vertices and must be FNX_Vector [4]
void GetVertexsFromData(FNX_Rotation& Rot,FNX_Vector& Position,FNX_Vector& Scale,FNX_Vector* pVertex);//////pVertex is returned vertices and must be FNX_Vector [4]