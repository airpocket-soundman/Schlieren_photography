#include <Arduino.h>
#include <Camera.h>
#include "LGFX_SPRESENSE_ILI9341.hpp"

LGFX lcd;

// ==== Camera Callback ==== //
void CamCB(CamImage img) {
  static int frameCount = 0;
  
  if (!img.isAvailable()) {
    Serial.println("frame NG");
    return;
  }

  // 最初の数フレームだけ詳細情報を出力
  if (frameCount < 5) {
    Serial.print("Frame ");
    Serial.print(frameCount);
    Serial.print(": ");
    Serial.print(img.getWidth());
    Serial.print("x");
    Serial.print(img.getHeight());
    Serial.print(" PixFormat=");
    Serial.print(img.getPixFormat());
    Serial.print(" ImgSize=");
    Serial.println(img.getImgSize());
  }
  
  // 10フレームごとに通知
  if (frameCount % 10 == 0 && frameCount > 0) {
    Serial.print(".");
  }

  // YUV422 → RGB565 変換
  CamErr err = img.convertPixFormat(CAM_IMAGE_PIX_FMT_RGB565);
  if (err != CAM_ERR_SUCCESS) {
    Serial.print("convertPixFormat failed: ");
    Serial.println(err);
    return;
  }
  
  if (frameCount < 3) {
    Serial.print("After convert - ImgSize=");
    Serial.print(img.getImgSize());
    Serial.print(" expected=");
    Serial.println(320*240*2);  // RGB565 = 2 bytes per pixel
    
    // 最初の数ピクセルを確認
    uint16_t* buf = (uint16_t*)img.getImgBuff();
    Serial.print("First pixels: ");
    Serial.print(buf[0], HEX);
    Serial.print(" ");
    Serial.print(buf[1], HEX);
    Serial.print(" ");
    Serial.println(buf[2], HEX);
  }

  // ピクセルデータをLCDへ描画
  lcd.pushImage(
    0, 0,
    img.getWidth(),
    img.getHeight(),
    (uint16_t*)img.getImgBuff()
  );
  
  frameCount++;
}

// ==== Camera Init ==== //
void setupCamera() {
  Serial.println("Camera init...");

  // カメラを初期化
  CamErr err = theCamera.begin();
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("Camera init failed");
    while(1);
  }

  // 自動露出とホワイトバランスを有効化
  err = theCamera.setAutoWhiteBalanceMode(CAM_WHITE_BALANCE_AUTO);
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("setAutoWhiteBalanceMode failed");
  }

  err = theCamera.setAutoExposure(true);
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("setAutoExposure failed");
  }

  // プレビューモードで QVGA サイズを設定
  // ストリーミングはプレビューモードで動作
  err = theCamera.setStillPictureImageFormat(
    CAM_IMGSIZE_QVGA_H,
    CAM_IMGSIZE_QVGA_V,
    CAM_IMAGE_PIX_FMT_YUV422
  );
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("setStillPictureImageFormat failed");
    while(1);
  }

  Serial.println("Camera format set to QVGA (320x240)");

  // ストリーミング開始（コールバック方式）
  err = theCamera.startStreaming(true, CamCB);
  if (err != CAM_ERR_SUCCESS) {
    Serial.println("Streaming start failed");
    while(1);
  }

  Serial.println("Camera streaming started successfully");
}

void setup() {
  Serial.begin(115200);
  while(!Serial){;}

  Serial.println("=== Spresense Camera → ILI9341 Stream ===");
  Serial.println("!!! Camera is DISABLED for display test !!!");

  // ディスプレイ初期化テスト
  Serial.println("Display init...");
  lcd.init();
  Serial.println("lcd.init() done");
  
  lcd.setRotation(0);  // まず縦向きでテスト（display_testと同じ）
  Serial.println("setRotation(0) done");
  
  lcd.fillScreen(TFT_BLACK);
  Serial.println("fillScreen BLACK done");
  delay(500);
  
  lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  lcd.setTextSize(2);
  lcd.setCursor(10, 10);
  lcd.print("Hello Spresense");
  Serial.println("text draw done");
  
  delay(2000);
  
  lcd.fillScreen(TFT_RED);  // 赤色で塗りつぶし（テスト）
  Serial.println("fillScreen RED done");
  delay(1000);
  
  lcd.fillScreen(TFT_GREEN);  // 緑色（テスト）
  Serial.println("fillScreen GREEN done");
  delay(1000);
  
  lcd.fillScreen(TFT_BLUE);  // 青色（テスト）
  Serial.println("fillScreen BLUE done");
  delay(1000);
  
  lcd.fillScreen(TFT_BLACK);
  Serial.println("Display test complete");

  // カメラは起動しない
  // setupCamera();
  
  Serial.println("=== Setup complete (Camera disabled) ===");
}

void loop() {
  // 何もしない (callback でフレーム処理)
}
