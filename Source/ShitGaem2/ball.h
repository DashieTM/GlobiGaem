// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoccerGameMode.h"
#include "Components/AudioComponent.h"
#include "ball.generated.h"

UCLASS()
class SHITGAEM2_API Aball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aball();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BallMesh;


	UPROPERTY(EditAnywhere, Category = "Ball")
	TSubclassOf<class Aball> ShitBall;

	UPROPERTY(EditAnywhere, Category = "Gravity")
		float Gravity;

	UPROPERTY(Editanywhere, Category = "Sound")
		UAudioComponent* SoundEffectBall;

private:
	int BallSoundCount;
	int BallSoundBobby;
	int BallHitCount;
	bool bCanPlay;
	bool bBallHit;

	FTimerHandle MemberTimerHandle;
	FTimerHandle MemberTimerHandle2;
	FTimerHandle MemberTimerHandle3;
	FTimerHandle MemberTimerHandle4;
	FTimerHandle MemberTimerHandle5;
	
	class ASoccerGameMode* Mode;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SpawnBall();
	void CallGreenGoalHit();
	void CallRedGoalHit();
	void BallDestroy();
	void BallJump();
	void ResetBallSoundEffect();
	void CallCountdown();
	void ResetBallHit();
	

	UFUNCTION(Server, Reliable)
		void ServerCallRedGoalHit();

	UFUNCTION(Server, Reliable)
		void ServerCallGreenGoalHit();

	UFUNCTION(Server, Reliable)
		void ServerBallDestroy();

	UFUNCTION(Server, Reliable)
		void ServerSpawnBall();

	UFUNCTION(NetMulticast, Reliable)
		void NetMulticastBallJump();

	UFUNCTION(Server, Reliable)
		void ServerBallJump();

	UFUNCTION(NetMulticast, Unreliable)
		void MultiGoalRedHit(class AGoalred* Target);

	UFUNCTION(NetMulticast, Unreliable)
		void MultiGoalGreenHit(class AGoalgreen* Target);

	UFUNCTION(NetMulticast, Unreliable)
		void MultiProjectileHit();

	UFUNCTION(NetMulticast, Unreliable)
		void MultiSoundPlay();

	UFUNCTION(NetMulticast, Unreliable)
		void MultiSetHidden();

	UFUNCTION(NetMulticast, Unreliable)
		void MultiCountdown(Acharacterthatworks* bobby);



	UFUNCTION()
		void OnBallHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
