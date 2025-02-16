#include "Enemy.h"
#include "EnemyFSM.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh에 퀸을 로드해서 넣고 싶다
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh ( TEXT ( "/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'" ) );

	// 만약 파일 읽기가 성공했다면
	if (TempMesh.Succeeded ( )) {
		// 로드한 메시를 넣어주고 싶다
		GetMesh ( )->SetSkeletalMesh ( TempMesh.Object );

		// 위치벡터, 회전벡터 값 넣어주기
		GetMesh ( )->SetRelativeLocationAndRotation ( FVector ( 0 , 0 , -90 ) , FRotator ( 0 , -90 , 0 ) );
	}

	FSM = CreateDefaultSubobject<UEnemyFSM> ( "FSM" );
} 

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

