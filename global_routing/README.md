# Global Routing (A* + Rip-up and Reroute)
使用 grid-based global routing 實作 2-pin nets 的布線。以 A*（wave propagation）為基礎，搭配 rip-up and reroute 與 history cost，目標是在容量限制下盡量降低 overflow，並兼顧總 wirelength。

> spec 可參考 `CS6135_HW4_spec.pdf`

## 使用說明 (Usage)

### 1. 編譯
請確保環境中已具備支援 C++17 的編譯器（如 `g++`）與 `make`。  
在專案根目錄下執行以下指令：

```bash
make
```

編譯完成後會產生執行檔 `main`。

### 2. 執行（預設 testcase：ibm01）
不帶參數預設會跑 `ibm01`（讀取 `testcase/ibm01.modified.txt`）：

```bash
./main
```

### 3. 執行指定 testcase（只輸入 case name）
只輸入 case name（對應 `testcase/<case>.modified.txt` 的檔名前綴），程式會讀取 `testcase/<case>.modified.txt`：

```bash
./main ibm02
./main ibm04
```

目前提供的 case（見 `testcase/`）：
- `ibm01`
- `ibm02`
- `ibm04`

### 4. 輸出檔案
程式會在 `output/` 目錄輸出結果檔：

- 輸出路徑：`output/<case>.result`（例如：`output/ibm01.result`）

輸出格式（`.result`，每條 net 以 `!` 結尾）：

```txt
<net_name> <net_id>
(<x1>, <y1>, 1)-(<x2>, <y2>, 1)
(<x2>, <y2>, 1)-(<x3>, <y3>, 1)
...
!
```

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
可利用 `verifier/verify` 來驗證輸出結果（參數格式以助教工具說明為準）。

範例（以 `ibm01` 為例）：

```bash
./verifier/verify testcase/ibm01.modified.txt output/ibm01.result
```

> 使用前要先確認此檔案權限是否可執行（例如：`chmod +x verifier/verify`）
