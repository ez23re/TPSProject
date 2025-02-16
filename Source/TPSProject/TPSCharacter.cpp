#include "TPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"


ATPSCharacter::ATPSCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshTemp ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'" ) );
	if (MeshTemp.Succeeded())
	{
		GetMesh ( )->SetSkeletalMesh ( MeshTemp.Object );
		GetMesh ( )->SetRelativeLocationAndRotation ( FVector ( 0 , 0 , -90 ) , FRotator ( 0 , -90 , 0 ) );
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent> ( "SpringArmComp" );
	SpringArmComp->SetupAttachment ( RootComponent );
	SpringArmComp->SetRelativeLocation ( FVector ( 0 , 60 , 80 ) );
	SpringArmComp->bUsePawnControlRotation = true;

	TpsCamComp = CreateDefaultSubobject<UCameraComponent> ( "TpsCamComp" );
	TpsCamComp->SetupAttachment ( SpringArmComp );
	TpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent> ( "GunMeshComp" );
	GunMeshComp->SetupAttachment ( GetMesh ( ) );
	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TmpGun ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'" ) );
	if (TmpGun.Succeeded ( ))
		GunMeshComp->SetSkeletalMesh ( TmpGun.Object );

	GunMeshComp->SetRelativeLocation ( FVector ( 0 , 60 , 120 ) );
}

void ATPSCharacter::BeginPlay ( )
{
	Super::BeginPlay ( );

	APlayerController* pc = Cast<APlayerController> ( GetController ( ) );
	if (pc) {
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem> ( pc->GetLocalPlayer ( ) );

		if (subsystem) {
			subsystem->AddMappingContext ( IMC_TPS , 0 );
		}
	}
}

void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Input으로 받아온 값을 Controller의 회전값에 맞춰서 바꿔주기
	Direction = FTransform ( GetControlRotation ( ) ).TransformVector ( Direction );

	//FVector p0 = GetActorLocation ( );
	//FVector vt = Direction.GetSafeNormal ( ) * WalkSpeed * DeltaTime;
	//FVector P = p0 + vt;
	//SetActorLocation (P);

	AddMovementInput ( Direction );
	Direction = FVector::ZeroVector;
}

void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = Cast<UEnhancedInputComponent> ( PlayerInputComponent );
	if (PlayerInput)
	{
		PlayerInput->BindAction ( IA_LookUp , ETriggerEvent::Triggered , this , &ATPSCharacter::LookUp );
		PlayerInput->BindAction ( IA_Turn , ETriggerEvent::Triggered , this , &ATPSCharacter::Turn );
		
		PlayerInput->BindAction( IA_PlayerMove , ETriggerEvent::Triggered , this , &ATPSCharacter::Move );
		
		PlayerInput->BindAction ( IA_PlayerJump , ETriggerEvent::Started , this , &ATPSCharacter::InputJump );
		
		PlayerInput->BindAction ( IA_Fire , ETriggerEvent::Started , this , &ATPSCharacter::InputFire );
	}
}

void ATPSCharacter::Turn ( const FInputActionValue& InputValue )
{
	float value = InputValue.Get<float> ( );
	AddControllerYawInput ( value );
}

void ATPSCharacter::LookUp ( const FInputActionValue& InputValue )
{
	float value = InputValue.Get<float> ( );
	AddControllerPitchInput ( value );
}

void ATPSCharacter::Move ( const FInputActionValue& InputValue )
{
	FVector2D value = InputValue.Get<FVector2D> ( );
	Direction.X = value.X;
	Direction.Y = value.Y;
}

void ATPSCharacter::InputJump ( const FInputActionValue& InputValue )
{
	Jump ( );
}

void ATPSCharacter::InputFire ( const FInputActionValue& InputValue )
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform firePosition = GunMeshComp->GetSocketTransform ( TEXT ( "FirePosition" ) );
	GetWorld ( )->SpawnActor<ABullet> ( BulletFactory , firePosition , spawnParams );
}