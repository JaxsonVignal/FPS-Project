// FPSGameMode.cpp
#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	// Initialize score
	Score = 0;
}

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Create score widget if set
	if (ScoreWidgetClass)
	{
		ScoreWidget = CreateWidget<UUserWidget>(GetWorld(), ScoreWidgetClass);
		if (ScoreWidget)
		{
			ScoreWidget->AddToViewport();
			ScoreText = Cast<UTextBlock>(ScoreWidget->GetWidgetFromName(TEXT("ScoreText")));
			UpdateScoreDisplay();
		}
	}
}

void AFPSGameMode::EnemyKilled()
{
	Score += 100;
	UpdateScoreDisplay();
}

void AFPSGameMode::UpdateScoreDisplay()
{
	if (ScoreText)
	{
		FString ScoreString = FString::Printf(TEXT("Score: %d"), Score);
		ScoreText->SetText(FText::FromString(ScoreString));
	}
}