// SprataCharacter.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SprataCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UWidgetComponent;

UCLASS()
class JBG_ASSIGNMENT08_API ASprataCharacter : public ACharacter
{
	GENERATED_BODY()
	


public:
	// Sets default values for this character's properties
	ASprataCharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") // 속성 정도는 바꿀 수있대
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;
	
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount, // 데미지양
		struct FDamageEvent const& DamageEvent, // 어떤 데미지 유형일지
		AController* EventInstigator, // 데미지를 누가 입혔는지
		AActor* DamageCauser // 데미지를 일으킨 오브젝트
		) override;
	

	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& vlalue);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	
	void OnDeath();
	void UpdateOverheadHP();


private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
};
