# Fix Outline Slicing Floorplan
使用 **Wong-Liu slicing floorplanning**（NPE + Simulated Annealing）產生切割式（slicing）floorplan，在滿足外框限制（由 dead space ratio 推導）後，再進一步最佳化 wirelength；輸入為 `.hardblocks` / `.pl` / `.nets`，輸出為 `.floorplan`。

> spec可參考 `CS6135_HW2_spec.pdf`

## 使用說明 (Usage)

### 1. 編譯
請確保環境中已具備支援 C++17 的編譯器（如 `g++`）與 `make`。  
在專案根目錄下執行以下指令：

```bash
make
```

編譯完成後會產生執行檔 `main`（並建立/更新 `build/` 物件檔）。

### 2. 執行（預設 testcase：n100）
不帶參數會使用 `testcase/n100.hardblocks` / `testcase/n100.pl` / `testcase/n100.nets`，且 dead space ratio 預設為 `0.2`：

```bash
./main
```

也可以使用 Makefile 的 `run` 目標：

```bash
make run
```

### 3. 執行指定 testcase（輸入檔名 + dead space ratio）
程式參數格式如下（兩個參數必須同時提供）：

```bash
./main <case> <dead_space_ratio>
```

範例：

```bash
./main n200 0.2
# 會讀取：
# testcase/n200.hardblocks
# testcase/n200.pl
# testcase/n200.nets
```

若使用 `make run`，請同時指定 `CASE` 與 `RATIO`（只給其中一個會直接報錯）：

```bash
make run CASE=n200 RATIO=0.2
```

說明：
- `<case>`：測資檔名主體（不含路徑與副檔名）
- `<dead_space_ratio>`：`double`，用來推導外框限制（程式內部會用 `sqrt(block_area * (1 + dead_space_ratio))` 計算 `limit`）

### 4. 輸入檔案
程式會固定從 `testcase/` 讀取同名三個檔案（缺一不可）：
- `testcase/<case>.hardblocks`
- `testcase/<case>.pl`
- `testcase/<case>.nets`

### 5. 輸出檔案
程式會在 `output/` 目錄輸出結果檔：
- 輸出路徑：`output/<case>.floorplan`（例如：`output/n200.floorplan`）

輸出格式：

```txt
Wirelength <number>
Blocks
<blockName> <x> <y> <rotate(0|1)>
...
```

其中 `(x, y)` 是 block 左下角座標，`rotate=1` 表示該 block 在解中被旋轉（寬高對調）。

### 6. 清理
清除編譯產物與輸出資料夾：

```bash
make clean
```

## 專案來源
來自NTHU CS Physical Design Automation 助教的Github (連結如下)  
[NTHU CS Physical Design Automation Github](https://github.com/EricLu1218/Physical_Design_Automation/tree/main)  

### 助教提供的工具
#### 1. Output格式驗證
可利用verifier資料夾內的verify檔案來驗證，使用方法如下  
```
Usage ./verifier/verify <testcase.hardblocks> <testcase.nets> <testcase.pl> <testcase.floorplan> <dead_space_ratio>
```
若正常會顯示OK  
> 使用前要先確認此檔案權限是否可執行
