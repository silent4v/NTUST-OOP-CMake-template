如果自己沒有額外實作其他內容，可以使用此文件的Demo流程。  
因為遊戲具有隨機因素，所以Demo流程步驟僅供參考，不一定能完全一樣，但還是能夠完成評分項目。  
自己規劃Demo流程，但若因此沒有辦法完成評分項目，則無法獲得該分數，  
如果Demo項目具有相依性且有不可回朔性，可以重開程式進行該部分Demo，  
例如金錢已花完但是沒有其他方法獲得金錢，造成需要項目無法購買，可以重新運行程式進行Demo評分，  
基本只要不修改程式碼，重新運行程式進行Demo評分是可以的。  
但若是因為Bug的不明確原因，導致Crash或者無法進行Demo，則視為Bug，Bug情況會另外討論扣分。  
1. 角色初始數值
   - Role1: 根據規格隨機
      以下數值為此範例Demo時的數值
      - Hp: 33
      - Focus:3
      - Physical Attack: 13
      - Physical Defense: 7
      - Magic Attack: 16
      - Magic Defense: 0
      - HitRate: 53
      - Speed: 38
   - Role2:
      - Hp:26
      - Focus: 6
      - Physical Attack: 24
      - Physical Defense: 3
      - Magic Attack: 0
      - Magic Defense: 3
      - HitRate: 80
      - Speed: 80
   - Role3:
      - Hp:26
      - Focus: 6
      - Physical Attack: 24
      - Physical Defense: 2
      - Magic Attack: 0
      - Magic Defense: 4
      - HitRate: 1
      - Speed: 90
2. 地圖初始  
   - 地圖大小: 140x50
   - 座標軸: (0,0)為地圖左上角，x軸向右，y軸向下，地圖右下角座標為(139,49)
   - 地塊符號: 
     - <span style="font-size:1.5rem; padding: 0 0.5rem;background: gold;">
            <font color="#000000">.</font>
        </span>: 可以通行的道路，背景顏色代表Road，前景符號'.'代表可以通行
     - <span style="font-size:1.5rem; padding: 0 0.5rem;background: grey;"></span>: 不可通行的障礙物
     - <span style="font-size:1.5rem; padding: 0 0.5rem;background: wheat;"><font color="#000000">$</font></span>: 商店
     - <font color ="#FF0000">E</font>: 敵人
     - <font color ="#FFFFFF">1、2、3</font>: 角色
     - <font color ="#FFFFFF">T</font>: Tent
     - <font color ="#FFFFFF">?</font>: 隨機事件
   - 地圖物件位置:
     - Role1: Position(135, 47)
     - Role2: Position(50, 11)
     - Role3: Position(50, 12)
     - Wall:
        - Position(136, 47),
        - Position(136, 48),
        - Position(136, 49),
        - Position(137, 47),
        - Position(138, 47),
        - Position(139, 47)
    - 隨機事件: Position(138, 48)
    - 商店: Position(133, 45)
    - Enemy1: 
      - Position(137, 49)
      - Attribute:
        - Hp: 200
        - Physical Attack: 100
        - Physical Defense: 50
        - Magic Attack: 0
        - Magic Defense: 0
        - HitRate: 100
        - Speed: 50
    - Enemy2: 
      - Position(50, 10)
      - Attribute:
        - Hp: 100
        - Physical Attack: 10
        - Physical Defense: 0
        - Magic Attack: 0
        - Magic Defense: 0
        - HitRate: 100
        - Speed: 50
    - Enemy3: 
      - Position(50, 9)
      - Attribute:
        - Hp: 20
        - Physical Attack: 10
        - Physical Defense: 0
        - Magic Attack: 0
        - Magic Defense: 0
        - HitRate: 100
        - Speed: 50
    - Enemy4:
      - Position(50, 8)
      - Attribute:
        - Hp: 20
        - Physical Attack: 10
        - Physical Defense: 0
        - Magic Attack: 0
        - Magic Defense: 0
        - HitRate: 100
        - Speed: 50
    - Silme:
        - Attribute:
            - Hp: 50
            - Physical Attack: 1
            - Physical Defense: 0
            - Magic Attack: 0
            - Magic Defense: 0
            - HitRate: 100
            - Speed: 50
        - 裝備:
            - Weapon: DirtyStuff 
              - Part: 武器
              - Skills: [NormalAttack(物理)、Destory]
              - Description: 攻擊命中時，破壞目標裝備並且賦予Debuff::Poisoned。 
3. 測試不可通行地塊，與不超出地圖範圍
    操作Role3移動，測試以下項目，地圖大小不嚴格檢查，如果能做出捲動效果基本就能有第1項分數，  
    第2項因為地圖初始將大部分物件都放在了，Role3附近，基本可以看到所有物件。  
    如果地圖探索階段沒有畫出Role的Stat資訊，能夠在其他的地方看到也可以，但必須要顯示Role當下可以movement point。  
    戰鬥階段一定要有Role的Stat資訊。  
    獲得分數:  
    1. UI: 140 * 50 的 Map: 2
    2. UI: 正確標記所有的道路、Role、Enemy、商店、不可通行地塊: 2
    3. UI: 所有Role的Stat資訊: 2
    4. UI: 所有Role的裝備資訊: 2
    5. 使用者控制的Role可以在地圖上移動，不能移動到地圖外、不可通行地塊: 6
4. 移動到商店    
    與商店互動，購買物品，以及測試因金錢不足無法購買
    - 商店物品清單 (後面Demo需要用到的物品): 
      - TeleportScroll * 2
      - Godsbeard * 3
      - GoldenRoot * 3
      - Tent * 2
      - WoodenSword * 1
      - Hammer * 1
      - MagicWand * 1
      - Shoes * 1
      - PlateArmor * 1
      - Bracelet * 1
    - 獲得分數:
      - 商店物品清單: 1
      - 購買物品: 2
        包含金錢系統，如果商品超過價格，則無法購買  
5. 使用TeleportScroll 
   測試不能傳送到不可通行地塊、地圖外與互動物件，  
   之後傳送到隨機事件旁邊。  
    - 獲得分數:
      - Items::TeleportScroll: 3
6. 觸發隨機事件 
   自行定義隨機事件，例如: 獲得金幣、獲得經驗值、獲得道具等等，  
   觸發隨機事件後，隨機事件消失。
    - 獲得分數:
      - 觸發隨機事件: 1
7. 與敵人("Enemy1")互動 
   列出互動行為，
   選擇退出回到上一個地塊，  
   再次與敵人互動，並且選擇戰鬥。
    - 獲得分數:
      - 敵人行為清單: 2
      - 進入戰鬥: 2
      - 返回: 2
      - 顯示地圖物件與互動: 6  
        到這裡可互動物件基本上都有了
8. 戰鬥   
   使用普通攻擊攻擊敵人，  
   使用技能Flee，並且使用專注使逃跑必定成功。  
   如果普通攻擊成功，則再次與敵人戰鬥，並且繼續上述操作，直到普通攻擊失敗為止。  
    - 獲得分數:
      - 逃跑: 5
9.  再次與敵人互動 
    選擇戰鬥，並且使用技能普通攻擊，直到敵人進行反擊，打倒玩家，  
    回到地圖探索階段，因為玩家死亡，之後此玩家無法再次進行任何操作，必須跳過。
    - 獲得分數:
      - 普通攻擊: 5
10. 穿戴裝備 
    Role2裝備MagicWand、PlateArmor、Shoes，  
    Role3裝備Hammer、WoodenShield、bracelet，  
    測試不能重複穿戴相同部位道具，並且套用裝備效果，使否套用效果依生物屬性使否變化來判斷。  
    - 獲得分數:
      - 穿戴裝備，相同部位道具不能重複穿戴，並且套用裝備效果: 5
      - Passive::Run: 2
        因為可能無法測試到，只要不要出現movement point為0的情況即可  
11. Role3使用GoldenRoot回復專注力 
    - 獲得分數:
      - GoldenRoot: 1        
12. 與敵人("Enemy2"、"Enemy3"、"Enemy4")互動 
    Role2、Role3與"Enemy2"、"Enemy3"、"Enemy4"戰鬥，
    需使用或觸發技能，包含{Shock-Blast、Heal、Provoke、Hammer-Splash}，與使用道具回復HP與專注，打倒敵人  
    - 動作列表: 會因為隨機因素而有所不同，不一定能完全複現
      - Role2: 使用Skill::ShockBlast，使用3個專注，對全部敵人造成5點傷害  
        $magical ATK(10)*0.5=5$
      - Role2: 使用Skill::ShockBlast，使用3個專注，對全部敵人造成5點傷害  
      - Enemy2: 使用Skill::NormalAttack
        - 第一次攻擊Role2，則造成5點傷害  
            $physical ATK(10)*(1-{26 \over 76}-Skill::Fortify(0.1)) = 5$
        - 攻擊Role2，Skill::Fortify冷卻，則造成6點傷害  
            $physical ATK(10)*(1-{26 \over 76}) = 6$
        - 攻擊Role3，則造成7點傷害  
            $physical ATK(10)*(1-{17 \over 67}) = 7$
      - Enemy3: 使用Skill::NormalAttack
      - Enemy4: 使用Skill::NormalAttack
      - Role3: 使用NormalAttack 攻擊Enemy2，使用4點專注
        造成28點傷害，對Enemy3、Enemy4造成14點傷害
        Enemy3、Enemy4死亡
        Enemy2附加Debuff::Dizziness
      - Role2: 使用道具GoldenRoot回復專注
      - Enemy2: 因為Debuff::Dizziness跳過回合
      - Role2: 使用Skill::Heal 回復有扣到HP的隊友或自己
      - Role3: 使用道具GoldenRoot回復專注
      - Role2: 使用Skill::NormalAttack 攻擊Enemy2
      - Enemy2: 使用Skill::NormalAttack
      - Role3: 使用Skill::Provoke 攻擊Enemy2，使用專注使得Provoke必定成功
      之後持續選擇動作將Enemy2打倒，結束戰鬥
    - 獲得分數:
      - Combat 行動順序: 5
      - 正確執行戰鬥機制: 6
      - 使用道具並且使用後消失 (戰鬥中：Role使用道具後直接進入 TurnEnd 階段): 3
      - Skill::Heal: 3
      - Skill::Provoke: 4
      - Skill::Shock-Blast: 6
      - Skill::Hammer-Splash: 6
      - Debuff::Angry: 2
      - Debuff::Dizziness: 4
        擁有此Debuff的生物跳過他的回合
        機率不嚴格測試，有顯示出此Debuff即可。
      - 正確套用與消失: 3
      - Godsbeard: 1
      - GoldenRoot: 1
13. Role2更換武器為WoodenSword
14. 使用Tent將狀態回復滿 
    - 獲得分數:
      - Tent: 5
15. 與敵人("Slime")互動並進入戰鬥 
    - Role2使用技能Speedup，對象為Role3，  
    - Role3逃跑，並且使用專注使逃跑必定成功，  
    - Silme攻擊，並且因未攜帶武器DirtyStuff，賦予Debuff::Poisoned與破壞裝備，  
    繼續進行戰鬥直到Silme發動第二次攻擊，
    因為裝備PlateArmor的被動技能效果，第一次攻擊具有減傷效果，或者是裝備被破壞掉，
    因此第二次攻擊造成傷害，應該要比第一次攻擊高，
    另外因為Silme的攻擊造成Debuff::Poisoned，因此Role2的回合開始時應該要受到Debuff::Poisoned的傷害。
    - 獲得分數:
      - 使用技能與正確選擇對象 (技能要進入冷卻，冷卻會在該Entity TurnStart時-1)
      - Skill:Speedup: 2
      - Skill:Destory: 2
      - Buff::Speedup: 2
      - Debuff::Poison: 4
16. Demo結束，之後助教可能會對專案提出問題，以及隨意測試
    如果程式出現問題，???(待討論扣分部分)

根據上述Demo流程，可以完成以下評分項目:  
- 基本項目
    - UI
      - 140 * 50 的 Map: 2
      - 正確標記所有的道路、Role、Enemy、商店、不可通行地塊: 2
      - 所有Role的Stat資訊: 2
      - 所有Role的裝備資訊: 2
    - Map
      - 使用者控制的Role可以在地圖上移動，不能移動到地圖外、不可通行地塊: 6
      - 顯示地圖物件與互動: 6
    - Object
      - Shop, 商店物品清單，列出可以購買的物品與價格: 1
      - Shop, 購買物品: 2
      - Enemy, 行為清單，列出可以進行的行為: 2
      - Enemy, 可以進入戰鬥: 2
      - Enemy, 撤退，回到上一個地塊: 2
      - Event, 隨機事件(自行定義地圖符號與內容，放置於Rect上): 1
    - Combat
      - 行動順序，每一個Turn Entity的行動順序正確: 5
      - 正確執行戰鬥機制，傷害計算、正確使用專注力和結束戰鬥，Entity死亡後必須正確標記，且跳過死亡Entity's Turn: 6
      - 使用技能與正確選擇對象 (技能要進入冷卻，冷卻會在該Entity TurnStart時-1): 6
      - 使用道具並且使用後消失 (戰鬥中：Role使用道具後直接進入 TurnEnd 階段): 3
    - Skill
      - Attack: 5
      - Flee: 5
    - Items
      - Godsbeard: 1
      - GoldenRoot: 1
      - TeleportScroll: 3
      - Tent: 5
- 進階項目
  - Equipment
    - Role能穿戴裝備，部位不能重複，並且正確套用裝備效果: 5
  - Skill
    - Heal: 3
    - Speedup: 2
    - Provoke: 4
    - Shock-Blast: 6
    - Hammer-Splash: 6
    - Run: 2
    - Destory: 2
- Buff/Debuff
  - Buff & Debuff能正確套用與消失: 3
  - Speedup: 2
  - Angry: 2
  - Dizziness: 4
  - Poison: 4
- 其他項目
  - 不能將整張地圖一次畫進整個視窗內(大地圖，需要完成捲動功能): 3
  - 戰鬥支援: 1