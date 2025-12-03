#include <Arduino.h>
#include <Camera.h>
#include <SDHCI.h>
#include "LGFX_SPRESENSE_ILI9341.hpp"

// SDカードインスタンス
SDClass SD;

// ディスプレイインスタンス
LGFX lcd;

// 撮影枚数カウンタ
int pictureCount = 0;

// 撮影フラグ
volatile bool bTakePicture = false;

// LEDピン（SPRESENSE標準LED）
#define LED0 LED_BUILTIN

/**
 * @brief カメラコールバック（ストリーミング用）
 */
void CamCB(CamImage img) {
  if (!img.isAvailable()) {
    return;
  }
  
  // YUV422からRGB565に変換
  CamErr err = img.convertPixFormat(CAM_IMAGE_PIX_FMT_RGB565);
  if (err == CAM_ERR_SUCCESS) {
    uint16_t* imgBuf = (uint16_t*)img.getImgBuff();
    // LovyanGFXはバイトスワップが必要
    lcd.setSwapBytes(true);
    lcd.pushImage(0, 0, CAM_IMGSIZE_QVGA_H, CAM_IMGSIZE_QVGA_V, imgBuf);
  }
}

/**
 * @brief ディスプレイの初期化
 */
void setupDisplay() {
  Serial.println("ディスプレイを初期化しています...");
  
  lcd.init();
  lcd.setRotation(1);  // 横向き（320x240）
  lcd.fillScreen(TFT_BLACK);
  
  lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  lcd.setTextSize(2);
  lcd.setCursor(10, 10);
  lcd.print("Hello Spresense");
  
  Serial.println("ディスプレイの初期化が完了しました");
}

/**
 * @brief カメラの初期化
 */
void setupCamera() {
  Serial.println("カメラを初期化しています...");
  
  CamErr err = theCamera.begin();
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("カメラの初期化に失敗しました");
    lcd.fillScreen(TFT_RED);
    lcd.setCursor(10, 10);
    lcd.print("Camera Error");
    while (1);
  }

  // 自動露出とホワイトバランスを有効化
  err = theCamera.setAutoWhiteBalanceMode(CAM_WHITE_BALANCE_AUTO);
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("オートホワイトバランスの設定に失敗しました");
  }

  err = theCamera.setAutoExposure(true);
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("自動露出の設定に失敗しました");
  }

  Serial.println("カメラの初期化が完了しました");
  
  // ストリーミング開始（デフォルトフォーマット）
  Serial.println("ストリーミング開始...");
  err = theCamera.startStreaming(true, CamCB);
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("ストリーミング開始に失敗しました");
    lcd.fillScreen(TFT_RED);
    lcd.setCursor(10, 10);
    lcd.print("Streaming Error");
    while (1);
  }
  
  Serial.println("ストリーミング開始しました");
  
  // HDRモードを有効化
  err = theCamera.setHDR(CAM_HDR_MODE_AUTO);
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("HDRモードの設定に失敗しました");
  } else {
    Serial.println("HDRモード: 有効");
  }
  
  // 静止画撮影用にJPEGフォーマットを設定（HDサイズ）
  err = theCamera.setStillPictureImageFormat(
    CAM_IMGSIZE_HD_H,           // 横1280
    CAM_IMGSIZE_HD_V,           // 縦720
    CAM_IMAGE_PIX_FMT_JPG       // JPEG形式
  );
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("JPEG画像フォーマットの設定に失敗しました");
  } else {
    Serial.println("静止画フォーマット: HD JPEG (1280x720) HDR");
  }
}

/**
 * @brief SDカードの初期化
 */
void setupSD() {
  Serial.println("SDカードを初期化しています...");
  
  if (!SD.begin()) {
    Serial.println("SDカードの初期化に失敗しました");
    Serial.println("SDカードが挿入されているか確認してください");
    
    lcd.fillScreen(TFT_RED);
    lcd.setCursor(10, 10);
    lcd.print("SD Card Error");
    lcd.setCursor(10, 30);
    lcd.print("Check SD Card");
    while (1);
  }
  
  Serial.println("SDカードの初期化が完了しました");
}

/**
 * @brief JPEG保存用の撮影
 */
void takePictureAndSave() {
  // LED点灯（撮影中）
  digitalWrite(LED0, HIGH);
  
  Serial.println("ストリーミング停止中...");
  
  // ストリーミング停止
  CamErr err = theCamera.startStreaming(false, CamCB);
  if (err != CAM_ERR_SUCCESS) {
    Serial.print("ストリーミング停止失敗: ");
    Serial.println(err);
  }
  
  delay(100); // カメラが停止するまで待機
  
  Serial.println("JPEG撮影中...");
  
  // JPEG形式で撮影（setup()でJPEG形式に設定済み）
  CamImage img = theCamera.takePicture();
  
  if (!img.isAvailable()) {
    Serial.println("撮影に失敗しました");
    
    // ストリーミング再開
    theCamera.startStreaming(true, CamCB);
    digitalWrite(LED0, LOW);
    return;
  }
  
  // ファイル名を生成
  char filename[32];
  sprintf(filename, "PICT%03d.jpg", pictureCount);
  
  Serial.print("JPEG保存中: ");
  Serial.println(filename);
  
  // SDカードにJPEG形式で保存
  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("ファイルを開けませんでした");
    
    // ストリーミング再開
    theCamera.startStreaming(true, CamCB);
    digitalWrite(LED0, LOW);
    return;
  }
  
  // JPEG画像データを書き込み
  file.write(img.getImgBuff(), img.getImgSize());
  file.close();
  
  Serial.print("JPEG保存完了: ");
  Serial.print(filename);
  Serial.print(" (");
  Serial.print(img.getImgSize());
  Serial.println(" bytes)");
  
  pictureCount++;
  
  Serial.println("ストリーミング再開中...");
  
  // ストリーミング再開
  err = theCamera.startStreaming(true, CamCB);
  if (err != CAM_ERR_SUCCESS) {
    Serial.print("ストリーミング再開失敗: ");
    Serial.println(err);
  }
  
  Serial.println("表示モードに戻りました");
  Serial.print("撮影回数: ");
  Serial.println(pictureCount);
  
  // LED消灯
  digitalWrite(LED0, LOW);
}

/**
 * @brief 初期設定
 */
void setup() {
  // シリアル通信開始
  Serial.begin(115200);
  while (!Serial) {
    ; // シリアルポートの接続を待つ
  }
  
  Serial.println("===================================");
  Serial.println("  Camera + Display Test");
  Serial.println("  撮影→SD保存→表示");
  Serial.println("===================================");
  Serial.println();
  
  // LED初期化
  pinMode(LED0, OUTPUT);
  digitalWrite(LED0, LOW);
  
  // ディスプレイ初期化
  setupDisplay();
  
  // SDカード初期化
  setupSD();
  
  // カメラ初期化
  setupCamera();
  
  Serial.println();
  Serial.println("準備完了!");
  Serial.println("シリアルモニタに任意の文字を送信すると撮影します");
  Serial.println("-----------------------------------");
  
  // 準備完了メッセージ - まず2秒待機してから表示
  delay(2000);
  
  lcd.fillScreen(TFT_GREEN);
  lcd.setTextColor(TFT_WHITE, TFT_GREEN);
  lcd.setTextSize(2);
  lcd.setCursor(10, 10);
  lcd.print("Ready!");
  lcd.setCursor(10, 40);
  lcd.setTextSize(1);
  lcd.print("Send any char");
  lcd.print(" to capture");
}

/**
 * @brief メインループ
 */
void loop() {
  // シリアル入力があればJPEG保存
  if (Serial.available() > 0) {
    // バッファをクリア
    while (Serial.available() > 0) {
      Serial.read();
    }
    
    Serial.println();
    takePictureAndSave();
    Serial.println("-----------------------------------");
    Serial.println("次の撮影待機中...");
  }
  
  delay(10); // CPU負荷軽減
}
