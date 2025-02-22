#include "Fish_Set.h"

// AsFish_Set
//------------------------------------------------------------------------------------------------------------
AsFish_Set::~AsFish_Set()
{
	for (auto &fish : Fish)
		delete fish;

	Fish.erase(Fish.begin(), Fish.end());
}
//------------------------------------------------------------------------------------------------------------
AsFish_Set::AsFish_Set()
{
}
//------------------------------------------------------------------------------------------------------------
void AsFish_Set::Init()
{
	Fish.push_back(new AFish(AsConfig::Sea_Widht * 2, Max_Y_Pos + Step_Y_Pos_Fish ) );
	Fish.push_back(new AFish(AsConfig::Sea_Widht, Max_Y_Pos + Step_Y_Pos_Fish * 2) );
	Fish.push_back(new AFish(AsConfig::Sea_Widht / 2, Max_Y_Pos + Step_Y_Pos_Fish * 3) );
	Fish.push_back(new AFish(AsConfig::Sea_Widht / 3, Max_Y_Pos + Step_Y_Pos_Fish * 4) );
}
//------------------------------------------------------------------------------------------------------------
void AsFish_Set::Act()
{
	for (auto &fish : Fish)
		fish->Act();

	//Emit_Fish();
}
//------------------------------------------------------------------------------------------------------------
void AsFish_Set::Draw(HDC hdc, RECT &paint_area)
{
	for (auto &fish : Fish)
		fish->Draw(hdc, paint_area);
}
//------------------------------------------------------------------------------------------------------------
void AsFish_Set::Emit_Fish()
{
	for (auto &fish : Fish)
		fish->Set_State(EFish_State::Emmiting);
}
//------------------------------------------------------------------------------------------------------------
