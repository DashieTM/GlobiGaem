// 2020TMCR


#include "PowerUp.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "characterthatworks.h"
#include "TimerManager.h"
#include "SoccerGameState.h"
// Sets default values
APowerUp::APowerUp()
{

	isfirst = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	PowerUpMesh = CreateDefaultSubobject<UStaticMeshComponent>("PowerUpMesh");
	PowerUpMesh->SetSimulatePhysics(true);
	SetRootComponent(PowerUpMesh);
	Sparks = CreateDefaultSubobject<UParticleSystemComponent>("Sparks");
	Sparks->SetupAttachment(PowerUpMesh);

	
	PowerUpMesh->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::OnPowerUpHit);
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APowerUp::OnPowerUpHit(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (Acharacterthatworks* bobby = Cast<Acharacterthatworks>(OtherActor))
		{
			if(IsHidden() == false)
			{
			bobby->PowerUpCollected();
			this->SetActorHiddenInGame(true);

			GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &APowerUp::SpawnPowerUp,6.f, false);
			}
		}
	}
}


void APowerUp::SpawnPowerUp()
{
	this->SetActorHiddenInGame(false);
	
}

