#include "FNX_Sprite.h"
#include "FNX_TextureManager.h"


void FNX_Sprite::WriteToVB(volatile FVF_FNX_Sprite* pVertex,UINT& Offset,WORD* pIndices,UINT& IndexOffset)
{
	float Z=1.0f;
	if(Visible==false)Z=10000.0f;//not visible out of range


	float Sinus=Angle.im;
	float Cosinus=Angle.re;

	pVertex[Offset+0].x=(-0.5f)*Cosinus*ScaleX-(-0.5f)*Sinus*ScaleY+x;	
	pVertex[Offset+0].y=(-0.5f)*Sinus*ScaleX+(-0.5f)*Cosinus*ScaleY+y;	
	pVertex[Offset+0].z= Z;
	pVertex[Offset+0].Color=Color;
	pVertex[Offset+0].u= MSLU.x;		
	pVertex[Offset+0].v= MERD.y;

	pVertex[Offset+1].x=(-0.5f)*Cosinus*ScaleX-(0.5f)*ScaleY*Sinus+x;	
	pVertex[Offset+1].y=(-0.5f)*Sinus*ScaleX+(0.5f)*ScaleY*Cosinus+y;
	pVertex[Offset+1].z= Z;
	pVertex[Offset+1].Color=Color;
	pVertex[Offset+1].u= MSLU.x;		
	pVertex[Offset+1].v= MSLU.y; 

	pVertex[Offset+2].x=(0.5f)*Cosinus*ScaleX-(-0.5f)*Sinus*ScaleY+x;		
	pVertex[Offset+2].y=(0.5f)*Sinus*ScaleX+(-0.5f)*Cosinus*ScaleY+y;	
	pVertex[Offset+2].z= Z;
	pVertex[Offset+2].Color=Color;
	pVertex[Offset+2].u= MERD.x;		
	pVertex[Offset+2].v= MERD.y;

	pVertex[Offset+3].x=(0.5f)*Cosinus*ScaleX-(0.5f)*Sinus*ScaleY+x;
	pVertex[Offset+3].y=(0.5f)*Sinus*ScaleX+(0.5f)*Cosinus*ScaleY+y;
	pVertex[Offset+3].z= Z;
	pVertex[Offset+3].Color=Color;
	pVertex[Offset+3].u=MERD.x;		
	pVertex[Offset+3].v=MSLU.y;


	pIndices[IndexOffset+0]=Offset+0; 
	pIndices[IndexOffset+1]=Offset+1;
	pIndices[IndexOffset+2]=Offset+2;
	pIndices[IndexOffset+3]=Offset+1;
	pIndices[IndexOffset+4]=Offset+3;
	pIndices[IndexOffset+5]=Offset+2;

	IndexOffset+=6;
	Offset+=4;

};


FNX_SpriteManager::FNX_SpriteManager(FNX_Logger* Logger,int sMaxVertexCount)
{
	IBOffset=VBOffset=CurrentVertexCount=0;
	MaxVertexCount=sMaxVertexCount;
	pLogger=Logger;
	Inited=false;
	Sorted=false;
	pD3DDevice=pLogger->GetD3DDevice();

	pLogger->BeginSection("Sprite Manager");
	pLogger->Log("Creating VertexBuffer",pD3DDevice->CreateVertexBuffer(MaxVertexCount*sizeof(FVF_FNX_Sprite),D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,D3DFVF_FNX_Sprite,D3DPOOL_DEFAULT,&VertexBuffer,NULL));
	pLogger->Log("Creating IndexBuffer",pD3DDevice->CreateIndexBuffer(static_cast<UINT>(MaxVertexCount*1.5*sizeof(WORD)),D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&IndexBuffer,NULL));
	pLogger->EndSection();

	
Inited=true;
};


void FNX_SpriteManager::Invalidate()
{
	if(VertexBuffer!=NULL){ VertexBuffer->Release();VertexBuffer=NULL;};
	if(IndexBuffer!=NULL){IndexBuffer->Release();IndexBuffer=NULL;};
};

HRESULT FNX_SpriteManager::Restore()
{
		IBOffset=VBOffset=CurrentVertexCount=0;
		Sorted=false;

		if(FAILED(pLogger->Log("Restoring VertexBuffer",pD3DDevice->CreateVertexBuffer(MaxVertexCount*sizeof(FVF_FNX_Sprite),D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,D3DFVF_FNX_Sprite,D3DPOOL_DEFAULT,&VertexBuffer,NULL))))
			return E_FAIL;

		if(FAILED(pLogger->Log("Restoring IndexBuffer",pD3DDevice->CreateIndexBuffer(static_cast<UINT>(MaxVertexCount*1.5*sizeof(WORD)),D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&IndexBuffer,NULL))))
			return E_FAIL;


		return S_OK;
};


FNX_Sprite* FNX_SpriteManager::Add(std::string TextureFileName, bool sort )
{
	FNX_Sprite* Sprite= new FNX_Sprite();
	pLogger->GetTextureManager()->Add(TextureFileName,&Sprite->Texture);


	Sprite->SetColor(D3DCOLOR_XRGB(255,255,255));
	Sprites.push_back(Sprite);
	
	if(sort)
	{
		VSort(FNX_Sort_Texture);
	Sorted=true;
	}else 
		Sorted=false;

	return Sprite;
};


void FNX_SpriteManager::VSort(FNX_Sort Type)
{
if(Sprites.size()==0)return;

	if(Type==FNX_Sort_Texture)
	{
		std::sort(Sprites.begin(),Sprites.end(),SortTexture());//quick sort by texture
		TDIP.clear();

		TexFD first(Sprites[0]->Texture);
		TDIP.push_back(first);

		int j=0;

		for(UINT i=1;i<(int)Sprites.size();++i)
		{
			if(TDIP[j].Texture==Sprites[i]->Texture)
			{
				TDIP[j].VertexCount+=4;

			}else
			{
				TDIP.push_back(TexFD(Sprites[i]->Texture));
				++j;
			}

		};

		Sorted=true;

	};


};


void FNX_SpriteManager::Render()
{
	if(Sprites.size()==0)return;//no sprites
	 if(!Sorted)VSort(FNX_Sort_Texture);

	 VBOffset=IBOffset=0;
	 
	 WORD* pIndices;
	 volatile FVF_FNX_Sprite* pVertex;
		int NewDataSize=(int)(Sprites.size()*4);
		 if(NewDataSize>MaxVertexCount){ pLogger->Log("Too much vertex data");return;};
		
		 DWORD LockFlags=D3DLOCK_NOOVERWRITE;
		 if((NewDataSize+CurrentVertexCount)>MaxVertexCount)
		 {																//if full then reset buffer
			 CurrentVertexCount=0;
			 LockFlags=D3DLOCK_DISCARD;
		 };

		 if (FAILED(VertexBuffer->Lock(CurrentVertexCount*sizeof(FVF_FNX_Sprite),NewDataSize*sizeof(FVF_FNX_Sprite),(void**)&pVertex,LockFlags)))return;
		 if (FAILED(IndexBuffer->Lock((CurrentVertexCount+(CurrentVertexCount/2))*sizeof(WORD),(NewDataSize+(NewDataSize/2))*sizeof(WORD),(void**)&pIndices,LockFlags)))return;

		 for(int i=0;i<(int)Sprites.size();++i)
		 {
			 //	if(VectorSprite[i]->visible)
			 Sprites[i]->WriteToVB(pVertex,VBOffset,pIndices,IBOffset);
		 };

		 VertexBuffer->Unlock();
		 IndexBuffer->Unlock();

		 pD3DDevice->SetFVF(D3DFVF_FNX_Sprite);
		 pD3DDevice->SetStreamSource(0,VertexBuffer,0,sizeof(FVF_FNX_Sprite));
		 pD3DDevice->SetIndices(IndexBuffer);

		 FNX_TextureManager* TextureManager=pLogger->GetTextureManager();

		 float HowMuchMore=0;

		 if(TextureManager!=NULL)	pD3DDevice->SetTexture(0,TDIP[0].Texture);
		 pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,CurrentVertexCount,0,TDIP[0].VertexCount,(CurrentVertexCount+(CurrentVertexCount/2)),TDIP[0].VertexCount/2);

		 HowMuchMore+=TDIP[0].VertexCount;
		 for(UINT i=1;i<(int)TDIP.size();++i)
		 {
		
			 if(TextureManager!=NULL)	pD3DDevice->SetTexture(0,TDIP[i].Texture);
			 pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,(int)(CurrentVertexCount+HowMuchMore),0,TDIP[i].VertexCount,(int)(CurrentVertexCount+(CurrentVertexCount/2)),(int)(TDIP[i].VertexCount/2));
		 HowMuchMore+=TDIP[i].VertexCount;
		 };


		 CurrentVertexCount+=NewDataSize;



};


void GetVertexsFromSprite(FNX_Sprite* Sprite,FNX_Vector* pVertex)
{

int Offset=0;
float Cosinus=Sprite->GetRotation().re;
float Sinus=Sprite->GetRotation().im;
int ScaleX=Sprite->GetScale().x;
int ScaleY=Sprite->GetScale().y;
float x=Sprite->GetPosition().x;
float y=Sprite->GetPosition().y;

	pVertex[Offset+0].x=(-0.5f)*Cosinus*ScaleX-(-0.5f)*Sinus*ScaleY+x;	
	pVertex[Offset+0].y=(-0.5f)*Sinus*ScaleX+(-0.5f)*Cosinus*ScaleY+y;	

	pVertex[Offset+1].x=(-0.5f)*Cosinus*ScaleX-(0.5f)*ScaleY*Sinus+x;	
	pVertex[Offset+1].y=(-0.5f)*Sinus*ScaleX+(0.5f)*ScaleY*Cosinus+y;
	
	pVertex[Offset+2].x=(0.5f)*Cosinus*ScaleX-(-0.5f)*Sinus*ScaleY+x;		
	pVertex[Offset+2].y=(0.5f)*Sinus*ScaleX+(-0.5f)*Cosinus*ScaleY+y;	

	pVertex[Offset+3].x=(0.5f)*Cosinus*ScaleX-(0.5f)*Sinus*ScaleY+x;
	pVertex[Offset+3].y=(0.5f)*Sinus*ScaleX+(0.5f)*Cosinus*ScaleY+y;

};


void GetVertexsFromData(FNX_Rotation& Rot,FNX_Vector& Position,FNX_Vector& Scale,FNX_Vector* pVertex)
{

int Offset=0;
float Cosinus=Rot.re;
float Sinus=Rot.im;
int ScaleX=Scale.x;
int ScaleY=Scale.y;
float x=Position.x;
float y=Position.y;

	pVertex[Offset+0].x=(-0.5f)*Cosinus*ScaleX-(-0.5f)*Sinus*ScaleY+x;	
	pVertex[Offset+0].y=(-0.5f)*Sinus*ScaleX+(-0.5f)*Cosinus*ScaleY+y;	

	pVertex[Offset+1].x=(-0.5f)*Cosinus*ScaleX-(0.5f)*ScaleY*Sinus+x;	
	pVertex[Offset+1].y=(-0.5f)*Sinus*ScaleX+(0.5f)*ScaleY*Cosinus+y;
	
	pVertex[Offset+2].x=(0.5f)*Cosinus*ScaleX-(-0.5f)*Sinus*ScaleY+x;		
	pVertex[Offset+2].y=(0.5f)*Sinus*ScaleX+(-0.5f)*Cosinus*ScaleY+y;	

	pVertex[Offset+3].x=(0.5f)*Cosinus*ScaleX-(0.5f)*Sinus*ScaleY+x;
	pVertex[Offset+3].y=(0.5f)*Sinus*ScaleX+(0.5f)*Cosinus*ScaleY+y;

};