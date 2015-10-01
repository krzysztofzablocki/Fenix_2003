#include "FNX_GUIPanel.h"

/*FNX_GUIPanel::~FNX_GUIPanel()
{
};
*/

void FNX_GUIPanel::Invalidate()
{
	for(int i=0;i<(int)Elements.size();++i)
		Elements[i]->Invalidate();
};

void FNX_GUIPanel::Restore()
{
	for(int i=0;i<(int)Elements.size();++i)
		Elements[i]->Restore();
};

void FNX_GUIPanel::SetPosition(FNX_Vector& s)
{

	FNX_Vector Sub=s-Position;

	for(int i=0;i<(int)Elements.size();++i)
		Elements[i]->SetPosition(Elements[i]->GetPosition()+Sub);

	Position=s;
};

void FNX_GUIPanel::LoadFromFile(std::string FileName)
{

};