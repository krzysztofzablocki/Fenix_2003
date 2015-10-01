#include "FNX_Logger.h"
#include "FNX_LineManager.h"
#include <d3d9.h>
#include <d3dx9.h>


FNX_LineManager::FNX_LineManager(FNX_Logger* Logger)
{
	
	pLogger=Logger;
	
	pD3DDevice=pLogger->GetD3DDevice();

	pLogger->BeginSection("Sprite Manager");

	if(SUCCEEDED(pLogger->Log("Creating VertexBuffer",pD3DDevice->CreateVertexBuffer(10000*sizeof(FVF_FNX_Line),D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,D3DFVF_FNX_Line,D3DPOOL_DEFAULT,&VertexBuffer,NULL))))
		
		pLogger->SetLineManager(this);
	pLogger->EndSection();
	

	
};

void FNX_LineManager::Invalidate()
{
	if(VertexBuffer!=NULL){ VertexBuffer->Release();VertexBuffer=NULL;};
	
};

HRESULT FNX_LineManager::Restore()
{
	
	if(FAILED(pLogger->Log("Restoring VertexBuffer",pD3DDevice->CreateVertexBuffer(10000*sizeof(FVF_FNX_Line),D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY,D3DFVF_FNX_Line,D3DPOOL_DEFAULT,&VertexBuffer,NULL))))
		return E_FAIL;


	return S_OK;
};


FNX_Line* FNX_LineManager::Add(FNX_Vector a, FNX_Vector b,D3DCOLOR sColor)
{
	FNX_Line* NewLine=new FNX_Line(a,b,sColor);
	Lines.push_back(NewLine);
	if(Lines.size()>5000)pLogger->Log("Max Lines count for 1 LineDrawer Exceeded");

	return NewLine;
};

void FNX_LineManager::Render()
{
 if(Lines.size()==0)return;

	FVF_FNX_Line* pVertex;
	VertexBuffer->Lock(0,0,(void**)&pVertex,D3DLOCK_DISCARD);

	for(int i=0;i<(int)Lines.size();++i)
	{
		pVertex[i*2].Color=pVertex[i*2+1].Color=Lines[i]->Color;
		pVertex[i*2].z=pVertex[i*2+1].z=1.0f;
		pVertex[i*2].x=Lines[i]->start.x;pVertex[i*2+1].x=Lines[i]->end.x;
		pVertex[i*2].y=Lines[i]->start.y;pVertex[i*2+1].y=Lines[i]->end.y;
	};

	VertexBuffer->Unlock();

	pD3DDevice->SetTexture(0,NULL);
	pD3DDevice->SetStreamSource(0,VertexBuffer,0,sizeof(FVF_FNX_Line));
	pD3DDevice->SetFVF(D3DFVF_FNX_Line);
	pD3DDevice->DrawPrimitive(D3DPT_LINELIST,0,(int)Lines.size());

	Lines.clear();

};