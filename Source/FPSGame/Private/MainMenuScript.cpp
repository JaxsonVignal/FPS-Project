// MainMenuScript.cpp
#include "MainMenuScript.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuScript::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (PlayButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &UMainMenuScript::OnPlayClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UMainMenuScript::OnQuitClicked);
    }

    if (TitleText)
    {
        TitleText->SetText(FText::FromString("Main Menu"));
    }
}

void UMainMenuScript::OnPlayClicked()
{
    // Hide the menu and reset input mode before loading level
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->bShowMouseCursor = false;
        PC->SetInputMode(FInputModeGameOnly());
    }

    // Remove the widget from viewport
    this->RemoveFromViewport();

    // Load your main game level
    UGameplayStatics::OpenLevel(this, FName("testMap"));
}

void UMainMenuScript::OnQuitClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
