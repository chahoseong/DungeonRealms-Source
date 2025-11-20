# Project: Dungeon Realms (Multiplayer Action RPG)
**AAA급 액션 RPG의 전투 경험을 멀티플레이어 환경에서 구현하기 위한 R&D 프로젝트**
- Engine: Unreal Engine 5.5
- Core Tech: Gameplay Ability System (GAS), Dedicated Server

# 프로젝트 개요 (Overview)
이 프로젝트는 데디케이티드 서버(Dedicated Server) 환경에서 데이터의 신뢰성을 보장하면서도, 즉각적인 조작감을 제공하는 고품질 액션 RPG 시스템을 구축하는 것을 목표로 했습니다. Unreal Engine 5.5의 **GAS(Gameplay Ability System)**를 기반으로 확장 가능한 전투 및 장비 시스템을 설계하였습니다.

# 구현 내용
## 멀티플레이어 전투 시스템 (Networked Combat with GAS)
멀티플레이 환경에서도 싱글 플레이와 같은 부드러운 조작감을 제공하기 위해 클라이언트 예측(Client Prediction) 기술을 적극 도입했습니다.

- Client Prediction 기반 콤보 시스템: 사용자 입력 즉시 클라이언트에서 어빌리티를 선행 실행하고, 서버가 이를 검증하는 구조를 통해 네트워크 지연을 감추고 끊김 없는 콤보 액션을 구현했습니다.

- 애니메이션 타이밍 동기화: WaitGameplayEvent 태스크와 AnimNotifyState를 활용하여, 공격 판정 및 콤보 입력 윈도우를 애니메이션 프레임에 정확히 동기화했습니다.

- 서버 권한 회피(Server-Authoritative Dodge): 회피(Roll)와 같이 위치 동기화가 중요한 액션은 커스텀 AbilityTask를 제작하여 클라이언트의 입력 벡터(LastMovementInput)를 서버로 전송하고, 서버가 최종 이동 방향과 거리를 확정하도록 하였습니다.

## 데이터 중심 설계 및 확장성 (Data-Driven Design & Scalability)
하드코딩을 지양하고, 기획 데이터의 변경이 코드 수정 없이 즉시 반영될 수 있는 유연한 아키텍처를 구축했습니다.

- 유연한 속성(Attribute) 시스템: UGameplayModMagnitudeCalculation을 상속받아 구현함으로써, '지능(Int)과 민첩(Dex)이 주문력(Ability Power)에 영향을 주는' 복합적인 RPG 공식을 데이터 기반으로 설계했습니다.


- 확장된 전투 피드백 (Context Extension): 기본 FGameplayEffectContext를 상속받아 확장(FDungeonRealmsGameplayEffectContext)하여, 데미지 계산 시 넉백(Knockback), 경직(Stagger), 다운(Knockdown) 판정 데이터를 함께 패킷에 실어 보내도록 구현했습니다. 이를 통해 피격 시 정교한 물리 반응을 처리합니다.

## 네트워크 대역폭 최적화 (Network Optimization)
다수의 유저가 접속하는 환경을 고려하여 데이터 복제 비용을 최소화했습니다.

- FastArraySerializer 활용: 장비(Equipment) 시스템 데이터 동기화 시, 배열 전체를 복제하는 대신 **변경된 항목(Delta)**만 선별적으로 전송하여 네트워크 대역폭을 획기적으로 절감했습니다.

## 확장성 있는 애니메이션 (Advanced Locomotion System)
새로운 무기나 캐릭터가 추가되어도 유지보수가 용이한 모듈형 애니메이션 시스템을 적용했습니다.

- Linked Anim Layer: 무기별 애니메이션 로직을 별도의 레이어로 분리하여, 메인 애니메이션 블루프린트의 수정 없이 무기 데이터 추가만으로 액션을 확장할 수 있는 구조를 확립했습니다.

- Motion Warping: 회피 거리나 공격 사거리가 가변적인 상황에서도, 루트 모션(Root Motion)을 런타임에 동적으로 보정하여 발이 미끄러지는 현상(Sliding) 없는 자연스러운 모션을 구현했습니다.