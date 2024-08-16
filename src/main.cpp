
#include <Print.h>
#include <SD.h>
#include <SPI.h>

struct SDCard {
    const int sck = 40;
    const int miso = 39;
    const int mosi = 14;
    const int cs = 12;
};

void setup(void) {
    SDCard mySd;
    Serial.begin(115200);
    delay(5000);
    while (!Serial) {
        delay(10);
    }

    SPI.begin(mySd.sck, mySd.miso, mySd.mosi, mySd.cs);

    while (false == SD.begin(mySd.cs, SPI)) {
        delay(1);
    }

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        printf("No SD card attached\n");
        return;
    }

    printf("SD Card Type: ");
    if (cardType == CARD_MMC) {
        printf("MMC\n");
    } else if (cardType == CARD_SD) {
        printf("SDSC\n");
    } else if (cardType == CARD_SDHC) {
        printf("SDHC\n");
    } else {
        printf("UNKNOWN\n");
    }

    // M5.Lcd.setRotation(1);
    //  Draw the jpeg file "p2.jpg" from TF(SD) card
    // M5.Lcd.setBrightness(200);

    // M5.Lcd.drawPngFile(SD, "/filename.png");

    // M5.Lcd.drawJpgFile(SD, "/p1.jpg", 20, 30, 40, 50);

    // M5.Lcd.drawJpgFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y,
    // uint16_t maxWidth, uint16_t maxHeight, uint16_t offX, uint16_t offY,
    // jpeg_div_t scale);
}

void loop() {
    // M5.Lcd.drawBmpFile(SD, "/filename.bmp");
    // M5.Lcd.drawJpgFile(SD, "/filename.jpg");
    // M5.Lcd.drawPngFile(SD, "/filename.png");
}