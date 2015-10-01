#include "FNX_TextureManager.h"
#include <d3dx9.h>
#include <d3d9.h>

HRESULT FNX_TextureManager::Add(std::string FileName,LPDIRECT3DTEXTURE9* AddedTexture)
{
	*AddedTexture=NULL;

	for(int i=0;i<(int)Textures.size();++i)
	{
		if(StrToUpper(Textures[i].FileName)==StrToUpper(FileName))
		{
			//alredy added this texture
			*AddedTexture=Textures[i].Texture;
			return S_OK;
		} 
	}

	FNX_Texture NewTexture;
	NewTexture.FileName=FileName;

	if FAILED(pLogger->Log(std::string("Creating Texture "+FileName),D3DXCreateTextureFromFile(pD3DDevice,FileName.c_str(),&NewTexture.Texture)))
		return E_FAIL;
	
	*AddedTexture=NewTexture.Texture;
	
	
	Textures.push_back(NewTexture);
	


	return S_OK;

};
/*
HRESULT FNX_TextureManager::Add(std::string FileName)
{
	//AddedTexture=NULL;

	for(int i=0;i<(int)Textures.size();++i)
	{
		if(StrToUpper(Textures[i].FileName)==StrToUpper(FileName))
		{
			//alredy added this texture
			//AddedTexture=Textures[i].Texture;
			return S_OK;
		} 
	}

	FNX_Texture NewTexture;
	NewTexture.FileName=FileName;

	if FAILED(pLogger->Log(std::string("Creating Texture "+FileName),D3DXCreateTextureFromFile(pD3DDevice,FileName.c_str(),&NewTexture.Texture)))
		return E_FAIL;

	//AddedTexture=NewTexture.Texture;
	Textures.push_back(NewTexture);

	return S_OK;

};
*/

FNX_TextureManager::~FNX_TextureManager()
{
	
	for(int i=0;i<(int)Textures.size();++i)
	{
		Textures[i].Texture->Release();
	}
}
