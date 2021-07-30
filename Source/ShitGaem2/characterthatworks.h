// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "SoccerGameState.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TextRenderComponent.h"
#include "characterthatworks.generated.h"


UCLASS()
class SHITGAEM2_API Acharacterthatworks : public ACharacter
{
	GENERATED_BODY()

private:
	


	UPROPERTY(ReplicatedUsing = OnRep_CurrentName)
		FText BobbyNameText;

	
	FTransform SpawnTransformPowerUp;

	FTimerHandle MemberTimerHandle;
	FTimerHandle MemberTimerHandle2;
	FTimerHandle MemberTimerHandle3;
	FTimerHandle MemberTimerHandle4;
	FTimerHandle MemberTimerHandle5;
	FTimerHandle MemberTimerHandle6;
	FTimerHandle MemberTimerHandle7;
	

	class Acharacterthatworks* BobbyRed;
	class Acharacterthatworks* BobbyGreen;
	class Acharacterthatworks* BobbyDefault;
	class ASoccerGameState* State;

	UPROPERTY(Replicated)
	bool bCanFire;

	UPROPERTY(Replicated)
	bool bCanDash;

	UPROPERTY(Replicated)
	bool bHasPowerUp;

	UPROPERTY(Replicated)
		bool bIsCountdown;

	UPROPERTY(Replicated)
		FString CountdownText;

	UPROPERTY(Replicated)
		class Acharacterthatworks* BobbyBuffer;

	UPROPERTY(Replicated)
		class Acharacterthatworks* BobbyBufferOld;

	UPROPERTY(EditAnywhere, Category = "Red Bobby")
		TSubclassOf<class Acharacterthatworks> Bobby_Red;

	UPROPERTY(EditAnywhere, Category = "Green Bobby")
		TSubclassOf<class Acharacterthatworks> Bobby_Green;

	

	UPROPERTY(EditAnywhere, Category = "Default Bobby")
		TSubclassOf<class Acharacterthatworks> Bobby_Default;

	UPROPERTY(EditAnywhere, Category = "Shootingposition")
		float ShootingPosition;

	UPROPERTY(EditAnywhere, Category = "Dash Cooldown")
		float DashCD;

	UPROPERTY(EditAnywhere, Category = "Dash lenght")
		float DashLenght;

	UPROPERTY(EditAnywhere, Category = "Dash Friction")
		float DashFriction;

	UPROPERTY(EditAnywhere, Category = "Dash Friction in air")
		float DashFrictionAir;

	UPROPERTY(EditAnywhere, Category = "PowerUpStrenght")
		float PowerUpStrenght;

	UPROPERTY(EditAnywhere, Category = "PowerUpCD")
	float PowerUpCD;

	UPROPERTY(EditAnywhere, Category = "Countdown", Replicated)
		float CountdownTime;
	
	

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	
	// Sets default values for this character's properties
	Acharacterthatworks();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Shooting")
	TSubclassOf<class AProjectile> ProjectileBP;

	UPROPERTY(Editanywhere, Category = "Sound", Replicated)
		UAudioComponent* SoundEffectBobby;

	UPROPERTY(EditAnywhere, Category = "PowerUp")
		TSubclassOf<class APowerUp> PowerUpBP;

	UPROPERTY(EditAnywhere, Category = "BobbyName")
	UTextRenderComponent* BobbyName;


	
	void MoveForward(float Axis); 
	void MoveRight(float Axis);
	void Shoot(); 
	void ResetShoot();
	void ResetDash();
	void DashMov();
	void NoDashMov();
	void CallCountdown();
	void DoCountdown();
	bool ReturnisCountdown();

	void SetBobbyBuffer(class Acharacterthatworks* Bobby);
	void ClearBobbyBuffer();
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;
	
	


	class Acharacterthatworks* GetBobbyBuffer();


	UWorld* OpenLevel();

	UFUNCTION()
		void OnRep_CurrentName();
	
	void UpdateName();

	UFUNCTION()
	void Dash();

	UFUNCTION()
	void DeleteBobby(class Acharacterthatworks* Bobby);

	UFUNCTION()
	void SpawnBobbyDefault(class ASoccerPlayerController* TheNewController);

	UFUNCTION()
	void SpawnBobbyRed(class ASoccerPlayerController* TheNewController, bool ballrespawn);

	UFUNCTION()
	void SpawnBobbyGreen(class ASoccerPlayerController* TheNewController, bool ballrespawn);

	UFUNCTION()
	void UsePowerUp();

	UFUNCTION()
	void SetBobbyName(const FText& LeName, ASoccerPlayerController* BobbyController);


	UFUNCTION(BlueprintPure, Category = "FireStatus")
	FString ReturnFireStatus();

	UFUNCTION(BlueprintPure, Category = "DashStatus")
	FString ReturnDashStatus();

	UFUNCTION(BlueprintPure, Category = "PowerUpStatus")
	FString ReturnPowerUpStatus();

	UFUNCTION(BlueprintPure, Category = "Countdown")
		FString ReturnCountdown();
	
	UFUNCTION(Server, Reliable)
	void ServerShoot();

	UFUNCTION(Server, Reliable)
	void ServerDeleteBobby(class Acharacterthatworks* Bobby);

	UFUNCTION(Server, Reliable)
	void ServerSpawnBobbyDefault(class ASoccerPlayerController* TheNewController);

	UFUNCTION(Server, Reliable)
	void ServerSpawnBobbyRed(class ASoccerPlayerController* TheNewController ,bool ballrespawn);

	UFUNCTION(Server, Reliable)
		void ServerResetDash();


	UFUNCTION(Server, Reliable)
	void ServerSpawnBobbyGreen(class ASoccerPlayerController* TheNewController ,bool ballrespawn);

	UFUNCTION(Server, Reliable)
	void ServerDash();

	UFUNCTION(Server, Reliable)
	void ServerUsePowerUp();

	UFUNCTION(Server, Reliable)
	void ServerSpawnPowerUp(FTransform Power);

	UFUNCTION(Server, Reliable)
	void ServerSpawnPowerUpDelay();

	UFUNCTION(Server, Reliable)
		void CallCountdownServer();

	UFUNCTION(Server, Reliable)
		void CallCountdownClient();

	UFUNCTION(Server, Reliable)
	void ServerSetBobbyName(const FText& LeName, ASoccerPlayerController* BobbyController);


	UFUNCTION(Client, Reliable) //function on client to smooth the movement, works, kinda, not really
	void ClientDash();

	UFUNCTION(Client, Reliable)
	void ClientWhichTeam(class ASoccerPlayerController* SController, bool ballrespawn); //which character are is the client requesting to spawn?

	UFUNCTION(Client, Reliable)
	void ClientUsePowerUp();

	UFUNCTION(Client, Reliable)
	void ClientResetPowerUp();

	UFUNCTION(Client, Reliable)
	void ClientBobbySound();

	UFUNCTION(NetMulticast, Reliable)
		void MultiSetBobbyName(const FText& LeName, ASoccerPlayerController* BobbyController);

	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PowerUpCollected();
	bool HasPowerUp();
};
