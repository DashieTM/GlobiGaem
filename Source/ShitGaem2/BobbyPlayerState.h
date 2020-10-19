// 2020TMCR

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SHITGAEM2_API ABobbyPlayerState : public APlayerState
{
	GENERATED_BODY()


protected:

	ABobbyPlayerState();
private:

	UPROPERTY(Replicated)
	FText BobbyName;

public:
	
	
	FText ReturnBobbyName();

	UFUNCTION()
	void SetBobbyName(const FText& Name);

	UFUNCTION(Server, Reliable)
		void ServerSetBobbyName(const FText& Name);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
};
