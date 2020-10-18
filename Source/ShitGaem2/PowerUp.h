// 2020TMCR

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"

UCLASS()
class SHITGAEM2_API APowerUp : public AActor
{
	GENERATED_BODY()
	

private:
	

public:	
	// Sets default values for this actor's properties
	APowerUp();
	bool isfirst;
	FTimerHandle MemberTimerHandle;
	FTransform SpawnTransformPowerUp;

	UPROPERTY(Editanywhere, Category = "Components")
		UStaticMeshComponent* PowerUpMesh;

	UPROPERTY(Editanywhere, Category = "Particles")
		class UParticleSystemComponent* Sparks;

	UPROPERTY(EditAnywhere, Category = "PowerUp")
		TSubclassOf<class APowerUp> PowerUpBP;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void OnPowerUpHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	void SpawnPowerUp();
};
