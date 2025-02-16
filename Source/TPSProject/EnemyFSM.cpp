#include "EnemyFSM.h"
#include "Kismet/GameplayStatics.h"
#include "TPSCharacter.h"
#include "Enemy.h"
#include "TPSProject.h"
#include "Components/CapsuleComponent.h"

UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	
	auto actor = UGameplayStatics::GetActorOfClass ( GetWorld ( ) , ATPSCharacter::StaticClass ( ) );

	// ATPSPlayer 타입으로 캐스팅
	target = Cast<ATPSCharacter>( actor );

	// 소유 객체 가져오기
	me = Cast<AEnemy> ( GetOwner ( ) );
}

void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString logMsg = UEnum::GetValueAsString ( mState );
	GEngine->AddOnScreenDebugMessage ( 0 , 1 , FColor::Red , logMsg );

	switch (mState)
	{
		case EEnemyState::Idle: { IdleState ( ); }	break;
		case EEnemyState::Move: { MoveState ( ); }	break;
		case EEnemyState::Attack: { AttackState ( ); }	break;
		case EEnemyState::Damage: { DamageState ( ); }	break;
		case EEnemyState::Die: { DieState ( ); }	break;
	}
}

void UEnemyFSM::IdleState ( )
{
	// 시간이 흐르다가
	currentTime += GetWorld ( )->DeltaTimeSeconds;

	// 만약 경과 시간이 대기시간을 초과했다면
	if (currentTime > IdleDelayTime)
	{
		// 이동 상태로 전환하고 싶다.
		mState = EEnemyState::Move;

		// 경과 시간 초기화
		currentTime = 0.0f;
	}
}

void UEnemyFSM::MoveState ( )
{
	// 타겟 목적지가 필요하다.
	FVector destination = target->GetActorLocation ( );

	// 방향이 필요하다
	FVector dir = destination - me->GetActorLocation ( );

	// dir 방향으로 이동하고 싶다.
	me->AddMovementInput ( dir.GetSafeNormal ( ) );

	// 타겟과 거리를 체크해서 attackRange 안으로 들어오면 공격상태로 전환 하고 싶다.
	// 거리체크
	if (dir.Size ( ) < attackRange)
	{
		// 공격 상태로 전환하고 싶다.
		mState = EEnemyState::Attack;
	}
}

void UEnemyFSM::AttackState ( )
{
	// 일정 시간에 한번씩 공격하고 싶다.
	// 시간이 흐르다가
	currentTime += GetWorld ( )->DeltaTimeSeconds;

	// 공격시간이 되면
	if (currentTime > attackDelayTime)
	{
		// 공격을 한다.
		PRINT_LOG ( TEXT ( "Attack~!~!~!" ) );

		// 경과 시간 초기화
		currentTime = 0.0f;
	}

	// 타겟이 공격범위를 벗어나면 이동상태로 전환하고 싶다.
	// 타겟과의 거리가 필요하다.
	float distance = FVector::Distance ( target->GetActorLocation ( ) , me->GetActorLocation ( ) );

	// 타겟과의 거리가 공격 범위를 벗어나면
	if (distance > attackRange)
	{
		// 상태를 이동상태로 전환하고 싶다.
		mState = EEnemyState::Move;
	}
}

void UEnemyFSM::DamageState ( )
{
	// 시간이 흐르다가
	currentTime += GetWorld ( )->DeltaTimeSeconds;

	// 경과 시간이 대기 시간을 초과하면
	if (currentTime > damageDelayTime)
	{
		// 대기 상태로 전환 하고 싶다.
		mState = EEnemyState::Idle;

		// 경과 시간 초기화
		currentTime = 0.0f;
	}
}

void UEnemyFSM::DieState ( )
{
	// 계속 아래로 내려가고 싶다.
	// P = P0 + vt
	FVector P0 = me->GetActorLocation ( );
	FVector vt = FVector::DownVector * dieSpeed * GetWorld ( )->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation ( P );

	// 만약 2미터 이상 내려왔다면
	if (P.Z < -200.0f)
	{
		// 제거시킨다.
		me->Destroy ( );
	}
}

void UEnemyFSM::OnDamageProcess ( )
{
	// 체력 감소
	hp--;

	// 체력이 남아있는지 체크
	if (hp > 0)
	{
		// 상태를 피격으로 전환
		mState = EEnemyState::Damage;
	}
	else
	{
		// 상태를 죽음으로 전환
		mState = EEnemyState::Die;
		// 캡슐 충돌체 비활성화
		me->GetCapsuleComponent ( )->SetCollisionEnabled ( ECollisionEnabled::NoCollision );
	}
}