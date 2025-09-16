# Attack Combo

## 개요
- 플레이어의 입력에 따라 자연스러운 연속 공격을 구현합니다

## 구현 방식
### Combo Window
- Anim Notify State를 사용하여 애니메이션의 특정 구간에
콤보가 가능한 영역(Combo Window)을 정의합니다.
[ComboWindow](Docs/Attack Combo/ComboWindow.png)

### Local Prediction
- 실행 중인 공격 어빌리티에서 입력이 들어오면 `ComboWindow` 상태를 확인하고 다음 공격 어빌리티를 활성화합니다.
[ComboInput](Docs/Attack Combo/ComboInput.png)

- 이때, 클라이언트에서 먼저 다음 공격 어빌리티를 활성화한 다음 서버로 요청을 보냅니다.
[CommitCombo](../DungeonRealms/Equipment/Weapon/DungeonRealmsGameplayAbility_MeleeWeapon.cpp#L19-L39)
```cpp
void UDungeonRealmsGameplayAbility_MeleeWeapon::CommitCombo()
{
	if (bWasComboCommitted)
	{
		return;
	}

	const UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	if (const FGameplayAbilitySpec* NextAttackSpec = AbilitySystemComponent->FindAbilitySpecFromClass(NextAttackClass))
	{
		if (HasAuthority(&CurrentActivationInfo))
		{
			AdvanceCombo(NextAttackSpec->Handle);
		}
		else
		{
			ServerCommitCombo(NextAttackSpec->Handle);
			AdvanceCombo(NextAttackSpec->Handle);
		}
	}
}
```

- 서버에서는 클라이언트의 요청이 올바른지 평가하고, 만약 유효하지 않으면 클라이언트가 활성화한 어빌리티를 취소합니다.
[ServerCommitCombo](../DungeonRealms/Equipment/Weapon/DungeonRealmsGameplayAbility_MeleeWeapon.cpp#L41-L54)
```cpp
void UDungeonRealmsGameplayAbility_MeleeWeapon::ServerCommitCombo_Implementation(
	FGameplayAbilitySpecHandle PredicatedAbilitySpecHandle)
{
	UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	
	if (IsComboWindowOpen() && IsCorrectPrediction(PredicatedAbilitySpecHandle))
	{
		EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
	}
	else
	{
		AbilitySystemComponent->CancelAbilityHandle(PredicatedAbilitySpecHandle);
	}
}
```

### Motion Warping
- 공격 애니메이션의 이동 및 회전을 조정하여 더욱 자연스럽게 애니메이션을 재생합니다.

## 문제 및 해결
### ComboWindow 동기화
클라이언트에서 다음 공격 어빌리티를 예측 활성화할 때, 서버에서 이를 검증하기 위해서는 ComboWindow 동기화가 필요했습니다. 이를 위해서 클라이언트와 서버에서 애니메이션 몽타주를 각자 실행을 하여 Anim Notify State를 실행하였습니다.

### 콤보 
클라이언트에서 콤보 요청이 왔을 때, 네트워크 지연 때문에 서버에 요청에 도달했을 때는 Combo Window가 닫혀있을 수 있습니다. 따라서 이를 보상해주기 위해 서버에서 Combo Window가 닫혔을 때 시간과 클라이언트에서 요청이 왔을 때의 시간 차이를 계산하여 어느정도 늦게 오더라도 콤보 요청을 유효하다고 판단했습니다.
[IsComboWindowOpen](../DungeonRealms/Equipment/Weapon/DungeonRealmsGameplayAbility_MeleeWeapon.cpp#L85-L96)
```cpp
bool UDungeonRealmsGameplayAbility_MeleeWeapon::IsComboWindowOpen() const
{
	if (HasAuthority(&CurrentActivationInfo))
	{
		const float ComboCommitLatency = GetWorld()->GetTimeSeconds() - ComboWindowCloseTime;
		return bComboWindowOpen || ComboCommitLatency <= AllowedComboWindowTolerance;
	}
	else
	{
		return bComboWindowOpen;
	}
}
```

## 결과 영상
[동영상 링크](https://drive.google.com/file/d/1kt9mIrZkSf_mqzFXEYNOQKuekR6NZWXr/view?usp=sharing)