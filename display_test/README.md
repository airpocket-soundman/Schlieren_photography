# Display Test - ILI9341 TFT液晶テストプロジェクト

SPRESENSE + ILI9341ドライバ搭載SPI制御TFT液晶のテストプロジェクトです。

## 概要

このプロジェクトは、LovyanGFXライブラリを使用してILI9341ドライバを搭載したTFT液晶ディスプレイの動作確認を行います。

## ハードウェア構成

### 必要な機器

- **SPRESENSE メインボード**
- **SPRESENSE 拡張ボード**
- **ILI9341ドライバ搭載 TFT液晶**（240x320ピクセル、SPI接続）

### ピン配置

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

**重要**: SPRESENSEのSPI4ポートを使用しています。他のSPIポートでは動作しません。

## 開発環境

### 必要なソフトウェア

- [PlatformIO](https://platformio.org/)
- Visual Studio Code + PlatformIO IDE拡張機能

### 使用ライブラリ

- **LovyanGFX** (v1.1.16以降): 高速グラフィックスライブラリ

## セットアップ手順

### 1. ハードウェア接続

上記のピン配置表に従って、TFT液晶をSPRESENSEに接続してください。

### 2. プロジェクトのビルドとアップロード

#### Visual Studio Codeでの操作

**重要**: このリポジトリには複数のPlatformIOプロジェクトが含まれています。**display_testフォルダを直接開いてください**（リポジトリのルートフォルダではありません）。

1. VS Codeで**display_testフォルダ**を開きます
   - `ファイル` → `フォルダーを開く` → `display_test`フォルダを選択
   - または、コマンドラインから: `code display_test`
2. PlatformIOタブを開きます
3. `display_test` 環境を選択
4. 以下のいずれかの方法でビルド：
   - PlatformIOツールバーの「Build」ボタンをクリック
   - ショートカットキー: `Ctrl+Alt+B`
   - コマンドパレット: `PlatformIO: Build`

5. SPRESENSEを接続してアップロード：
   - PlatformIOツールバーの「Upload」ボタンをクリック
   - ショートカットキー: `Ctrl+Alt+U`
   - コマンドパレット: `PlatformIO: Upload`

#### コマンドラインでの操作（PlatformIO CLIがインストールされている場合）

```bash
# display_testディレクトリに移動
cd display_test

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
   ILI9341 Test Start (SPI4)
   lcd.init()...
   lcd.init() done
   fillScreen(BLACK)...
   fillScreen done
   text draw done
   ```

2. TFT液晶の画面が黒くクリアされます

3. 画面左上（座標10,10）に白色で "Hello Spresense" と表示されます

### トラブルシューティング

#### 画面に何も表示されない

- **配線を確認**: 特にD13(SCLK)、D11(MOSI)、D9(DC)、D10(CS)、D8(RESET)が正しく接続されているか
- **電源電圧を確認**: 3.3Vが供給されているか
- **シリアル出力を確認**: `lcd.init()...` の後でハングしている場合はハードウェアフォルト

#### ハードフォルトが発生する

- **SPI4ポートの設定を確認**: lgfx設定ファイルで `cfg.spi_port = 4` になっているか
- **通信速度を下げる**: `freq_write` を 40MHz から 20MHz に変更
  ```cpp
  cfg.freq_write = 20000000;  // 20MHz
  ```

#### 画面がちらつく/表示が不安定

- **配線の長さを短く**: ジャンパワイヤーはできるだけ短くする
- **GND接続を確認**: しっかりとグランドが共通になっているか
- **通信速度を下げる**: 上記の方法で速度を20MHzに変更

## プロジェクト構成

```
display_test/
├── platformio.ini              # PlatformIO設定ファイル
├── README.md                   # 本ファイル
├── include/                    # ヘッダファイル用ディレクトリ
├── lib/                        # 追加ライブラリ用ディレクトリ
└── src/
    ├── main.cpp                # メインプログラム
    └── LGFX_SPRESENSE_ILI9341.hpp  # LovyanGFX設定ファイル
```

## カスタマイズ

### 表示内容の変更

`src/main.cpp` の `setup()` 関数内で表示内容をカスタマイズできます：

```cpp
// 画面回転（0, 1, 2, 3）
lcd.setRotation(1);

// 背景色の変更
lcd.fillScreen(TFT_BLUE);  // 青色

// テキストサイズの変更
lcd.setTextSize(3);  // 3倍サイズ

// テキスト色の変更
lcd.setTextColor(TFT_RED, TFT_BLACK);  // 赤文字、黒背景

// 座標指定
lcd.setCursor(50, 100);

// 描画
lcd.print("Custom Text");
```

### SPI通信速度の調整

`src/LGFX_SPRESENSE_ILI9341.hpp` で通信速度を変更できます：

```cpp
cfg.freq_write = 20000000;  // 20MHz（安定重視）
cfg.freq_write = 40000000;  // 40MHz（高速、デフォルト）
```

### ピン配置の変更

異なるピン配置を使用する場合は、`LGFX_SPRESENSE_ILI9341.hpp` を編集：

```cpp
cfg.pin_dc   = 9;   // DC ピン
cfg.pin_cs   = 10;  // CS ピン
cfg.pin_rst  = 8;   // RESET ピン
```

## 次のステップ

このプロジェクトで液晶表示が正常に動作することを確認したら、以下の機能を追加できます：

1. **タッチパネル入力**の実装
2. **画像表示機能**の追加
3. **GUIインターフェース**の作成
4. **カメラ画像のリアルタイム表示**（schlieren_cameraプロジェクトへの統合）

## 参考資料

- [LovyanGFX GitHub](https://github.com/lovyan03/LovyanGFX)
- [SPRESENSE公式ドキュメント](https://developer.sony.com/develop/spresense/)
- [ILI9341データシート](https://cdn-shop.adafruit.com/datasheets/ILI9341.pdf)

## ライセンス

TBD

---

**更新日**: 2025/12/01  
**プロジェクト状況**: 動作確認済み
