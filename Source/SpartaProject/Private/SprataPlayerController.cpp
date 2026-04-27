// SpartaPlayerController.cpp


#include "SprataPlayerController.h"
#include "SpartaGameState.h"
#include "SpartaGameInstance.h"
#include "TransitionWidget.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"


ASprataPlayerController::ASprataPlayerController() : InputMappingContext(nullptr),
                                                     MoveAction(nullptr),
                                                     JumpAction(nullptr),
                                                     LookAction(nullptr),
                                                     SprintAction(nullptr),
                                                     HUDWidgetClass(nullptr),
                                                     HUDWidgetInstance(nullptr),
                                                     MainMenuWidgetClass(nullptr),
                                                     MainMenuWidgetInstance(nullptr),
                                                     TransitionWidgetClass(nullptr),
                                                     TransitionWidgetInstance(nullptr){}

void ASprataPlayerController::BeginPlay(){
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext) { SubSystem->AddMappingContext(InputMappingContext, 0); }
		}
	}

	// 맨처음에만 적용
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel")) { ShowMainMenu(false); }
}

UUserWidget* ASprataPlayerController::GetHUDWidget() const{ return HUDWidgetInstance; }

void ASprataPlayerController::ClearAllWidget(){
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
}

void ASprataPlayerController::ShowMainMenu(bool bIsRestart){
	ClearAllWidget();

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true; // 마우스커서가UI한테만  가도록
			SetInputMode(FInputModeUIOnly());
		}

		if (UTextBlock* ButtonText = Cast<
			UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart) { ButtonText->SetText(FText::FromString(TEXT("Restart?"))); }
			else { ButtonText->SetText(FText::FromString(TEXT("Start"))); }
		}

		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc) { MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr); }

			if (UTextBlock* TotalScoreText = Cast<
				UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(
					UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score: %d"), SpartaGameInstance->TotalScore)
					));
				}
			}
		}
	}
}

void ASprataPlayerController::ShowGameHUD(){
	SetPause(false);
	
	ClearAllWidget();
	
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false; // 마우스커서가UI한테만  가도록
			SetInputMode(FInputModeGameOnly());
		}

		ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
		if (SpartaGameState) { SpartaGameState->UpdateHUD(); }
	}
}

void ASprataPlayerController::ShowTransition(){
	SetPause(true);
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());

	if (TransitionWidgetClass)
	{
		if (!TransitionWidgetInstance)
		{
			TransitionWidgetInstance = CreateWidget<UTransitionWidget>(this, TransitionWidgetClass);
		}

		if (TransitionWidgetInstance) { TransitionWidgetInstance->ShowTransitionWidget(); }
	}
}


void ASprataPlayerController::StartGame(){
	if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SpartaGameInstance->CurrentLevelIndex = 0;
		SpartaGameInstance->TotalScore = 0;
	}
	ShowTransition();
	// UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
}
