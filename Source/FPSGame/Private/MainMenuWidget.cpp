#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

bool UMainMenuWidget::Initialize()
{
    if (!Super::Initialize()) return false;

    if (StartButton)
        StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartGame);

    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitGame);

    return true;
}

void UMainMenuWidget::OnStartGame()
{
    UGameplayStatics::OpenLevel(this, "MainLevel"); // Replace with your actual level name
}

void UMainMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}