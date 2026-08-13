# Placement Legalization (Abacus)
使用 Abacus 演算法實作的 placement legalization。目標是在不與 blockages 重疊、並對齊 row/site grid 的前提下，將 global placement 的 cells 合法化，並盡量降低 displacement cost。

> spec 可參考 `CS6135_HW3_spec.pdf`

## 使用說明 (Usage)

### 1. 編譯
請確保環境中已具備支援 C++17 的編譯器（如 `g++`）與 `make`。  
在專案根目錄下執行以下指令：

```bash
make
```

編譯完成後會產生執行檔 `main`。

### 2. 執行（預設 testcase：ibm01）
不帶參數預設會跑 `ibm01`：

```bash
./main
```

### 3. 執行指定 testcase（只輸入 case name）
只輸入 case name（對應 `testcase/<case>/` 資料夾名稱），程式會到 `testcase/<case>/` 底下尋找 `.aux`，並依 `.aux` 內容讀取 `.nodes/.pl/.scl`：

```bash
./main adaptec1
./main ibm07
./main sample
```

目前提供的 case（見 `testcase/`）：
- `sample`
- `adaptec1`
- `adaptec3`
- `ibm01`
- `ibm07`
- `ibm09`

### 4. 輸出檔案
程式會在 `output/` 目錄輸出結果檔：

- 輸出路徑：`output/<case>.pl`（例如：`output/ibm01.pl`）

輸出格式（`.pl`）：

```txt
<name> <x> <y> : N
<name> <x> <y> : N /FIXED
...
```

其中：
- `N` 表示 movable cell
- `N /FIXED` 表示 blockage / terminal（固定物件）

### 5. 清理
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
```bash
./verifier/verify [aux file] [result file]
```

範例（以 `ibm01` 為例，`.aux` 檔名依 testcase 實際內容為準）：

```bash
./verifier/verify testcase/ibm01/ibm01-cu85.aux output/ibm01.pl
```

若正常會顯示Verfication passed.  
> 使用前要先確認此檔案權限是否可執行
