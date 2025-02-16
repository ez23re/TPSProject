#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// 1. 충돌체 등록
	CollisionComp = CreateDefaultSubobject<USphereComponent> ( TEXT ( "CollisionComp" ) );

	// 2. 충돌 프로파일 설정
	CollisionComp->SetCollisionProfileName ( FName ( "BlockAll" ) );

	// 3. 충돌체 크기 설정
	CollisionComp->SetSphereRadius ( 20.f );

	// 4. 루트로 등록
	SetRootComponent ( CollisionComp );

	// 5. 외관 컴포넌트 등록
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent> ( TEXT ( "MeshComp" ) );

	// 6. 부모 컴포넌트 지정
	MeshComp->SetupAttachment ( RootComponent );

	// 7. 충돌 비활성화
	MeshComp->SetCollisionEnabled ( ECollisionEnabled::NoCollision );

	// 8. 외관 크기 설정
	MeshComp->SetRelativeScale3D ( FVector ( 0.5 ) );

	// 9. 발사체 컴포넌트 등록
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent> ( TEXT ( "MovementComp" ) );

	// Movement Component가 갱신시킬 컴포넌트 지정
	MovementComp->SetUpdatedComponent ( CollisionComp );

	// 초기 속도
	MovementComp->InitialSpeed = 7000.f;

	// 최대 속도
	MovementComp->MaxSpeed = 7000.f;

	// 반동 여부
	MovementComp->bShouldBounce = true;

	// 반동 값 
	MovementComp->Bounciness = 0.3f;

	// 생명(시간)주기
	// InitialLifeSpan = 0.3f;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	// 사망 Timer 설정
	FTimerHandle deathTimer;
	// GetWorld()->GetTimerManager().SetTimer( deathTimer , this , &ABullet::Die , 0.3f , false );

	// lambda식으로
	GetWorld ( )->GetTimerManager ( ).SetTimer ( deathTimer ,
		FTimerDelegate::CreateLambda ( [this]( )->void
			{
				Destroy ( );
			} ) , 0.3f , false );
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Die ( )
{
	Destroy ( );
}

