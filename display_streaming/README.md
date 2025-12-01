# Display Streaming - カメラ画像リアルタイム表示プロジェクト

SPRESENSE HDRカメラの映像をILI9341 TFT液晶にリアルタイム表示するプロジェクトです。

## 概要

このプロジェクトは、SPRESENSEのカメラから取得したQVGA（320x240）解像度の映像を、ILI9341ドライバ搭載のTFT液晶ディスプレイにリアルタイムでストリーミング表示します。camera_testとdisplay_testの機能を統合した実用的なアプリケーションです。

## 主な機能

- **カメラストリーミング**: QVGA解像度でのリアルタイム映像取得
- **高速描画**: LovyanGFXによる最適化された画像転送
- **コールバック方式**: 効率的なフレーム処理
- **RGB565変換**: カメラ画像からディスプレイ形式への自動変換

## ハードウェア構成

### 必要な機器

- **SPRESENSE メインボード**
- **SPRESENSE 拡張ボード**
- **SPRESENSE HDRカメラボード**
- **ILI9341ドライバ搭載 TFT液晶**（240x320ピクセル、SPI接続）

### ピン配置

#### カメラ接続
カメラボードはSPRESENSEメインボードに直接接続します。

#### ディスプレイ接続（SPI4使用）

| 機能 | SPRESENSEピン | 説明 |
|------|--------------|------|
| **SCLK** | D13 (SPI4) | SPIクロック |
| **MOSI** | D11 (SPI4) | SPIデータ出力 |
| **MISO** | D12 (SPI4) | SPIデータ入力 |
| **DC** | D9 | データ/コマンド選択 |
| **CS** | D10 | チップセレクト |
| **RESET** | D8 | リセット |
| **VCC** | 3.3V | 電源 |
| **GND** | GND | グランド |

**重要**: 
- SPRESENSEのSPI4ポートを使用しています
- カメラとディスプレイを同時に使用するため、電源容量に注意してください
- USB給電では電力不足の可能性があるため、外部電源の使用を推奨します

## 開発環境

### 必要なソフトウェア

- [PlatformIO](https://platformio.org/)
- Visual Studio Code + PlatformIO IDE拡張機能

### 使用ライブラリ

- **LovyanGFX** (v1.1.16以降): 高速グラフィックスライブラリ
- **Camera**: SPRESENSE標準カメラライブラリ（プラットフォームに含まれる）

## セットアップ手順

### 1. ハードウェア接続

1. HDRカメラボードをSPRESENSEメインボードに接続
2. 上記のピン配置表に従って、TFT液晶を拡張ボードに接続
3. **外部電源を接続**（推奨：5V 2A以上）

### 2. プロジェクトのビルドとアップロード

#### Visual Studio Codeでの操作

**重要**: このリポジトリには複数のPlatformIOプロジェクトが含まれています。**display_streamingフォルダを直接開いてください**。

1. VS Codeで**display_streamingフォルダ**を開きます
   - `ファイル` → `フォルダーを開く` → `display_streaming`フォルダを選択
   - または、コマンドラインから: `code display_streaming`

2. PlatformIOタブを開きます

3. ビルド:
   - PlatformIOツールバーの「Build」ボタン
   - または `Ctrl+Alt+B`

4. SPRESENSEを接続してアップロード:
   - PlatformIOツールバーの「Upload」ボタン
   - または `Ctrl+Alt+U`

5. シリアルモニタを開いて動作確認:
   - `Ctrl+Alt+S` または PlatformIOツールバーの「Serial Monitor」

#### コマンドラインでの操作

```bash
# display_streamingディレクトリに移動
cd display_streaming

# ビルド
pio run

# SPRESENSEにアップロード
pio run --target upload

# シリアルモニタを開く
pio device monitor
```

## 動作確認

### 期待される動作

1. シリアルモニタに以下のメッセージが表示されます：
   ```
   Initializing Camera…
   Streaming started.
   ```

2. TFT液晶にカメラからのリアルタイム映像が表示されます

3. 映像は横向き（Rotation=1）で表示されます

### パフォーマンス

- **解像度**: QVGA (320x240ピクセル)
- **フレームレート**: 約10-15 FPS（環境により変動）
- **遅延**: 約100-200ms

## プロジェクト構成

```
display_streaming/
├── platformio.ini              # PlatformIO設定ファイル
├── README.md                   # 本ファイル
├── include/                    # ヘッダファイル用ディレクトリ
├── lib/                        # 追加ライブラリ用ディレクトリ
└── src/
    ├── main.cpp                # メインプログラム
    └── LGFX_SPI4_ILI9341.hpp  # LovyanGFX設定ファイル
```

## トラブルシューティング

### カメラが初期化できない

**症状**: "Camera init failed" というエラーメッセージ

**対処法**:
- カメラボードがメインボードにしっかり接続されているか確認
- カメラモジュールのケーブル接続を確認
- 電源供給が十分か確認（外部電源使用を推奨）

### 画面に何も表示されない

**症状**: ディスプレイが真っ暗

**対処法**:
- ディスプレイのピン接続を再確認（特にD13, D11, D9, D10, D8）
- 電源電圧を確認（3.3V）
- シリアルモニタで "Streaming started" が表示されているか確認
- display_testプロジェクトで単独でディスプレイが動作するか確認

### フレームレートが低い

**症状**: 映像がカクカクする、遅延が大きい

**対処法**:
- **SPI通信速度を調整**: `LGFX_SPI4_ILI9341.hpp`の`freq_write`を変更
  ```cpp
  bcfg.freq_write = 20000000;  // 40MHz → 20MHzに下げる
  ```
- 外部電源を使用（USB給電では電力不足の可能性）
- 解像度を下げる（QQVGAに変更）
  ```cpp
  theCamera.begin(CAM_IMGSIZE_QQVGA, CAM_IMGSIZE_QQVGA)
  ```

### 電源の問題

**症状**: 動作が不安定、リセットがかかる

**対処法**:
- **外部電源を使用**（5V 2A以上推奨）
- USB給電の場合は高出力のUSB電源アダプタを使用
- カメラとディスプレイの同時使用は電力消費が大きいため、安定した電源が必須

### ビルドエラー

**症状**: コンパイル時にエラー

**対処法**:
- PlatformIOが最新版か確認
- プラットフォームとライブラリが正しくダウンロードされているか確認
- `.pio`フォルダを削除して再ビルド

## カスタマイズ

### 画面の向きを変更

`src/main.cpp`の`setup()`関数内：
```cpp
lcd.setRotation(0);  // 0, 1, 2, 3 で90度ごとに回転
```

### カメラ解像度の変更

```cpp
// QVGAから他の解像度に変更
theCamera.begin(CAM_IMGSIZE_QQVGA, CAM_IMGSIZE_QQVGA);  // より低解像度、高速
theCamera.begin(CAM_IMGSIZE_HD, CAM_IMGSIZE_HD);        // より高解像度、低速
```

**注意**: ディスプレイサイズ（320x240）を超える解像度は表示がはみ出します。

### SPI通信速度の調整

`src/LGFX_SPI4_ILI9341.hpp`で通信速度を変更：
```cpp
bcfg.freq_write = 40000000;  // 40MHz（デフォルト）
bcfg.freq_write = 20000000;  // 20MHz（安定重視）
bcfg.freq_write = 60000000;  // 60MHz（高速、不安定の可能性）
```

## 技術詳細

### アーキテクチャ

1. **カメラストリーミング**: `startStreaming()`でコールバック方式を使用
2. **画像変換**: カメラからのYUV画像をRGB565に変換
3. **高速転送**: LovyanGFXの`pushImage()`で効率的にSPI転送
4. **非同期処理**: メインループは空で、全ての処理はコールバック内で実行

### メモリ使用量

- カメラバッファ: 約150KB（QVGA RGB565）
- ディスプレイバッファ: LovyanGFXが管理
- 合計: 約200-300KB

### 消費電力

- カメラ動作時: 約200-300mA
- ディスプレイ動作時: 約50-100mA（輝度による）
- 合計: **約300-400mA**（外部電源推奨）

## 次のステップ

このプロジェクトで基本的なストリーミング表示が動作することを確認したら、以下の機能を追加できます：

1. **画像フィルタリング**: エッジ検出、コントラスト調整
2. **オーバーレイ表示**: フレームレート、タイムスタンプの表示
3. **タッチパネル操作**: カメラ設定の変更
4. **静止画キャプチャ**: ボタン押下でSDカードに保存
5. **シュリーレン画像処理**: 密度変化の可視化（本プロジェクトの最終目標）

## 参考資料

- [LovyanGFX GitHub](https://github.com/lovyan03/LovyanGFX)
- [SPRESENSE Camera API](https://developer.sony.com/develop/spresense/docs/arduino_developer_guide_ja.html#_camera)
- [SPRESENSE公式ドキュメント](https://developer.sony.com/develop/spresense/)
- [ILI9341データシート](https://cdn-shop.adafruit.com/datasheets/ILI9341.pdf)

## ライセンス

TBD

---

**更新日**: 2025/12/01  
**プロジェクト状況**: 基本実装完了 - カメラストリーミング + ディスプレイ表示動作確認済み
