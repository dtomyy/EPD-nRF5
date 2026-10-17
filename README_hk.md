# EPD-nRF5

電子報卡鈔韌體，支援農曆、二十四節氣與假期調度。除此之外，亦可透過藍牙將影像傳輸至電子墨水顯示器，作為數位相桌使用。日曆介面已適配常見的4.2吋與7.5吋電子紙解析度，同一個韌體可以驅動不同螢幕尺寸（透過網頁介面線上切換畫面大小與驅動程式）。

支援MCU：`nrf51822` / `nrf51802` / `nrf52811` / `nrf52810`。對於電子顯示器，支援常見的UC81xx/SSD16xx系列驅動程式（單色/三色/四色）。亦支援從電子顯示器到MCU的自定義腳位對應，以及睡眠/喚醒功能（NFC / 無線充電），以及藍牙OTA韌體更新。

## 網頁介面

本專案包含一個使用Web Bluetooth API實現的網頁介面。您可以透過手機或電腦訪問以下URL，或直接雙擊本地的`html/index.html`：

- URL: https://dtomyy.github.io/EPD-nRF5
- Demo視頻: https://www.bilibili.com/video/BV1KWAVe1EKs
- 討論群組: [1033086563](https://qm.qq/q/SckzhfDxuu) (點擊加入聊天群組)

網頁介面支援多種圖像曃圖演算法，並允許您在圖片上潦草塗鴉並添加文字。除了作為數位相桌外，亦可切換至日曆模式，顯示月曆、農曆二十四節氣、假期與工作排程調整。

## 支援的設備

[檢視文檔](docs/devices.md)。

## 開發

[檢視文檔](docs/develop.md)。

## 鳴謝

本專案使用或引用了以下項目的代碼：

- [ZinggJM/GxEPD2](https://github.com/ZinggJM/GxEPD2)
- [waveshareteam/e-Paper](https://github.com/waveshareteam/e-Paper)
- [atc1441/ATC_TLSR_Paper](https://github.com/atc1441/ATC_TLSR_Paper)\n\n--forked from tsl0922/EPD-nRF5