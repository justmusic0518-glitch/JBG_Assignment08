#include "SprataCharacter.h"
#include "SprataPlayerController.h"
#include "EnhancedInputComponent.h"
#include "SpartaGameState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

ASprataCharacter::ASprataCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent> (TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f; // 삼각대 거리
	SpringArmComp->bUsePawnControlRotation = true; // 컨트롤러 움직일때 스프링 암도 같이 움직이게 해주세요
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp,  USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false; // 삼각대만 움직이고 카메라는 움직이지 말아주세요
	
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed; // 캐릭터 이동 속도 
	
	MaxHealth = 100.0f;
	Health = MaxHealth;
}

void ASprataCharacter::BeginPlay(){
	Super::BeginPlay();
	UpdateOverheadHP();
}

void ASprataCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASprataPlayerController* PlayerController = Cast<ASprataPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,  // 입력이 눌렀을때
					this,
					&ASprataCharacter::Move  // MoveAction이 있을때 Move를 연결 시키겠다
					);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&ASprataCharacter::StartJump
					);
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed, // 끝났을때
					this,
					&ASprataCharacter::StopJump
					);
			}
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,  // 입력이 눌렀을때
					this,
					&ASprataCharacter::Look  // MoveAction이 있을때 Move를 연결 시키겠다
					);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&ASprataCharacter::StartSprint
					);
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed, // 끝났을때
					this,
					&ASprataCharacter::StopSprint
					);
			}
			
		}
	}
}


void ASprataCharacter::Move(const FInputActionValue& value){
	// value가 2D Vector로 들어옴
	if (!Controller) return; // 컨트롤러가 있는지 확인
	
	const FVector2D MoveInput = value.Get<FVector2D>();
	
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

void ASprataCharacter::StartJump(const FInputActionValue& value){
	if (value.Get<bool>())
	{
		Jump();
	}
}

void ASprataCharacter::StopJump(const FInputActionValue& value){
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void ASprataCharacter::Look(const FInputActionValue& value){
	FVector2D LookInput = value.Get<FVector2D>();
	
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void ASprataCharacter::StartSprint(const FInputActionValue& value){
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ASprataCharacter::StopSprint(const FInputActionValue& value){	
	if (GetCharacterMovement())
	{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

float ASprataCharacter::GetHealth() const{
	return Health;
}

void ASprataCharacter::AddHealth(float Amount){
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

float ASprataCharacter::TakeDamage(
	float DamageAmount, // 데미지양
	struct FDamageEvent const& DamageEvent, // 어떤 데미지 유형일지
	AController* EventInstigator, // 데미지를 누가 입혔는지
	AActor* DamageCauser // 데미지를 일으킨 오브젝트
	)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();
	
	if (Health <=  0.0f)
	{
		OnDeath();
	}
	return ActualDamage;
}

void ASprataCharacter::OnDeath(){
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->OnGameOver();
	}
}

void ASprataCharacter::UpdateOverheadHP(){
	/*
	if (!OverheadWidget) return;
	
	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;
	
	if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
	*/
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASprataPlayerController* SpartaPlayerController = Cast<ASprataPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* HPText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName((TEXT("PlayerHP")))))
				{
					HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
				}
				if (UProgressBar* HPBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName((TEXT("ProgressHPBar")))))
				{
					float HPPercent = Health / MaxHealth;
					HPBar->SetPercent(HPPercent);
				}
			}
		}
	}
	
}




