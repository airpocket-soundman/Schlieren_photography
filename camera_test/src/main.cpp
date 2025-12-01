#include <Arduino.h>
#include <Camera.h>
#include <SDHCI.h>

// SDカードインスタンス
SDClass SD;

// 撮影枚数カウンタ
int pictureCount = 0;

// LEDピン（SPRESENSE標準LED）
#define LED0 LED_BUILTIN

/**
 * @brief カメラの初期化
 */
void setupCamera() {
  Serial.println("カメラを初期化しています...");
  
  CamErr err = theCamera.begin();
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("カメラの初期化に失敗しました");
    while (1);
  }

  // HDRモードを有効化
  err = theCamera.setHDR(CAM_HDR_MODE_AUTO);
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("HDRモードの設定に失敗しました");
  } else {
    Serial.println("HDRモード: 有効");
  }

  // 画像フォーマットをJPEGに設定
  err = theCamera.setStillPictureImageFormat(
    CAM_IMGSIZE_HD_H,           // 横1280
    CAM_IMGSIZE_HD_V,           // 縦720
    CAM_IMAGE_PIX_FMT_JPG       // JPEG形式
  );
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("画像フォーマットの設定に失敗しました");
    while (1);
  }

  Serial.println("カメラの初期化が完了しました");
  Serial.println("解像度: 1280x720 (HD)");
  Serial.println("フォーマット: JPEG");
}

/**
 * @brief SDカードの初期化
 */
void setupSD() {
  Serial.println("SDカードを初期化しています...");
  
  if (!SD.begin()) {
    Serial.println("SDカードの初期化に失敗しました");
    Serial.println("SDカードが挿入されているか確認してください");
    while (1);
  }
  
  Serial.println("SDカードの初期化が完了しました");
}

/**
 * @brief 写真を撮影してSDカードに保存
 */
void takePicture() {
  // LED点灯（撮影中）
  digitalWrite(LED0, HIGH);
  
  Serial.println("撮影中...");
  
  // 写真を撮影
  CamImage img = theCamera.takePicture();
  
  if (!img.isAvailable()) {
    Serial.println("撮影に失敗しました");
    digitalWrite(LED0, LOW);
    return;
  }
  
  // ファイル名を生成
  char filename[32];
  sprintf(filename, "PICT%03d.JPG", pictureCount);
  
  Serial.print("保存中: ");
  Serial.println(filename);
  
  // SDカードに保存
  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("ファイルを開けませんでした");
    digitalWrite(LED0, LOW);
    return;
  }
  
  // 画像データを書き込み
  file.write(img.getImgBuff(), img.getImgSize());
  file.close();
  
  pictureCount++;
  
  Serial.print("保存完了: ");
  Serial.print(filename);
  Serial.print(" (");
  Serial.print(img.getImgSize());
  Serial.println(" bytes)");
  
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
  Serial.println("  SPRESENSE HDRカメラテスト");
  Serial.println("===================================");
  Serial.println();
  
  // LED初期化
  pinMode(LED0, OUTPUT);
  digitalWrite(LED0, LOW);
  
  // SDカード初期化
  setupSD();
  
  // カメラ初期化
  setupCamera();
  
  Serial.println();
  Serial.println("準備完了!");
  Serial.println("シリアルモニタに任意の文字を送信すると撮影します");
  Serial.println("-----------------------------------");
}

/**
 * @brief メインループ
 */
void loop() {
  // シリアル入力があれば撮影
  if (Serial.available() > 0) {
    // バッファをクリア
    while (Serial.available() > 0) {
      Serial.read();
    }
    
    Serial.println();
    takePicture();
    Serial.println("-----------------------------------");
    Serial.println("次の撮影待機中...");
  }
  
  delay(100);
}
