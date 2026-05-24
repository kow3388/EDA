# Two-Way Min-Cut Partition

一個使用 Fiduccia-Mattheyses (FM) 演算法實作的 Two-Way Min-Cut Partition 程式。本專案為電子設計自動化 (EDA) 領域中常見的演算法，用於將電路單元 (Cells) 分割成兩個集合，在滿足平衡條件的情況下，最小化跨越兩個集合的連接線 (Nets) 數量（Min-Cut）。

## 使用說明 (Usage)

### 1. 編譯專案 (Installation)
請確保環境中已具備支援 C++17 的編譯器 (如 `g++`) 與 `make` 工具。
在專案根目錄下執行以下指令進行編譯：

```bash
make
```
編譯完成後，會產生一個名為 `main` 的執行檔。

### 2. 執行程式
執行編譯出的程式，即可開始進行 Partition：

```bash
./main
```

> **備註：** 目前的輸入測資路徑已寫死在 `src/main.cpp` 中（預設為 `./testcase/p2-1.cells` 和 `./testcase/p2-1.nets`）。若需要測試其他案例（例如 `testcase/` 底下的 `p2-2` ~ `p2-5`），請先修改 `src/main.cpp` 內的 `cell_file_path` 與 `net_file_path` 變數，然後重新執行 `make` 進行編譯。

### 3. 開發與清理指令 (Development)
- **清除編譯檔案**：若要清除產生出的 `.o` 執行檔及建置目錄，請執行：
  ```bash
  make clean
  ```
- **預設測資**：專案內的 `testcase/` 目錄包含多個層級的測試資料  
- **測資產生**: 助教有提供附帶 `genTestcase.rb` 腳本可供生成測試資料使用。  

# 專案來源
來自NTHU CS Physical Design Automation 助教的Github (連結如下)  
[NTHU CS Physical Design Automation Github](https://github.com/EricLu1218/Physical_Design_Automation/tree/main)  
