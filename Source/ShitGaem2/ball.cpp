#include "ball.h"
#include "Components/StaticMeshComponent.h"
#include "SoccerGameMode.h"
#include "Goalgreen.h"
#include "Goalred.h"

// Sets default values
Aball::Aball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>("BallMesh");
	BallMesh->SetSimulatePhysics(true);

	OnActorHit.AddDynamic(this, &Aball::OnBallHit);
}

// Called when the game starts or when spawned
void Aball::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void Aball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Aball::OnBallHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (AGoalgreen* Target = Cast<AGoalgreen>(OtherActor))
	{
		if (ASoccerGameMode* GM = Cast<ASoccerGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GM->OnGreenGoalHit();
		}
	}
	if (AGoalred* Target = Cast<AGoalred>(OtherActor))
	{
		if (ASoccerGameMode* GM2 = Cast<ASoccerGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GM2->OnRedGoalHit();
		}
	}
	
}

