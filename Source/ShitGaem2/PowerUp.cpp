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
	SetReplicates(true);
	PowerUpMesh = CreateDefaultSubobject<UStaticMeshComponent>("PowerUpMesh");
	PowerUpMesh->SetSimulatePhysics(true);
	SetRootComponent(PowerUpMesh);
	Sparks = CreateDefaultSubobject<UParticleSystemComponent>("Sparks");
	Sparks->SetupAttachment(PowerUpMesh);

	OnActorHit.AddDynamic(this, &APowerUp::OnPowerUpHit);
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

void APowerUp::OnPowerUpHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (Acharacterthatworks* bobby = Cast<Acharacterthatworks>(OtherActor))
		{
			if(isfirst)
			{
			isfirst = false;
			
			SpawnTransformPowerUp.SetLocation(this->GetActorLocation());
			bobby->PowerUpCollected(SpawnTransformPowerUp);
			this->SetActorHiddenInGame(true);
			GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &APowerUp::SpawnPowerUp,6.f, false);
			UE_LOG(LogTemp, Warning, TEXT("Some warning message"));
			}
		}
	}
	
}


void APowerUp::SpawnPowerUp()
{
	
		FActorSpawnParameters SpawnParams;
	GetWorld()->SpawnActor<APowerUp>(PowerUpBP, SpawnTransformPowerUp, SpawnParams);
	Destroy();
}