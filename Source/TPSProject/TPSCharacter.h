#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Bullet.h"
#include "TPSCharacter.generated.h"

UCLASS ( )
class TPSPROJECT_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY ( )

public:
	ATPSCharacter ( );

protected:
	virtual void BeginPlay ( ) override;

public:
	virtual void Tick ( float DeltaTime ) override;

	virtual void SetupPlayerInputComponent ( class UInputComponent* PlayerInputComponent ) override;

public:
	UPROPERTY ( EditAnywhere , Category = Camera )
	class USpringArmComponent* SpringArmComp;

	UPROPERTY ( VisibleAnywhere , BlueprintReadOnly , Category = Camera )
	class UCameraComponent* TpsCamComp;

	// Input
	UPROPERTY ( EditDefaultsOnly , Category = Input )
	class UInputMappingContext* IMC_TPS;

	UPROPERTY ( EditDefaultsOnly , Category = Input )
	class UInputAction* IA_LookUp;

	UPROPERTY ( EditDefaultsOnly , Category = Input )
	class UInputAction* IA_Turn;

	void Turn ( const struct FInputActionValue& InputValue );
	void LookUp ( const struct FInputActionValue& InputValue );

	UPROPERTY ( EditDefaultsOnly , Category = Input )
	class UInputAction* IA_PlayerMove;

	UPROPERTY ( EditDefaultsOnly , Category = PlayerSetting )
	float WalkSpeed = 600.f;

	FVector Direction;

	void Move ( const struct FInputActionValue& InputValue );

	UPROPERTY ( EditDefaultsOnly , Category = Input )
	class UInputAction* IA_PlayerJump;

	void InputJump ( const struct FInputActionValue& InputValue );

	UPROPERTY ( EditDefaultsOnly )
	class USkeletalMeshComponent* GunMeshComp;

	UPROPERTY ( EditDefaultsOnly , Category = BulletFactory )
	TSubclassOf<class ABullet> BulletFactory;

	UPROPERTY ( EditDefaultsOnly , Category = Input )
	class UInputAction* IA_Fire;

	void InputFire ( const struct FInputActionValue& InputValue );

	UPROPERTY ( EditDefaultsOnly )
	class UStaticMeshComponent* SniperGunMesh;

	UPROPERTY ( EditDefaultsOnly , Category = Input )
	class UInputAction* IA_GrenadeGun;

	UPROPERTY ( EditDefaultsOnly , Category = Input )
	class UInputAction* IA_SniperGun;

	void ChangeGrenadeGun ( const struct FInputActionValue& InputValue );
	void ChangeSniperGun ( const struct FInputActionValue& InputValue );

	bool bUsingGrenadeGun = true;

	UPROPERTY ( EditDefaultsOnly , Category = Input )
	class UInputAction* IA_Sniper;

	void SniperAim ( const struct FInputActionValue& InputValue );

	bool bSniperAim;

	UPROPERTY ( EditDefaultsOnly )
	TSubclassOf<class UUserWidget> SniperUIFactory;

	UPROPERTY ( )
	class UUserWidget* SniperUI;

	UPROPERTY ( EditAnywhere , Category = BulletEffect )
	class UParticleSystem* BulletEffectFactory;

	UPROPERTY ( EditDefaultsOnly , Category = SniperUI )
	TSubclassOf<class UUserWidget> CrossHairUIFactory;

	UPROPERTY ( )
	class UUserWidget* _CrossHairUI;
};