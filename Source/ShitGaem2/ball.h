// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
		void OnBallHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
