// 2020TMCR


#include "Ground.h"

// Sets default values
AGround::AGround()
{
	SetReplicateMovement(true);
	SetReplicates(true);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Ground = CreateDefaultSubobject<UStaticMeshComponent>("Ground");
	SetRootComponent(Ground);

}

// Called when the game starts or when spawned
void AGround::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

