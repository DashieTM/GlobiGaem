// 2020TMCR


#include "BobbyPlayerState.h"

ABobbyPlayerState::ABobbyPlayerState()
{
	BobbyName.FromString("");
}

void ABobbyPlayerState::SetBobbyName(const FText& Name)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSetBobbyName(Name);
		return;
	}
	BobbyName = Name;
}

FText ABobbyPlayerState::ReturnBobbyName()
{
	return BobbyName;
}

void ABobbyPlayerState::ServerSetBobbyName_Implementation(const FText& Name)
{
	SetBobbyName(Name);
}