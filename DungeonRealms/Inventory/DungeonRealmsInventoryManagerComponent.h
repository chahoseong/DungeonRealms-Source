#pragma once

#include "CoreMinimal.h"
#include "DungeonRealmsInventoryList.h"
#include "Components/ActorComponent.h"
#include "DungeonRealmsInventoryManagerComponent.generated.h"


class UDungeonRealmsInventoryItemInstance;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUNGEONREALMS_API UDungeonRealmsInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDungeonRealmsInventoryManagerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="DungeonRealms|Inventory")
	void AddItem(UDungeonRealmsInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="DungeonRealms|Inventory")
	void RemoveItem(UDungeonRealmsInventoryItemInstance* Item);

	//~Begin UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End UObject interface
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(Replicated)
	FDungeonRealmsInventoryList InventoryList;
};
