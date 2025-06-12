// SimpleGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SimpleGameMode.generated.h"

UCLASS()
class FPSGAME_API ASimpleGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASimpleGameMode();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class UUserWidget> MainMenuWidgetClass;

private:
    UPROPERTY()
    class UMainMenuScript* MainMenuWidget;
};
