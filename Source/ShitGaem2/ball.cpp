#include "ball.h"
#include "Components/StaticMeshComponent.h"
#include "SoccerGameMode.h"
#include "SoccerGameState.h"
#include "Goalgreen.h"
#include "Goalred.h"
#include "characterthatworks.h"
#include "Projectile.h"
#include "Gamebounds.h"
#include "MenuHUD.h"
#include "SoccerPlayerController.h"
#include "Ground.h"
#include "TimerManager.h"


// Sets default values
Aball::Aball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicateMovement(true);
	SetReplicates(true);
	
	if (GetLocalRole() < ROLE_Authority) SetRole(ROLE_SimulatedProxy);
	bCanPlay = true;
	BallSoundBobby = 0;
	BallSoundCount = 0;
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>("BallMesh");
	BallMesh->SetSimulatePhysics(true);
	SetRootComponent(BallMesh);
	SoundEffectBall = CreateDefaultSubobject<UAudioComponent>("Sound");
	SoundEffectBall->SetupAttachment(BallMesh);
	
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
	if(GetLocalRole() == ROLE_Authority)
	{
		if (AGoalgreen* Target = Cast<AGoalgreen>(OtherActor))
		{
			MultiGoalGreenHit(Target);
			MultiSetHidden();
			
			GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle2, this, &Aball::CallGreenGoalHit, 0.7f, false);
			
			BallSoundCount = 0;
			return;
		}

		if (AGoalred* Target = Cast<AGoalred>(OtherActor))
		{
			MultiGoalRedHit(Target);
			MultiSetHidden();
			
			GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle2, this, &Aball::CallRedGoalHit, 0.7f, false);
			
			BallSoundCount = 0;
			return;
		}
	
		if (AProjectile* Projectile = Cast<AProjectile>(OtherActor))
		{
			Projectile->Destroy();
			Aball::BallJump();
			MultiProjectileHit();
			BallSoundCount = 0;
			return;
		}

		if (AGamebounds* Target3 = Cast<AGamebounds>(OtherActor))
		{
			BallDestroy();
			Aball::SpawnBall();
			BallSoundCount = 0;
			return;
		}

		if (Acharacterthatworks* bobby = Cast<Acharacterthatworks>(OtherActor))
		{
			BallSoundCount = 0;
			MultiSoundPlay();
			return;
		}

		if (AGround* Ground = Cast<AGround>(OtherActor))
		{
			if (BallSoundCount < 3)
			{
				MultiSoundPlay();
				BallSoundCount++;
			}
			
			return;
		}
		MultiSoundPlay();
		BallSoundCount = 0;
	}
}

void Aball::MultiSetHidden_Implementation()
{
	BallMesh->SetHiddenInGame(true);
}

void Aball::MultiGoalRedHit_Implementation(AGoalred* Target)
{
	Target->Explosion->Activate();
	Target->SoundEffect->Play();
	BallSoundCount = 0;
	BallSoundBobby = 0;
}

void Aball::MultiGoalGreenHit_Implementation(AGoalgreen* Target)
{
	Target->Explosion->Activate();
	Target->SoundEffect->Play();
	BallSoundCount = 0;
	BallSoundBobby = 0;
}

void Aball::MultiProjectileHit_Implementation()
{
	SoundEffectBall->Play();
	BallSoundCount = 0;
	BallSoundBobby = 0;
}

void Aball::MultiSoundPlay_Implementation()
{
	if(bCanPlay)
	{ 
	SoundEffectBall->Play();
	bCanPlay = false;
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &Aball::ResetBallSoundEffect, 0.1f, false);
	
	}
}

void Aball::ResetBallSoundEffect()
{
	bCanPlay = true;
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);
}

void Aball::SpawnBall()
{
	BallDestroy();
	FActorSpawnParameters SpawnParams;
	FTransform BallRespawn = BallMesh->GetComponentTransform();
	BallRespawn.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	BallRespawn.SetLocation(FVector(0.f, 0.f, 1100.f));
	GetWorld()->SpawnActor<Aball>(ShitBall, BallRespawn, SpawnParams);
	GetWorldTimerManager().ClearTimer(MemberTimerHandle3);
}




void Aball::ServerSpawnBall_Implementation()
{
	SpawnBall();
}


void Aball::ServerCallRedGoalHit_Implementation()
{
	CallRedGoalHit();
}

void Aball::ServerCallGreenGoalHit_Implementation()
{
	CallGreenGoalHit();
}


void Aball::CallGreenGoalHit()
{
	if (ASoccerGameState* GM = Cast<ASoccerGameState>(GetWorld()->GetGameState()))
	{
		GM->OnGreenGoalHit();
	}
	for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
	{
		ASoccerPlayerController* playerController = Cast<ASoccerPlayerController>(*iter);
		Acharacterthatworks* Bobby = Cast<Acharacterthatworks>(playerController->GetCharacter());
		Bobby->ClientWhichTeam(playerController);
	}
	GetWorldTimerManager().ClearTimer(MemberTimerHandle2);
	Aball::SpawnBall();
}

void Aball::CallRedGoalHit()
{
	if (ASoccerGameState* GM = Cast<ASoccerGameState>(GetWorld()->GetGameState()))
	{
		GM->OnRedGoalHit();
	}
	for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter) 
		{
		ASoccerPlayerController* playerController = Cast<ASoccerPlayerController>(*iter);
		Acharacterthatworks* Bobby =Cast<Acharacterthatworks>(playerController->GetCharacter());
		Bobby->ClientWhichTeam(playerController);
		}
	GetWorldTimerManager().ClearTimer(MemberTimerHandle2);
	Aball::SpawnBall();
}

void Aball::BallDestroy()
{
	if (GetLocalRole() < ROLE_Authority)
	{
		ServerBallDestroy();
		
		return;
	}
	Destroy();
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);
}

void Aball::ServerBallDestroy_Implementation()
{
	BallDestroy();
}



void Aball::BallJump()
{

	if (GetLocalRole() < ROLE_Authority)
	{
		
		ServerBallJump();
		return;
	}
	BallMesh->UPrimitiveComponent::SetPhysicsLinearVelocity(FVector(0.f, 0.f, 700.f));
}

void Aball::NetMulticastBallJump_Implementation()
{
	BallMesh->UPrimitiveComponent::SetPhysicsLinearVelocity(FVector(0.f, 0.f, 700.f));
}

void Aball::ServerBallJump_Implementation()
{
	NetMulticastBallJump();
}