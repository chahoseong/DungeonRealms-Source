#include "Equipment/Actions/EquipmentAction_AttachToSocket.h"
#include "Equipment/DungeonRealmsEquipmentInstance.h"
#include "GameFramework/Character.h"

void UEquipmentAction_AttachToSocket::OnEquipped(UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	if (!EquipmentInstance->HasAuthority())
	{
		return;
	}

	APawn* OwningPawn = EquipmentInstance->GetPawn();
	if (!IsValid(OwningPawn))
	{
		return;
	}

	USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
	if (const ACharacter* OwningCharacter = Cast<ACharacter>(OwningPawn))
	{
		AttachTarget = OwningCharacter->GetMesh();
	}

	for (const FDungeonRealmsEquipmentActorToSpawn& ActorToSpawn : EquipmentActorsToSpawn)
	{
		AActor* NewActor = OwningPawn->GetWorld()->SpawnActorDeferred<AActor>(
			ActorToSpawn.EquipmentActorClass,
			FTransform::Identity,
			OwningPawn
		);
		NewActor->FinishSpawning(FTransform::Identity, true);
		NewActor->SetActorRelativeTransform(ActorToSpawn.AttachTransform);
		NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, ActorToSpawn.AttachSocketName);
		
		EquipmentInstance->AddEquipmentActor(NewActor);
	}
}

void UEquipmentAction_AttachToSocket::OnUnequipped(UDungeonRealmsEquipmentInstance* EquipmentInstance)
{
	if (EquipmentInstance->HasAuthority())
	{
		EquipmentInstance->DestroyEquipmentActors();
	}
}
