#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSCharacter.h"
#include "Enemy.h"
#include "EnemyFSM.generated.h"

UENUM ( BlueprintType )
enum class EEnemyState : uint8
{
	Idle ,
	Move ,
	Attack ,
	Damage ,
	Die ,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyFSM();

protected:
	virtual void BeginPlay ( ) override;

public:
	virtual void TickComponent ( float DeltaTime , ELevelTick TickType , FActorComponentTickFunction* ThisTickFunction ) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=FSM)
	EEnemyState mState = EEnemyState::Idle;

	void IdleState ( );
	void MoveState ( );
	void AttackState ( );
	void DamageState ( );
	void DieState ( );

	// 대기 시간
	UPROPERTY ( EditDefaultsOnly , Category = FSM )
	float IdleDelayTime = 2.0f;

	// 경과 시간 
	float currentTime = 0.0f;

	// 타켓
	UPROPERTY ( VisibleAnywhere , Category = FSM )
	class ATPSCharacter* target;

	// 소유 액터
	UPROPERTY ( )
	class AEnemy* me;


	// 공격 범위
	UPROPERTY ( EditAnywhere , Category = FSM )
	float attackRange = 150.0f;

	// 공격 대기 시간
	UPROPERTY ( EditAnywhere , Category = FSM )
	float attackDelayTime = 2.0f;


	// 피격 알림 이벤트 함수
	void OnDamageProcess ( );

	// 체력
	UPROPERTY ( EditDefaultsOnly , BlueprintReadWrite , Category = FSM )
	int32 hp = 3;

	// 피격 대기 시간
	UPROPERTY ( EditAnywhere , Category = FSM )
	float damageDelayTime = 2.0f;

	// 아래로 사라지는 속도
	UPROPERTY ( EditAnywhere , Category = FSM )
	float dieSpeed = 50.0f;

};
