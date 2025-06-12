// FPSGameMode.h
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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ScoreWidgetClass;

private:
	// Add score tracking
	UPROPERTY()
	int32 Score;

	UPROPERTY()
	class UUserWidget* ScoreWidget;

	UPROPERTY()
	class UTextBlock* ScoreText;

	void UpdateScoreDisplay();
};



