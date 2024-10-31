# [UE5] 공포게임 MIZI 개발 일지 #1: 랜덤맵 생성기(1)
# 서두
---
다음 프로젝트를 하면 꼭 넣어보고 싶었던 요소 중 하나인 랜덤맵을 구현해보았다.
BSP나 MST 같은 랜덤맵 생성에 자주 쓰이는 알고리즘을 보면서 시간이 많이 소요될 것이라 판단했는데, 다른 방식을 채택하면서 놀랍게도 하루 안에 다 끝났다! (근데 문서화가 더 오래걸리는.)

앞서 말했듯이ㅡ 랜덤맵 생성으로 여러 자료들을 찾아봤더니 BSP 방식이나 MST 방식을 많이 사용하는 것을 확인했다.
튜토리얼도 많은 편이었고.
하지만, 내가 원하는 바와 조금 달랐다. 내가 원하는 건 정해진 방 프리셋이 여러 종류가 있고, 이 프리셋들로만 구성된 랜덤맵을 구현하는 것이었다.
그러니까 각 방의 사이즈가 규격화된, 혹은 통제된 형태의 랜덤맵이었으면 좋겠다는 생각으로 랜덤맵 생성 알고리즘을 찾아봤다. 사실 더 BSP나 MST보다 더 편리한 방법이 있을 것이라고 생각하기도 했다. ^^

그중 발견한 것이 랜덤 워크 방식이다.

" 무작위 행보(無作爲行步, 영어: random walk 랜덤 워크) 또는 취보(醉步, drunkard walking)는 수학, 컴퓨터 과학, 물리학 분야에서 임의 방향으로 향하는 연속적인 걸음을 나타내는 수학적 개념이다. 

랜덤 워크에 대한 위키피디아의 설명이다. 일단 랜덤맵 생성기를 다 만들고 나서 이게 대체 뭔 알고리즘일까 고민하다가 이게 내 랜덤맵을 가장 잘 표현한 방식인 것 같다는 결론을 내렸다.

각 방마다 최소 한개의 다음 방으로 이어질 수 있는 출구가 있고, 다음 방을 랜덤으로 스폰시키면서 전체 맵을 완성시키자는 아이디어로부터 시작했다. 복도 역시 방의 일종으로 본다면 방과 방 사이를 잇는 것도 어려운 일이 아닐 것이다.

오늘의 포스팅에서는 일단 블루프린트로 랜덤맵 생성기를 만드는 법을 설명해둘 것이다.
앞으로 있을 대부분의 기능들은 블루프린트로 먼저 만들어보고 C++로 포팅하는 순서로 만들어질 것이다.
다른 이유가 있는 건 아니고, 블루프린트를 통해 생각한 로직이 맞는지 확인하는 절차가 굉장히 효율적이기 때문이다...! 그리고 블루프린트도 애써 만들었는데 컨텐츠 브라우저에만 있으면 내가 슬프니까...

# BP_MasterRoom
---
![alt text](/Devlog/image/01/image.png)

Actor를 상속받아서 마스터룸과 랜덤맵생성기 블루프린트를 만들었다.
이 마스터룸을 상속받아서 방 프리셋을 만들 것이다.

![alt text](/Devlog/image/01/image-2.png)
마스터룸의 경우는 생성의 진행 방향을 표시하기 위한 Arrow Component(빨간색)와 생성된 방이 서로 충돌되는지 확인하기 위한 Box Collison Component를 가지고 있다.
실질적인 랜덤맵 생성 로직은 RandomMapGenerator에서 이루어질 것이다.
맵에는 RandomMapGenerator만 끌어다 놓으면 얘가 다 알아서 해줄 수 있게 구현할 것이다.

![alt text](/Devlog/image/01/image-3.png)
마스터룸을 상속받아 각 방의 프리셋을 생성한다.
위와 같은 방식으로 ExitsFolder에는 다음 방을 스폰시킬 수 있는 출구를 표시하는 Arrow Component(파란색)를 위치시킨다.
위 사진에서는 동, 서, 북 방향으로 3개의 출구를 표시했지만, 생성된 위치인 입구를 제외하고
나머지 출구의 여부는 선택적이다.
파란 화살표 위치에 다음 방을 스폰시킬 것이기 때문에 z좌표에 유의해야한다.
이 화살표, 즉 출구 위치는 ExitsFolder에 넣어 관리된다.

![alt text](/Devlog/image/01/image-4.png)
같은 방식으로 복도를 만들어줄 수 있다.
복도 또한 하나의 방으로 보는 아이디어에서 시작했기 때문에, 똑같이 출구를 표시하고 다른 방과의 충돌을 확인할 Collision Component가 있으면 된다.

![alt text](/Devlog/image/01/image-5.png)
2층 또한 구현이 가능하다. 대신 다음에 스폰될 방이 2층에 붙어야 하므로 Arrow의 위치도 2층에 위치시켜야함을 
잊지 않아야 한다.
다시 말해, 출구 Arrow의 위치만 신경쓰면 방의 크기나 종류가 다른 프리셋을 무궁무진하게 만들 수 있다는 뜻이다.

# BP_RandomMapGenerator
---
이제부터는 랜덤맵 생성기에서 본격적으로 방을 스폰시키는 로직을 구현해보자.
## Spawn Start Room
![alt text](/Devlog/image/01/image-1.png)
첫번째 방을 스폰하면서 생성기가 작동하기 시작한다.
첫번째 방은 단순히 랜덤맵 생성기에 위치에 스폰될 것이다. 
SpawnActor by Class 노드를 사용해 첫번째 방을 스폰시킨 뒤, 첫번째 방의 출구 목록을 가져와 
Exits List에 넣고 있다.
앞으로 이 Exits List에서 랜덤으로 출구를 가져와 그 위치에 방을 스폰시킬 것이다.

## Spawn Next Room
![alt text](/Devlog/image/01/image-6.png)
이제 첫번째 방에서 얻은 출구의 위치들 중 임의의 출구를 뽑을 차례이다.
내가 구현하고 있는 랜덤맵 생성기의 경우, 시드 시스템에 기반하고 있기 때문에 Stream을 통해 
랜덤 요소를 뽑고 있다.
지금은 방을 생성하는 것에 집중하고, 시드 시스템을 추후에 다시 살펴보자.
시드 시스템에 의해 선택된 출구는 Selected Exit Point가 된다.
선택된 출구의 위치에 또다시 시드 시스템을 통해 임의의 방 프리셋을 스폰시킨다.

![alt text](/Devlog/image/01/image-7.png)
이런 식으로 미리 방 프리셋을 설정해두었다.
이렇게 만들어 둔 Room List에서 Random Array Item From Stream 노드를 통해
스폰될 다음 방의 종류를 랜덤하게 선택할 수 있다.

이후, 생성된 방을 Latest Room으로 설정하고, Check for Overlap 이벤트를 호출한다.

Check for Overlap 함수는 랜덤맵 생성기에 필수적이다.
말 그대로 랜덤한 위치에 랜덤한 방을 스폰시키고 있기 때문에, 이미 생성된 방 위에 또다른 방이 겹쳐서 생성될 수 있기 때문이다.
따라서, 새로 생성된 방이 기존의 영역과 겹치는지 확인하고 겹친다면 방을 파괴하는 작업이 필요하다.

## Add Overlapping Rooms to List
![alt text](/Devlog/image/01/image-8.png)
일단 방이 겹치는지 확인하는 이벤트에서 Add Overlapping Rooms to List 이벤트를 호출한다.
해당 이벤트부터 확인해보자.

![alt text](/Devlog/image/01/image-9.png)
Add Overlapping Rooms To List 이벤트가 하는 일은 단순하다. 
가장 최근 추가된(스폰된) 방에 겹쳐진 컴포넌트들을 죄다 Overlapped List에 넣는 일을 한다.
이 이벤트 이후 Overlapped List가 비어있지 않다면, 방금 만든 방이 다른 방과 겹친다는 의미일 것이다.

## Check for Overlap
![alt text](/Devlog/image/01/image-10.png)
다시 Check for Overlap 이벤트로 돌아가보자
앞서 말했듯이 Overlapped List가 비어있지 않다는 것은 막 만든 방이 이미 존재하는 방과 겹치고 있다는 뜻이다.
따라서, Not Empty 노드의 결과가 true라면 Overlapped List를 비우고 막 만든 방을 파괴(Destroy)한다.
여기서 그만두면 로직이 끝나버리니 다시 Spawn Next Room을 호출해서 위 과정을 반복시킨다.

![alt text](/Devlog/image/01/image-11.png)
반대로 Overlapped List가 비어있다는 것은 막 만든 방이 겹치지 않고 성공적으로 스폰됐다는 뜻이다.
Cur Room Amount에서 1를 빼준다. 여기서 Cur Room Amount는 현재 생성기에 의해 스폰될 방의 개수를 의미한다. 
예를 들어, 방 20개를 만들고 싶다면 Cur Room Amount의 초기값을 20으로 해두면 된다.
로직이 돌아가고 성공적으로 1씩 빠지면서 0이 될때까지 과정을 반복하면 결과적으로 20개의 방이 스폰된다.

선택된 출구에서 성공적으로 방이 스폰되었으므로 출구 리스트(Extits List)에서 선택된 출구(Selected Exit Point)를 빼줘야 해당 위치에 다시 방이 스폰되지 않을 것이다.

앞서 봤던 Spawn Start Room과 마찬가지로 방 스폰에 성공했다면, 새로운 방의 출구 위치들을 Exits List에 넣어줘야 한다.

![alt text](/Devlog/image/01/image-12.png)
현재 생성되어야 할 방의 개수가 0개보다 많다면, Spawn Next Room을 호출해서 위 과정을 반복하면된다.
생성될 방의 개수가 0이 될때까지!
중간에 특별방을 필수적으로 생성하는 로직을 넣어두었지만 이는 그닥 중요하지 않아서 넘어간다.

![alt text](/Devlog/image/01/image-13.png)
더 이상 생성될 방의 개수가 없다면 랜덤맵 생성이 끝났다.
하지만, 가장자리에 있는 방들은 출구가 뚫려 있는 상태이기 때문에 그들을 막아주는 과정이 필요하다.
따라서 Close Holes 이벤트를 호출하고 있다.

## Close Holes
![alt text](/Devlog/image/01/image-14.png)
해당 이벤트에서는 Exits List를 for로 돌고 있다.
이미 랜덤맵 생성이 끝난 시점이기 때문에 Exits List에는 선택되지 않은 출구들만 남아있다.
이 선택되지 않은 출구들을 막아야하니, 해당 위치에 벽 블루프린트를 스폰시킨다.

![alt text](/Devlog/image/01/image-15.png)
벽 블루프린트는 말 그대로 스태틱 메쉬 컴포넌트만 있는 '벽'이다.

## Check for Overlap
![alt text](/Devlog/image/01/image-16.png)
다시 Check for Overlap 이벤트에서 랜덤맵을 마무리하는 부분으로 돌아와보면, 갑자기 타이머를 Clear하고 있다.
사실, 해당 타이머는 앞서 작성했지만 위 과정을 모두 설명하고 타이머 관련 이벤트를 확인하는 게 효율적일 것 같아 후술하게 됐다.

## Start Dungeon Timer
![alt text](/Devlog/image/01/image-17.png)
랜덤맵 생성기에서 타이머는 필수적이다.
예를 들어, 막 생성된 방이 계속해서 다른 방과 겹쳐 Spawn Next Room이 무한 호출된다면?
그렇다면 슬프게도 플레이어는 게임을 절대 시작할 수 없을 것이다. 맵이 없는데 어떻게 게임을 해.
그래서 타이머를 설정해 일정 시간이 지나고 나서도 맵이 모두 완성되지 않았다면 레벨 자체를 새로 여는 로직을 추가했다.
타이머에서 Max Dungeon Time이 넘어가도록 게임 시간이 진행된다면 OpenLevel 노드를 통해 레벨을 다시 열도록 설정하고 있다.
모든 것을 우연에 기대는 Random Walk 방식으로 사용하고 있기 때문에 타이머가 필수적으로 있어야 한다.

## Check for Overlap
![alt text](/Devlog/image/01/image-18.png)
다시 랜덤맵 마무리로 돌아와보자.
이미 랜덤맵 생성에 성공했으니, 이젠 타이머를 보내줄 때다.
Clear를 통해 타이머를 초기화시키고, 성공했으니 기분이 좋으니까 String도 한번 출력시킨다 >~<


## Set Seed
여기서 끝이 아니다!
맨 앞에서 바쁘다고 넘어간 시드 시스템, 미래의 나를 위해 설명해두겠다.

![alt text](/Devlog/image/01/image-19.png)
이름만 거창했지 대단한 것은 아니고 초기화 해둔 Seed가 -1이면 매번 다른 맵이 생성되고 -1이 아니면 정해진 Seed에 기반한 맵이 생성되는 것이다.
Seed Random Stream 노드가 이미 있기 때문에 쉽게 구현했다.
이렇게 시드를 미리 설정해서 Spawn Next Room 이벤트에서 Seed에 기반한 위치와 방 종류를 선택해 스폰시킬 수 있는 것이다.

## Event BeingPlay
![alt text](/Devlog/image/01/image-20.png)
이제 BeginPlay에서 시드를 정해주고, 앞서 구현해둔 이벤트들을 차례대로 호출해주면된다.
타이머 맞추는 것도 잊으면 안 된다!

# 참고자료
---
- [How To Create Epic Procedural Dungeons In Unreal Engine 5](https://youtu.be/4ddbnQIuwAM?si=dTouLceg2JWsd8fd)