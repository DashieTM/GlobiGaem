
#include "SoccerPlayerController.h"



ASoccerPlayerController::ASoccerPlayerController()
{

}


void ASoccerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("OpenMenu", IE_Pressed, this, &ASoccerPlayerController::OpenMenu);
	}

}


void ASoccerPlayerController::OpenMenu()
{
	
}

