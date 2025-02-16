#include "TPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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

	TpsCamComp = CreateDefaultSubobject<UCameraComponent> ( "TpsCamComp" );
	TpsCamComp->SetupAttachment ( SpringArmComp );

}

void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

