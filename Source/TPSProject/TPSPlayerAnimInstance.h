#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSPlayerAnimInstance.generated.h"

UCLASS()
class TPSPROJECT_API UTPSPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	virtual void NativeUpdateAnimation ( float DeltaSeconds ) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=TPSPlayerAnimInstance)
	float Speed = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= TPSPlayerAnimInstance )
	float Direction = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= TPSPlayerAnimInstance )
	bool IsInAir;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= TPSPlayerAnimInstance )
	class UAnimMontage* AttackAnimMontage;

	void PlayAttackAnim ( );
};
