// SimpleGameMode.cpp
#include "SimpleGameMode.h"
#include "MainMenuScript.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ASimpleGameMode::ASimpleGameMode()
{
    // Set default widget class - you'll need to create a BP based on MainMenuScript
    // and assign it in the editor or here via ConstructorHelpers
}

void ASimpleGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuWidgetClass)
    {
        MainMenuWidget = CreateWidget<UMainMenuScript>(GetWorld(), MainMenuWidgetClass);
        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();

            // Show mouse cursor
            APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
            if (PC)
            {
                PC->bShowMouseCursor = true;
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}