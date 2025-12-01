# Schlieren_photography

シュリーレン法を用いた撮影装置の開発プロジェクト

## 概要

このプロジェクトでは、シュリーレン法（Schlieren photography）を用いた可視化撮影装置をSPRESENSEマイコンボードを使用して製作します。シュリーレン法は、透明な流体中の密度変化を可視化する技術で、熱対流や衝撃波、音波などの観察に使用されます。

## プロジェクト構成

このリポジトリには3つの独立したプロジェクトが含まれています：

```
Schlieren_photography/
├── README.md                    # 本ファイル
├── camera_test/                 # カメラテストプロジェクト
│   ├── platformio.ini
│   ├── src/
│   │   └── main.cpp
│   └── README.md
├── display_test/                # ディスプレイテストプロジェクト
│   ├── platformio.ini
│   ├── src/
│   │   └── main.cpp
│   └── README.md
└── schlieren_camera/            # シュリーレンカメラ統合プロジェクト（準備中）
    ├── platformio.ini
    ├── src/
    │   └── main.cpp
    └── README.md
```

### 1. camera_test

SPRESENSE + HDRカメラボードを使用したJPEG画像撮影のテストプロジェクト。

**主な機能:**
- HDRモード撮影
- JPEG形式での画像保存（1280x720 HD解像度）
- SDカードへの自動保存
- シリアル通信によるトリガー撮影

詳細は [camera_test/README.md](camera_test/README.md) を参照してください。

### 2. display_test

ILI9341ドライバを使用したSPI制御TFT液晶のテストプロジェクト。

**主な機能:**
- TFT液晶の初期化と表示制御
- LovyanGFXライブラリによる高速グラフィックス
- SPRESENSEのSPI4ポート使用
- 基本的なテキスト・図形描画

詳細は [display_test/README.md](display_test/README.md) を参照してください。

### 3. schlieren_camera（準備中）

カメラとディスプレイを統合したシュリーレン撮影システムの完全版。

**予定機能:**
- リアルタイム画像表示
- タッチパネルによる操作UI
- HDR撮影と画像処理
- シュリーレン画像の最適化
- 画像保存機能

## ハードウェア構成

### コントローラ

| コンポーネント | 説明 |
|--------------|------|
| **SPRESENSE メインボード** | Sony製の高性能マイコンボード（CXD5602） |
| **SPRESENSE 拡張ボード** | メインボードの機能拡張用ボード |
| **HDRカメラボード** | 高ダイナミックレンジ撮影対応カメラモジュール |
| **SPI制御 TP付TFT液晶** | タッチパネル付きディスプレイ（ドライバ: ILI9341） |

### 光学系

| コンポーネント | 仕様 | 個数 |
|--------------|------|------|
| **高輝度LED** | 光源として使用 | 1個 |
| **フレネルレンズ** | 直径: 150mm<br>焦点距離: 100mm<br>用途: 平行光生成とカメラへの導光 | 2枚 |

## システム構成

### シュリーレン法の基本原理

1. **光源部**: 高輝度LEDから発光
2. **平行光生成**: 第1フレネルレンズで平行光を生成
3. **観測領域**: 密度変化のある流体を平行光が通過
4. **集光部**: 第2フレネルレンズでカメラに導光
5. **撮影部**: HDRカメラボードで画像を取得
6. **表示部**: TFT液晶でリアルタイム表示

## 開発環境

### 必要なソフトウェア

- [PlatformIO](https://platformio.org/)
- Visual Studio Code + PlatformIO IDE拡張機能（推奨）

### セットアップ手順

1. PlatformIOのインストール
   ```bash
   # VS Code拡張機能からインストール、または
   pip install platformio
   ```

2. リポジトリのクローン
   ```bash
   git clone https://github.com/airpocket-soundman/Schlieren_photography.git
   cd Schlieren_photography
   ```

3. 各プロジェクトのビルド

   **重要**: このリポジトリには複数の独立したPlatformIOプロジェクトが含まれています。VS Codeでは、**各プロジェクトフォルダ（camera_test、display_testなど）を個別に開いてください**。リポジトリのルートフォルダではなく、作業したいプロジェクトフォルダを直接開く必要があります。

   ```bash
   # camera_testをビルド
   cd camera_test
   pio run
   
   # SPRESENSEに書き込み
   pio run --target upload
   ```

   VS Codeで開く場合：
   ```bash
   # camera_testプロジェクトを開く
   code camera_test
   
   # または display_testプロジェクトを開く
   code display_test
   ```

## 開発ロードマップ

- [x] プロジェクト構造の構築
- [x] camera_test: 基本的なカメラ撮影機能の実装
- [x] display_test: ディスプレイ制御の基本実装
  - [x] LovyanGFXライブラリ統合
  - [x] ILI9341ドライバでの表示動作確認
  - [ ] タッチパネル入力機能
  - [ ] 高度なGUI機能
- [ ] schlieren_camera: 統合システムの実装
  - [ ] リアルタイム画像表示
  - [ ] タッチUIの実装
  - [ ] 画像処理・エッジ検出
  - [ ] LED光源の輝度制御
  - [ ] 最適化と性能向上

## 使用方法

各プロジェクトの詳細な使用方法は、それぞれのREADME.mdを参照してください：

- [camera_test/README.md](camera_test/README.md) - カメラテストの使用方法
- display_test/README.md - ディスプレイテストの使用方法（準備中）
- schlieren_camera/README.md - シュリーレンカメラの使用方法（準備中）

## ハードウェア接続

### 基本接続

1. メインボードを拡張ボードに接続
2. HDRカメラボードをメインボードに接続
3. TFT液晶をSPIピンに接続（詳細は各プロジェクトのREADMEを参照）
4. microSDカードを拡張ボードのSDスロットに挿入

### 光学系のセットアップ

1. 高輝度LEDを点光源として配置
2. 第1フレネルレンズ（焦点距離100mm）をLEDから100mm離して配置
3. 観測領域を平行光の経路に配置
4. 第2フレネルレンズをカメラの前に配置し、集光

## トラブルシューティング

### ビルドエラー

- PlatformIOが正しくインストールされているか確認
- `platformio.ini`の設定を確認
- 必要なライブラリが自動でダウンロードされない場合は手動でインストール

### アップロードエラー

- SPRESENSEが正しく接続されているか確認
- COMポートを確認（デバイスマネージャーで確認可能）
- ブートローダーモードに入っているか確認

## 参考資料

- [SPRESENSE公式ドキュメント](https://developer.sony.com/develop/spresense/)
- [PlatformIO SPRESENSE](https://docs.platformio.org/en/latest/boards/sonyspresense/spresense.html)
- シュリーレン法の原理と応用
- ILI9341データシート

## ライセンス

TBD

## 貢献

プルリクエストやイシューの投稿を歓迎します。

---

**更新日**: 2025/12/01  
**プロジェクト状況**: camera_test 完成 / display_test 準備中 / schlieren_camera 準備中
