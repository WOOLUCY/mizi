# MIZI
괴생명체를 피해 폐품을 모으는 1인칭 SF 공포게임

# 프로젝트 소개
- Unreal Engine 5를 사용한 1인칭 SF 공포 게임
- 블루프린트로 데모 버전을 제작 후, 구현 내용을  C++로 바꿔 개발
- 랜덤맵 생성기를 사용해 플레이마다 새로운 맵을 제공
- 다양한 행동트리를 가진 몬스터를 피해 폐품을 주워 생존하는 게임
- 맵 곳곳에서 파밍할 수 있는 다양한 아이템과 Pick, Drop, Scan 등 상호작용 가능
- 인기 게임 <Lethal Company>의 주요 요소를 모작

# 개발 기간
- 2024.10.14. ~

# 개발 환경
- C++
- Blueprint
- Github
- Visual Studio 2022
- Unreal Engine 5

# 개발 일지
| 이름 | URL | 주제 |
| --- | --- | --- |
| 프롤로그 | [[UE5] 공포게임 MIZI 개발 일지 #0: 들어가기에 앞서](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-0-%EB%93%A4%EC%96%B4%EA%B0%80%EA%B8%B0%EC%97%90-%EC%95%9E%EC%84%9C) |  |
| 랜덤맵 생성기(1) | [[UE5] 공포게임 MIZI 개발 일지 #1: 랜덤맵 생성기(1)](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-1-%EB%9E%9C%EB%8D%A4%EB%A7%B5-%EC%83%9D%EC%84%B1%EA%B8%B01) | Level Design |
| 랜덤맵 생성기(2) | [[UE5] 공포게임 MIZI 개발 일지 #2: 랜덤맵 생성기(2)](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-2-%EB%9E%9C%EB%8D%A4%EB%A7%B5-%EC%83%9D%EC%84%B1%EA%B8%B02) | Level Design |
| 랜덤 아이템 스포너 | [[UE5] 공포게임 MIZI 개발 일지 #3: 아이템 스포너(1)](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-3-%EC%95%84%EC%9D%B4%ED%85%9C-%EC%8A%A4%ED%8F%AC%EB%84%881) | Level Design |
| 캐릭터 | [[UE5] 공포게임 MIZI 개발 일지 #4: 캐릭터](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-4-%EC%BA%90%EB%A6%AD%ED%84%B0) | Character |
| 플레이어 컨트롤러 | [[UE5] 공포게임 MIZI 개발 일지 #5: 플레이어 컨트롤러](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-5-%ED%94%8C%EB%A0%88%EC%9D%B4%EC%96%B4-%EC%BB%A8%ED%8A%B8%EB%A1%A4%EB%9F%AC) | Framework |
| 애니메이션 | [[UE5] 공포게임 MIZI 개발 일지 #6: 애니메이션](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-6-%EC%95%A0%EB%8B%88%EB%A9%94%EC%9D%B4%EC%85%98) | Character |
| 카메라(1) | [[UE5] 공포게임 MIZI 개발 일지 #7: 카메라(1)](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-7-%EC%B9%B4%EB%A9%94%EB%9D%BC1) | Character |
| 카메라(2) | [[UE5] 공포게임 MIZI 개발 일지 #8: 카메라(2)](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-8-%EC%B9%B4%EB%A9%94%EB%9D%BC2) | Character |
| Item Base | [[UE5] 공포게임 MIZI 개발 일지 #9: Item Base](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-9-Item-Base) | Item |
| Item Scan | [[UE5] 공포게임 MIZI 개발 일지 #10: Item Scan](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-10-Item-Scan) | Item |
| Item Pick&Drop | [[UE5] 공포게임 MIZI 개발 일지 #11: Item Pick&Drop](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-11-Item-PickDrop) | Item |
| Inventory | [[UE5] 공포게임 MIZI 개발 일지 #12: Inventory](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-11-Inventory) | UI |
| 상태창 | [[UE5] 공포게임 MIZI 개발 일지 #13: Status Widget](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-13-Status-Widget) | Character, UI |
| EnemyBase | [[UE5] 공포게임 MIZI 개발 일지 #14: Enemy Base](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-14-Enemy-Base) | Enemy |
| AI Controller | [[UE5] 공포게임 MIZI 개발 일지 #15: Basic AI Controller](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-15-Basic-AI-Controller) | Enemy |
| Weeping Angel | [[UE5] 공포게임 MIZI 개발 일지 #16: Weeping Angel](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-16-Weeping-Angel) | Enemy |
| Land Mine | [[UE5] 공포게임 MIZI 개발 일지 #17: Land Mine](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-17-Land-Mine) | Gimmick |
| 랜덤맵 생성기(3) | [[UE5] 공포게임 MIZI 개발 일지 #18: 랜덤맵 생성기(3)](https://velog.io/@woolucy/18.UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-18-%EB%9E%9C%EB%8D%A4%EB%A7%B5-%EC%83%9D%EC%84%B1%EA%B8%B03) | Level Design |
| 카드키 시스템 | [[UE5] 공포게임 MIZI 개발 일지 #19: 카드키](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-19-%EC%B9%B4%EB%93%9C%ED%82%A4) | Item |
| Flash | [[UE5] 공포게임 MIZI 개발 일지 #20: 플래시](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-20-%ED%94%8C%EB%9E%98%EC%8B%9C) | Item |
| 아이템 스포너(2) | [[UE5] 공포게임 MIZI 개발 일지 #21: 아이템 스포너(2)](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-21-%EC%95%84%EC%9D%B4%ED%85%9C-%EC%8A%A4%ED%8F%AC%EB%84%882) | Level Design |
| HUD | [[UE5] 공포게임 MIZI 개발 일지 #22: UI Material](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-22-UI-Material) | UI |
| 중간 발표 | [[UE5] 공포게임 MIZI 개발 일지 #23: 중간 발표](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-23-%EC%A4%91%EA%B0%84-%EB%B0%9C%ED%91%9C) |  |
| Crouch | [[UE5] 공포게임 MIZI 개발 일지 #24: Crouch](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-24-Crouch) | Character |
| 총 | [[UE5] 공포게임 MIZI 개발 일지 #25: Rifle](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-25-Rifle) | Item |
| 터미널(1) | [[UE5] 공포게임 MIZI 개발 일지 #26: Terminal(1)](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-26-Terminal1) | Terminal |
| Time Widget | [[UE5] 공포게임 MIZI 개발 일지 #27: Time Widget](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-27-Time-Widget) | UI |
| Weight System | [[UE5] 공포게임 MIZI 개발 일지 #28: Weight System](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-28-Weight-System) | Character, UI |
| Level Design(1) | [[UE5] 공포게임 MIZI 개발 일지 #29: Level Design(1)](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-29-Level-Design1) | Level Design |
| HUD | [[UE5] 공포게임 MIZI 개발 일지 #30: HUD Redesign](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-30-HUD-Redesign) | UI |
| Spider | [[UE5] 공포게임 MIZI 개발 일지 #31: Spider](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-31-Spider-b2fcimpg) | Enemy |
| 낙하 대미지 | [[UE5] 공포게임 MIZI 개발 일지 #32: 낙하 대미지](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-32-%EB%82%99%ED%95%98-%EB%8C%80%EB%AF%B8%EC%A7%80) |Character |
| Mask | [[UE5] 공포게임 MIZI 개발 일지 #33: Mask](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-33-Mask) | Enemy |
| 오브젝트 풀(액터 풀) | [[UE5] 공포게임 MIZI 개발 일지 #34: Actor Pool](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-34-Actor-Pool) | Gun |
| SMG | [[UE5] 공포게임 MIZI 개발 일지 #35: SMG](https://velog.io/@woolucy/UE5-%EA%B3%B5%ED%8F%AC%EA%B2%8C%EC%9E%84-MIZI-%EA%B0%9C%EB%B0%9C-%EC%9D%BC%EC%A7%80-35-SMG) | Gun |

