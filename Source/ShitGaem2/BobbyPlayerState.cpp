// 2020TMCR


#include "BobbyPlayerState.h"
#include "Net/UnrealNetwork.h"

ABobbyPlayerState::ABobbyPlayerState()
{
	BobbyName.FromString("");
}


void ABobbyPlayerState::SetBobbyName(const FText& Name)
{
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