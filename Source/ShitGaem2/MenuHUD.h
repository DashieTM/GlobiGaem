// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"


/**
 * 
 */
UCLASS()
class SHITGAEM2_API AMenuHUD : public AHUD
{
	GENERATED_BODY()
	

private:
	int Team;
	bool BobbyRedExists;
	bool BobbyGreenExists;
	bool BobbyDefaultExists;
	bool IsServer;
	FText BobbyName;

	FTimerHandle MemberTimerHandle;

	class Acharacterthatworks* BobbyRed;
	class Acharacterthatworks* BobbyGreen;
	class Acharacterthatworks* BobbyDefault;
	class Acharacterthatworks* BobbyBuffer;
	class ASoccerGameState* GM;
	class ASoccerPlayerController* TheNewController;

	UPROPERTY(EditAnywhere, Category = "Red Bobby")
		TSubclassOf<class Acharacterthatworks> Bobby_Red;

	UPROPERTY(EditAnywhere, Category = "Green Bobby")
		TSubclassOf<class Acharacterthatworks> Bobby_Green;


	UPROPERTY(EditAnywhere, Category = "Default Bobby")
		TSubclassOf<class Acharacterthatworks> Bobby_Default;

protected:

	TSharedPtr<class SMenuWidget> MenuWidget;
	TSharedPtr<class SOptionsWidget> OptionWidget;
	TSharedPtr<class SOVideoOptionsWidget> VideoOptionWidget;
	TSharedPtr<class SWidget> MenuWidgetContainer;
	TSharedPtr<class SWidget> OptionWidgetContainer;
	TSharedPtr<class SWidget> VideoOptionWidgetContainer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PointsHUD", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> PointsHUD;

	

	UPROPERTY()
		class UUserWidget* CurrentWidget;
	
	virtual void BeginPlay() override;

public:

	void RespawnBobby();
	void SetTeamRed();
	void SetTeamGreen();
	void SetTeamSpectator();
	void ShowMenu();
	void RemoveMenu();
	void OpenLevel1();
	void OpenLevel2();
	void SetBobbyBuffer(class Acharacterthatworks* Bobby,int Team);
	class Acharacterthatworks* GetBobbyBuffer();
	void ClearBobbyBuffer();
	void ShowOptions();
	void ShowVideoOptions();
	void RemoveOptions();
	void RemoveVideoOptions();
	int GetBobbyTeam();
	void ReloadBobby(FText PlayerName);
	void SetNameBuffer(FText& Name);
	FText GetNameBuffer();
	void ReloadBobbyWSpawn();

	

	

	
	


};
