#pragma once

#include <d3dx9.h>
#include <string>
#include "FNX_Logger.h"

struct FNX_Texture
{
	LPDIRECT3DTEXTURE9 Texture;
	std::string FileName;
};

class FNX_TextureManager
{
private:
	std::vector<FNX_Texture>Textures;
	
	FNX_Logger* pLogger;
	LPDIRECT3DDEVICE9 pD3DDevice;
	
public:
	FNX_TextureManager(FNX_Logger* Logger){pLogger=Logger;pD3DDevice=pLogger->GetD3DDevice(); pLogger->SetTextureManager(this);};
	~FNX_TextureManager();
	void Release(){delete this;};

	HRESULT Add(std::string FileName,LPDIRECT3DTEXTURE9* AddedTexture);//add texture and set pointer AddedTexture to it
	HRESULT Add(std::string FileName);	
	//making GetTexture have no sense because add do the same work + adding if not find wanted texture

	int GetTextureCount(){ return static_cast<int>(Textures.size());}; 


};