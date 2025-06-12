#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSGameMode();

	// Add these score functions
	UFUNCTION(BlueprintCallable)
	void EnemyKilled();

	UFUNCTION(BlueprintCallable)
	int32 GetScore() const { return Score; }

	// Add game over function
	UFUNCTION(BlueprintCallable)
	void ShowGameOver();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ScoreWidgetClass;

	// Add Game Over Widget class
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

private:
	// Add score tracking
	UPROPERTY()
	int32 Score;

	UPROPERTY()
	class UUserWidget* ScoreWidget;

	UPROPERTY()
	class UTextBlock* ScoreText;

	// Add Game Over Widget
	UPROPERTY()
	class UUserWidget* GameOverWidget;

	void UpdateScoreDisplay();
};