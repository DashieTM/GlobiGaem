// 2020TMCR


#include "BobbyPlayerState.h"
#include "Net/UnrealNetwork.h"

ABobbyPlayerState::ABobbyPlayerState()
{
	BobbyName.FromString("");
}

void ABobbyPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABobbyPlayerState, BobbyName);


}

void ABobbyPlayerState::SetBobbyName(const FText& Name)
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerSetBobbyName(Name);
		BobbyName;
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