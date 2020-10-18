// 2020TMCR

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ground.generated.h"

UCLASS()
class SHITGAEM2_API AGround : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGround();

	UPROPERTY(Editanywhere, Category = "Components")
		UStaticMeshComponent* Ground;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
