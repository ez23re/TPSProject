#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class TPSPROJECT_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category=Movement)
	class UProjectileMovementComponent* MovementComp;

	UPROPERTY ( VisibleAnywhere , Category = Collision )
	class USphereComponent* CollisionComp;

	UPROPERTY ( VisibleAnywhere , Category = Movement )
	class UStaticMeshComponent* MeshComp;

	void Die ( );
};
