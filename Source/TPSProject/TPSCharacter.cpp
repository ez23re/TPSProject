#include "TPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ATPSCharacter::ATPSCharacter ( )
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshTemp ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'" ) );
	if (MeshTemp.Succeeded ( ))
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

	SniperGunMesh = CreateDefaultSubobject<UStaticMeshComponent> ( "SniperGunMesh" );
	SniperGunMesh->SetupAttachment ( GetMesh ( ) );

	ConstructorHelpers::FObjectFinder<UStaticMesh> TmpSniperGun ( TEXT ( "/Script/Engine.StaticMesh'/Game/SniperGun/sniper11.sniper11'" ) );
	if (TmpSniperGun.Succeeded ( ))
	{
		SniperGunMesh->SetStaticMesh ( TmpSniperGun.Object );
	}
	SniperGunMesh->SetRelativeLocation ( FVector ( 0 , 50 , 120 ) );
	SniperGunMesh->SetRelativeScale3D ( FVector ( .15f ) );

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

	SniperUI = CreateWidget<UUserWidget> ( GetWorld ( ) , SniperUIFactory );

	_CrossHairUI = CreateWidget<UUserWidget> ( GetWorld ( ) , CrossHairUIFactory );
	_CrossHairUI->AddToViewport ( );


	ChangeSniperGun ( FInputActionValue ( ) );
	
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
	
		PlayerInput->BindAction ( IA_GrenadeGun , ETriggerEvent::Started , this , &ATPSCharacter::ChangeGrenadeGun );
		PlayerInput->BindAction ( IA_SniperGun , ETriggerEvent::Started , this , &ATPSCharacter::ChangeSniperGun );
	
		PlayerInput->BindAction ( IA_Sniper , ETriggerEvent::Started , this , &ATPSCharacter::SniperAim );
		PlayerInput->BindAction ( IA_Sniper , ETriggerEvent::Completed , this , &ATPSCharacter::SniperAim );

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
	// 유탄총 사용시
	if (bUsingGrenadeGun)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform firePosition = GunMeshComp->GetSocketTransform ( TEXT ( "FirePosition" ) );
		GetWorld ( )->SpawnActor<ABullet> ( BulletFactory , firePosition , spawnParams );
	}

	// 스나이퍼건 사용시
	else {
		FVector startPos = TpsCamComp->GetComponentLocation ( );
		FVector endPos = startPos + TpsCamComp->GetForwardVector ( ) * 5000;
		FHitResult hitInfo;
		FCollisionQueryParams params;

		params.AddIgnoredActor ( this );

		bool bHit = GetWorld ( )->LineTraceSingleByChannel ( hitInfo , startPos , endPos , ECC_Visibility , params );
		if (bHit)
		{
			FTransform BulletTrans;
			BulletTrans.SetLocation ( hitInfo.ImpactPoint );
			UGameplayStatics::SpawnEmitterAtLocation ( GetWorld ( ) , BulletEffectFactory , BulletTrans );
		}

		auto hitComp = hitInfo.GetComponent ( );
		if (hitComp && hitComp->IsSimulatingPhysics ( )) 
		{
			FVector dir = (endPos - startPos).GetSafeNormal ( );
			FVector force = dir * hitComp->GetMass ( ) * 10000;
			hitComp->AddForceAtLocation ( force , hitInfo.ImpactPoint );
		}
	}
}

void ATPSCharacter::ChangeGrenadeGun ( const FInputActionValue& InputValue )
{
	if (bUsingGrenadeGun) return;

	// 유탄총 사용 중으로 체크
	bUsingGrenadeGun = true;
	GunMeshComp->SetVisibility ( bUsingGrenadeGun );
	SniperGunMesh->SetVisibility ( !bUsingGrenadeGun );
	
}

void ATPSCharacter::ChangeSniperGun ( const FInputActionValue& InputValue )
{
	if (!bUsingGrenadeGun)return;

	// 스나이퍼건 사용 중으로 체크
	bUsingGrenadeGun = false;
	GunMeshComp->SetVisibility ( bUsingGrenadeGun );
	SniperGunMesh->SetVisibility ( !bUsingGrenadeGun );
}

void ATPSCharacter::SniperAim ( const FInputActionValue& InputValue )
{
	// 스나이퍼건 모드가 아니라면 처리하지 않는다
	if (bUsingGrenadeGun) return;


	bSniperAim = !bSniperAim;
	if (bSniperAim)
	{
		SniperUI->AddToViewport ( );
		TpsCamComp->SetFieldOfView ( 45.f );
		_CrossHairUI->RemoveFromParent ( );
	}
	else {
		SniperUI->RemoveFromParent( );
		TpsCamComp->SetFieldOfView ( 90.f );
		_CrossHairUI->AddToViewport ( );
	}
}