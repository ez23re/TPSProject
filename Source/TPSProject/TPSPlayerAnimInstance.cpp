#include "TPSPlayerAnimInstance.h"
#include "TPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSPlayerAnimInstance::NativeUpdateAnimation ( float DeltaSeconds )
{
	Super::NativeUpdateAnimation ( DeltaSeconds );

	ATPSCharacter* player = Cast<ATPSCharacter> ( TryGetPawnOwner ( ) );
	if (!player) return;

	FVector velocity = player->GetVelocity ( );
	FVector forwardVector = player->GetActorForwardVector ( );

	Speed = FVector::DotProduct ( velocity , forwardVector );

}
