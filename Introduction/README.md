# Introduction

## Digital Design Flow

1. **System Specification**   
   定義整顆晶片要做什麼（功能、效能、功耗、...），產出 Spec 文件、Block diagram、...（純需求）

2. **Architectural Design**  
   把系統拆成模組（CPU core、cache、bus、...），產出 Microarchitecture spec；spec 的實作和設計的選擇

3. **Functional & Logical Design**  
   用 RTL（Register Transfer Level）來描述行為（通常用 Verilog/VHDL），會產出 RTL code 和 Testbench

4. **Logic Synthesis**  
   把程式變成電路，把 RTL 轉成 gate-level，會產出 gate-level netlist 和 timing report  
   > netlist: 讓電腦能夠讀懂的電路行為描述  

5. **Physical Design**  
   把邏輯電路畫到晶片上，產出 Layout

6. **Mask Synthesis & Verification**  
   把 layout 轉成光罩並檢查設計規則（Design Rule Check, DRC）和電路佈局驗證（Layout Versus Schematic, LVS）

7. **Fabrication**  
   把 layout 製作在 wafer 上

8. **Packaging & Testing**  
   將 wafer 上的每個 die 切割封裝，變成可用的晶片

![IC design flow1](./img/IC_design_flow1.png)  
![IC design flow2](./img/IC_design_flow2.png)

## Synthesis & Technology Mapping

Synthesis: increasing information about the design by providing more detail（e.g. logic synthesis, physical synthesis）

**Logic synthesis**: 把 RTL 轉換成 gate-level netlist。因為 RTL 大多數都是用 HDL（High-level Description Language）寫的，因此也有人稱為 HDL synthesis，但嚴格來說 HDL synthesis 是被包含在 Logic synthesis 內。

Logic synthesis = Domain Translation（轉換成 gate-level）+ Optimization（選擇要用哪些 gate 會最佳）

**Technology mapping**: 是把設計綁定到某個製程（例如 TSMC 7nm），會從這個製程 library 中挑選哪個 gate 會最佳，因此 technology mapping 是 optimization 的一環。

**Physical synthesis**: 在 placement 階段時優化 size、buffer、...，簡單理解就是實體層的最佳化。

![HDL synthesis](./img/HDL_synthesis.png)

## Design Styles

IC 設計時的設計思路，設計時主要考量了 performance、area、cost 和 time to market。  
目前 design style 大宗有四種：

1. **Full custom**  
   完全從頭設計，performance 最強且能保有小 area，但 cost 高且 time to market 久

2. **Standard cell**  
   用已經設計好的標準邏輯元件，只需要寫 RTL 再用 EDA tool 做 synthesis、placement、routing、...  
   performance、area 和 cost、time to market 平衡，目前主流

3. **Gate array**  
   晶片上已經預先做好很多 transistor（base array），只須修改 metal layer 來連接  
   因為已經有預先做好很多 transistor，所以 cost 較 standard cell 低，且所需時間短  
   但性能和彈性較 standard cell 差

4. **FPGA（Field-Programmable Gate Array）**  
   最方便簡單入門兼容性高，因為晶片已經完成，可以透過燒錄 program 來完成預期的工作  
   為了高兼容性，所以 FPGA 內其實經常有很多已完成的部份，是預期燒錄的 program 用不到的部份  
   因此 area 大、performance 差、速度慢，但 cost 低；如果是小量產品可以快速開發完成

![design style](./img/design_style.png)

## MOS Transistors

pMOS: 當 gate 為 0 時會導通，pMOS 導通 signal "1" 效果好（因此經常接 VDD）  
nMOS: 當 gate 為 1 時會導通，nMOS 導通 signal "0" 效果好（因此經常接 GND）

![pmos](./img/pmos.png)  
![nmos](./img/nmos.png)

因為 pMOS 和 nMOS 互補，所以經常搭配一起設計電路，稱為 CMOS。  
這邊以 CMOS inverter 為例，並介紹電路怎麼走達到 inverter 的效果：

![inverter](./img/inverter.png)  
![inverter input0](./img/inverter_input0.png)  
![inverter input1](./img/inverter_input1.png)

下面一併放上 CMOS 的 NAND 和 NOR：

![NAND](./img/nand.png)  
![NOR](./img/nor.png)

### Construction of Compound Gates

1. 將 equation 先取反，output 為 0 的情況（製作 nMOS 的部份）
2. 將 nMOS 依照取反後的 equation 連接
3. 將原本的 equation 展開，output 為 1 的情況（製作 pMOS 的部份）
4. 將 pMOS 依照 equation 連接

example:  
F = (AB + CD)'

1. 取反  
   F' = AB + CD

2. 連接 nMOS  
   ![NMOS example](./img/nmos_example.png)

3. 展開原本的 equation  
   F = (A' + B')(C' + D')

4. 連接 pMOS  
   ![PMOS example](./img/pmos_example.png)

最終成果如下：  
![final example](./img/final_example.png)
