# Floorplanning
Partition把一個大的design拆成多個block/module等等的subsystem後，floorplanning決定這些block/module怎麼去擺它  
Floorplanning算是實際有擺物件的layout非常早期階段，因此可以在早期就做一些layout的評估  

> Floorplanning和placement很相似，因為都是擺放東西，在廣義上來說是相同的事情，但大多數會把floorplanning代表擺放大的block/module等等，而placement則是擺放較小的元件例如cell等等  

> Block有分兩種，hard block表示內部實作已經完成，例如他的dimension, pin已經固定，而soft block則是還只有邏輯描述，實作尚未完成  

![flow](./img/flow.png)  

## Slicing floorplan & Slicing tree
我們可以把一個floorplan design拆解成一個樹狀結構，其中的名詞  
1. Leaf cell (block/module)  
   Tree的leaf，代表這個cell當中已經不包含其他cell (block/module)了  

2. Composite cell (block/module)  
   Tree的non-leaf，代表這個cell (block/module)還可以繼續拆解成leaf cell或其他coposite cell  

![leaf cell and composite cell](./img/leaf_and_composite.png)  

我們知道每個composite cell可以繼續拆解，其中拆解法是以horizontal或vertical方法拆解  
這種拆解的表示稱為slicing floorplan  
Slicing floorplan可以用slicing tree來表示  

> Slicing floorplan又被稱為floorplan of order 2  

![slicing tree](./img/slicing_tree.png)  

Slicing floorplan是在切的時候每一刀都切到底，也會有non-slicing floorplan，就是有一個像是循環結構的切法  
大家會盡可能的使用slicing的方法，因為這會讓後續的routing變得比較容易  

![slicing and non-slicing](./img/slicing_and_non_slicing.png)  

### Polish Expression  
將一個slicing floorplan用slicing tree表示，再用post-ordered的方式來表示  
Normalize Polish Expression (NPE): polish epxression當中沒有連續相同type的operators  

![NPE](./img/npe.png)  

### Skewed slicing tree
若整棵樹的所有composite cell的切法和它的right child切法不同，則這棵樹被稱為skewed slicing tree  

為什麼需要skewed slicing tree呢?  
因為一個slicing floorplan可能會有好幾種slicing tree畫法，若用polish expression表示，可能會造成同一種polish expression畫出不同的slicing tree，因此需要一個***規範化***的slicing tree來盡可能減少polish expression所能代表的slicing floorplan  

![skewed and non-skewed](./img/skew_and_non_skew.png)  

> NPE + skewed slicing tree會讓slicing tree唯一  

## Wong-Liu Floorplanning Algo
