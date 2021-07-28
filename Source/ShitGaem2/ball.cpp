#include "ball.h"
#include "Components/StaticMeshComponent.h"
#include "SoccerGameMode.h"
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
	
	
	if (GetLocalRole() < ROLE_Authority) SetRole(ROLE_SimulatedProxy);
	if (GetLocalRole() < ROLE_Authority) DisableComponentsSimulatePhysics();
	bCanPlay = true;
	BallSoundBobby = 0;
	BallSoundCount = 0;
	BallHitCount = 0;
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>("BallMesh");
	BallMesh->SetSimulatePhysics(true);
	SetRootComponent(BallMesh);
	SoundEffectBall = CreateDefaultSubobject<UAudioComponent>("Sound");
	SoundEffectBall->SetupAttachment(BallMesh);
	
	OnActorHit.AddDynamic(this, &Aball::OnBallHit);
	Gravity = 1000.f;
	
	bReplicates = true;
	
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
	BallMesh->AddForce(FVector(0.f, 0.f,- Gravity * BallMesh->GetMass()));
	
	
}

//what happens to the ball after hit with a certain actor
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
			//GameStateBall->ResetBall();
			BallSoundCount = 0;
			return;
		}

		if (Acharacterthatworks* bobby = Cast<Acharacterthatworks>(OtherActor))
		{
			if (bobby->GetCharacterMovement()->GetLastUpdateVelocity().Size() < 1800)
			{
				BallMesh->SetPhysicsLinearVelocity(Hit.ImpactNormal * (bobby->GetCharacterMovement()->GetLastUpdateVelocity().Size() + 900));
			}
			else {
				BallMesh->SetPhysicsLinearVelocity(Hit.ImpactNormal * bobby->GetCharacterMovement()->GetLastUpdateVelocity().Size());
			}
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

//set the ball hidden for a few seconds, tape solution for the duration the goal hit function takes
void Aball::MultiSetHidden_Implementation()
{
	BallMesh->SetHiddenInGame(true);
}

//play sound and particle system after red goal hit on all clients
void Aball::MultiGoalRedHit_Implementation(AGoalred* Target)
{
	Target->Explosion->Activate();
	Target->SoundEffect->Play();
	BallSoundCount = 0;
	BallSoundBobby = 0;
}

//play sound and particle system after green goal hit on all clients
void Aball::MultiGoalGreenHit_Implementation(AGoalgreen* Target)
{
	Target->Explosion->Activate();
	Target->SoundEffect->Play();
	BallSoundCount = 0;
	BallSoundBobby = 0;
}

//play the sound on all clients after ball hit
void Aball::MultiProjectileHit_Implementation()
{
	SoundEffectBall->Play();
	BallSoundCount = 0;
	BallSoundBobby = 0;
}

//play the sound on all clients
void Aball::MultiSoundPlay_Implementation()
{
	if(bCanPlay)
	{ 
	SoundEffectBall->Play();
	bCanPlay = false;
	GetWorld()->GetTimerManager().SetTimer(MemberTimerHandle, this, &Aball::ResetBallSoundEffect, 0.1f, false);
	}
}

//cooldown for the sound, we don't want tinitus
void Aball::ResetBallSoundEffect()
{
	bCanPlay = true;
	GetWorldTimerManager().ClearTimer(MemberTimerHandle);
}

//spawn a new ball
void Aball::SpawnBall()
{
	FActorSpawnParameters SpawnParams;
	FTransform BallRespawn = BallMesh->GetComponentTransform();
	BallRespawn.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
	BallRespawn.SetLocation(FVector(0.f, 0.f, 1100.f));
	GetWorld()->SpawnActor<Aball>(ShitBall, BallRespawn, SpawnParams);
}


void Aball::MultiCountdown_Implementation(Acharacterthatworks* bobby)
{
	
}


//spawn the ball serverside
void Aball::ServerSpawnBall_Implementation()
{
	SpawnBall();
}



//server red goal hit implementation
void Aball::ServerCallRedGoalHit_Implementation()
{
	CallRedGoalHit();
}

//server green goal hit implementation
void Aball::ServerCallGreenGoalHit_Implementation()
{
	CallGreenGoalHit();
}

//increase the red points and reset the ball and characters
void Aball::CallGreenGoalHit()
{
	if (ASoccerGameState* GM = Cast<ASoccerGameState>(GetWorld()->GetGameState()))
	{
		GM->OnGreenGoalHit();
		//GM->ResetBall();
	}
	for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter)
	{
		ASoccerPlayerController* playerController = Cast<ASoccerPlayerController>(*iter);
		Acharacterthatworks* Bobby = Cast<Acharacterthatworks>(playerController->GetCharacter());
		Bobby->ClientWhichTeam(playerController, true);
	}
	GetWorldTimerManager().ClearTimer(MemberTimerHandle2);
}

//increase the green points and reset the ball and characters
void Aball::CallRedGoalHit()
{
	if (ASoccerGameState* GM = Cast<ASoccerGameState>(GetWorld()->GetGameState()))
	{
		GM->OnRedGoalHit();
		//GM->ResetBall();
	}
	for (FConstPlayerControllerIterator iter = GetWorld()->GetPlayerControllerIterator(); iter; ++iter) 
		{
		ASoccerPlayerController* playerController = Cast<ASoccerPlayerController>(*iter);
		Acharacterthatworks* Bobby =Cast<Acharacterthatworks>(playerController->GetCharacter());
		Bobby->ClientWhichTeam(playerController, true);
		}
	GetWorldTimerManager().ClearTimer(MemberTimerHandle2);
}

//destroy the ball
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

//destroy the ball serverside
void Aball::ServerBallDestroy_Implementation()
{
	BallDestroy();
}


// ball jump after hit with projectile
void Aball::BallJump()
{
	NetMulticastBallJump();
}

//smoothing ball jump for clients
void Aball::NetMulticastBallJump_Implementation()
{
	BallMesh->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 900.f));
}

//serverballjump implementation, acutal position
void Aball::ServerBallJump_Implementation()
{
}

void Aball::CallCountdown()
{
}