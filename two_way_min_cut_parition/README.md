# Two-Way Min-Cut Partition
使用 Fiduccia–Mattheyses (FM) 演算法實作的 Two-Way Min-Cut Partition。目標是在滿足兩群大小平衡限制下，最小化跨越兩群的 nets 數量（cut size）。  

> sepc可參考CS613500_HW2_spec.pdf  
## 使用說明 (Usage)

### 1. 編譯
請確保環境中已具備支援 C++17 的編譯器（如 `g++`）與 `make`。  
在專案根目錄下執行以下指令：  

```bash
make
```
編譯完成後會產生執行檔 `main`。  

### 2. 執行（預設 testcase：p2-1）
不帶參數會使用 `testcase/p2-1.cells` 與 `testcase/p2-1.nets`：

```bash
./main
```

若 Makefile 有提供 `run` 目標，也可：

```bash
make run
```

### 3. 執行指定 testcase（只輸入檔名）
只輸入檔名（不含路徑與副檔名），程式會自動從 `testcase/` 讀取對應的 `.cells` / `.nets`：

```bash
./main p2-3
# 會讀取：
# testcase/p2-3.cells
# testcase/p2-3.nets
```

### 4. 輸出檔案
程式會在 `output/` 目錄輸出結果檔：

- 輸出路徑：`output/<case>.out`（例如：`output/p2-3.out`）

輸出格式：

```txt
cut_size <number>
A <count>
<cellName>
<cellName>
...
B <count>
<cellName>
<cellName>
...
```

### 5. 清理
清除編譯產物與輸出資料夾：

```bash
make clean
```

# 專案來源
來自NTHU CS Physical Design Automation 助教的Github (連結如下)  
[NTHU CS Physical Design Automation Github](https://github.com/EricLu1218/Physical_Design_Automation/tree/main)  

## NTHU CS Physical Design Automation 助教提供的function
### 1. Testcase生成
可利用testcase資料夾的genTestcase.rb來生成testcase，使用方法如下  
```
# ruby v1.9.3
Usage: ruby genTestcase.rb <filename> <#cells> <maxCellSize> <#nets> <maxDegree>
```

### 2. Output格式驗證
可利用verifier資料夾內的verify檔案來驗證，使用方法如下  
```
Usage ./verifier/verify <testcase.nets> <testcase.cells> <testcase.out>
```
若正常會顯示OK  
> 使用前要先確認此檔案權限是否可執行
