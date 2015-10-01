#pragma once
#include "FNX_Logger.h"
#include <vector>
#include "FNX_Vector.h"


class FNX_LineManager//for drawing lines 1 manager can draw upto 5000 lines... After each draw you must add lines again // should be only 1 in app
{
private:
	std::vector<FNX_Line*>Lines;
	FNX_Logger* pLogger;
	LPDIRECT3DDEVICE9 pD3DDevice;
	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
public:
	FNX_LineManager(FNX_Logger*);
	~FNX_LineManager(){Invalidate();};
	void Release(){delete this;};

	void Invalidate();
	HRESULT Restore();

	FNX_Line* Add(FNX_Vector a,FNX_Vector b,D3DCOLOR sColor);
	void Render();


};