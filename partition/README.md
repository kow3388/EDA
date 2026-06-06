# Partition
把一個複雜的system拆解成多個小的subsystem，可以個別優化subsystem，各個subsystems之間要盡可能最小化彼此的interconnections  

## Terminologies
1. Cell  
   元件模組的定義，他的logic, area, timing, ... 例如:inv, nand  

2. Instances  
   在實務上實際做出來的cell  

3. Pin  
   cell的針腳 (比較篇定義) 通常不會包含GND, VDD  

4. Terminal/Port  
   實務上的連線端點  

5. Net  
   等電位的多個terminals的connections的集合  

6. Netlist  
   A set of nets  

7. LEF (Library Exchange Format)  
   用來描述實體設計的資訊，主要包含technology (例如metal層定義, ...) 和Cell的資訊 (例如cell size)  
8. DEF (Design Exchange Format)  
   用來描述某個design的實體實做狀態/結果，例如Die/Core的邊界, pin/ports的位置, ...  

## Kernighan-Lin Algo
經典的將圖切分成兩半讓cut size盡可能小的啟發式演算法  

### Problem formulation
給一張無相加權圖，G(V, E) 權重可代表net的成本, 線數等等  
目標是把vertex拆分成A, B兩群並最小化cut cost  
$cut cost = \sum_{(u, v) in E, u in A, v in B} W(u, v)$  

### Steps
1. Initial partition  
   隨機將所有vertex二分割成兩群  

2. 計算每個vertex的cost  
   E(v): 從v到"另一邊"的edge weight總和 (external)  
   I(v): 從v到"同一邊"的edge weight總和 (internal)  
   D(v): E(v) - I(v) (當把v換到另一邊所獲得的增益)  

3. Algo主體  
   一個iterrative會要做|V|/2次  
   從A選一個a和從B選一個b (未被locked) 使得交換的增益最大化  
   交換(a, b) (增益常見的公式 g(a, b) = D(a) + D(b) - 2\*W(a, b))  
   交換完將a和b locked  

4. Backtracking  
   做完一次iterration獲得到一組gain list = [g1, g2, g3, ... gn]  
   計算prefix sum找出最好的獲益在哪一次交換需要停止，backtrack到那步  

5. 重複步驟3和4  
   直到gain不再增加為止  

### Properties
1. 會將cut cost下降到local minimum  

2. Algo主體time complexity是$O(n^3)$  
   挑pair為$O(n^2)$，因為兩兩計算哪組pair最好  
   要做n/2次  

4. Total time complexity為$O(rn^3)$  

## Fiduccia-Mattheyses Algo
和KL algo類似，把vertex分成兩個group，盡可能減少cut size的heruistic algo  

### Problem formulation
給一張無相圖，G(V, E)  
目標是把vertex拆分成A, B兩群並最小化cut cost  
cut cost = 被cut的net數量  

### Steps
1. Initial partition  
   隨機將所有vertex二分割成兩群  

2. 計算每個vertex的gain  
   若移動此vertex會讓某個net的node全部在某一群，則gain++  
   反之gain--  
   以***bucket list***這個data struct來紀錄每個vertex的gain  

3. Algo主體  
   一個iterrative會要做n次  
   挑選一個gain值最高且未被locked住的vertex，將其移動到另一個group  
   每當移動一個vertex後更新所有和這個vertex處於相同net的所有vertex的gain值  
   移動後將vertex lock  
   因為用***bucket list***儲存gain，因此每個vertex更新gain所需的成本為O(1)  

4. Backtracking  
   做完一次iterration獲得到一組gain list = [g1, g2, g3, ... gn]  
   計算prefix sum找出最好的獲益在哪一次交換需要停止，backtrack到那步  

5. 重複步驟3和4  
   直到gain不再增加為止  

### Properties
1. 會將cut cost下降到local minimum  

2. Algo主體time complexity是$O(np)$    
   p是整個netlist中的所有vertex中的最大degree (某個vertex接到多少net)  
   因為每次要挑選出當下最佳的gain vertex所花的時間是O(p) (查看所有可能的gain有沒有vertex)  
   要做n次  

4. Total time complexity為$O(rnp)$  
