#include "ball.h"
#include "Components/StaticMeshComponent.h"
#include "SoccerGameMode.h"
#include "Goalgreen.h"
#include "Goalred.h"
#include "Projectile.h"
#include "Gamebounds.h"


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
		Destroy();
		FActorSpawnParameters SpawnParams;
		FTransform BallRespawn = BallMesh->GetComponentTransform();
		BallRespawn.SetLocation(FVector(0.f, 0.f, 1100.f));
		GetWorld()->SpawnActor<Aball>(ShitBall, BallRespawn, SpawnParams);
		if (ASoccerGameMode* GM = Cast<ASoccerGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GM->OnGreenGoalHit();
		}
	}
	if (AGoalred* Target = Cast<AGoalred>(OtherActor))
	{
		Destroy();
		FActorSpawnParameters SpawnParams;
		FTransform BallRespawn = BallMesh->GetComponentTransform();
		BallRespawn.SetLocation(FVector(0.f, 0.f, 1100.f));
		GetWorld()->SpawnActor<Aball>(ShitBall, BallRespawn, SpawnParams);
		if (ASoccerGameMode* GM2 = Cast<ASoccerGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GM2->OnRedGoalHit();
		}
	}
	
	if (AProjectile* Projectile = Cast<AProjectile>(OtherActor))
	{
		BallMesh->UPrimitiveComponent::SetPhysicsLinearVelocity(FVector(0.f,0.f,700.f));
	}
	if (AGamebounds* Target3 = Cast<AGamebounds>(OtherActor))
	{
		Destroy();
		FActorSpawnParameters SpawnParams;
		FTransform BallRespawn = BallMesh->GetComponentTransform();
		BallRespawn.SetLocation(FVector(0.f, 0.f, 1100.f));
		GetWorld()->SpawnActor<Aball>(ShitBall, BallRespawn, SpawnParams);
	}
}

