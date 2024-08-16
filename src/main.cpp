/*
 * pin 1 - not used          |  Micro SD card     |
 * pin 2 - CS (SS)           |                   /
 * pin 3 - DI (MOSI)         |                  |__
 * pin 4 - VDD (3.3V)        |                    |
 * pin 5 - SCK (SCLK)        | 8 7 6 5 4 3 2 1   /
 * pin 6 - VSS (GND)         | ▄ ▄ ▄ ▄ ▄ ▄ ▄ ▄  /
 * pin 7 - DO (MISO)         | ▀ ▀ █ ▀ █ ▀ ▀ ▀ |
 * pin 8 - not used          |_________________|
 *                             ║ ║ ║ ║ ║ ║ ║ ║
 *                     ╔═══════╝ ║ ║ ║ ║ ║ ║ ╚═════════╗
 *                     ║         ║ ║ ║ ║ ║ ╚══════╗    ║
 *                     ║   ╔═════╝ ║ ║ ║ ╚═════╗  ║    ║
 * Connections for     ║   ║   ╔═══╩═║═║═══╗   ║  ║    ║
 * full-sized          ║   ║   ║   ╔═╝ ║   ║   ║  ║    ║
 * SD card             ║   ║   ║   ║   ║   ║   ║  ║    ║
 * Pin name         |  -  DO  VSS SCK VDD VSS DI CS    -  |
 * SD pin number    |  8   7   6   5   4   3   2   1   9 /
 *                  |                                  █/
 *                  |__▍___▊___█___█___█___█___█___█___/
 *
 * Note:  The SPI pins can be manually configured by using `SPI.begin(sck, miso, mosi, cs).`
 *        Alternatively, you can change the CS pin and use the other default settings by using `SD.begin(cs)`.
 *
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | SPI Pin Name | ESP8266 | ESP32 | ESP32‑S2 | ESP32‑S3 | ESP32‑C3 | ESP32‑C6 | ESP32‑H2 |
 * +==============+=========+=======+==========+==========+==========+==========+==========+
 * | CS (SS)      | GPIO15  | GPIO5 | GPIO34   | GPIO10   | GPIO7    | GPIO18   | GPIO0    |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | DI (MOSI)    | GPIO13  | GPIO23| GPIO35   | GPIO11   | GPIO6    | GPIO19   | GPIO25   |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | DO (MISO)    | GPIO12  | GPIO19| GPIO37   | GPIO13   | GPIO5    | GPIO20   | GPIO11   |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | SCK (SCLK)   | GPIO14  | GPIO18| GPIO36   | GPIO12   | GPIO4    | GPIO21   | GPIO10   |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 *
 * For more info see file README.md in this library or on URL:
 * https://github.com/espressif/arduino-esp32/tree/master/libraries/SD
 */

#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Uncomment and set up if you want to use custom pins for the SPI communication
#define REASSIGN_PINS
int sck = 40;
int miso = 39;
int mosi = 14;
int cs = 12;

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
    printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        printf("Failed to open directory\n");
        return;
    }
    if (!root.isDirectory()) {
        printf("Not a directory\n");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            printf("  DIR : ");
            printf("%s\n", file.name());
            if (levels) {
                listDir(fs, file.path(), levels - 1);
            }
        } else {
            printf("  FILE: ");
            printf("%s\n", file.name());
            printf("  SIZE: ");
            printf("%d\n", file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char *path) {
    printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path)) {
        printf("Dir created\n");
    } else {
        printf("mkdir failed\n");
    }
}

void removeDir(fs::FS &fs, const char *path) {
    printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path)) {
        printf("Dir removed\n");
    } else {
        printf("rmdir failed\n");
    }
}

void readFile(fs::FS &fs, const char *path) {
    printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file) {
        printf("Failed to open file for reading\n");
        return;
    }

    printf("Read from file: \n");
    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
    printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        printf("Failed to open file for writing\n");
        return;
    }
    if (file.print(message)) {
        printf("File written\n");
    } else {
        printf("Write failed\n");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
    printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file) {
        printf("Failed to open file for appending\n");
        return;
    }
    if (file.print(message)) {
        printf("Message appended\n");
    } else {
        printf("Append failed\n");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2) {
    printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        printf("File renamed\n");
    } else {
        printf("Rename failed\n");
    }
}

void deleteFile(fs::FS &fs, const char *path) {
    printf("Deleting file: %s\n", path);
    if (fs.remove(path)) {
        printf("File deleted\n");
    } else {
        printf("Delete failed\n");
    }
}

void testFileIO(fs::FS &fs, const char *path) {
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if (file) {
        len = file.size();
        size_t flen = len;
        start = millis();
        while (len) {
            size_t toRead = len;
            if (toRead > 512) {
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        printf("%u bytes read for %lu ms\n", flen, end);
        file.close();
    } else {
        printf("Failed to open file for reading\n");
    }

    file = fs.open(path, FILE_WRITE);
    if (!file) {
        printf("Failed to open file for writing\n");
        return;
    }

    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++) {
        file.write(buf, 512);
    }
    end = millis() - start;
    printf("%u bytes written for %lu ms\n", 2048 * 512, end);
    file.close();
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }

#ifdef REASSIGN_PINS
    SPI.begin(sck, miso, mosi, cs);
    if (!SD.begin(cs)) {
#else
    if (!SD.begin()) {
#endif
        printf("Card Mount Failed\n");
        return;
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

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    printf("SD Card Size: %lluMB\n", cardSize);

    // listDir(SD, "/", 0);
    // createDir(SD, "/mydir");
    // listDir(SD, "/", 0);
    // removeDir(SD, "/mydir");
    // listDir(SD, "/", 2);
    // writeFile(SD, "/hello.txt", "Hello ");
    // appendFile(SD, "/hello.txt", "World!\n");
    // readFile(SD, "/hello.txt");
    // deleteFile(SD, "/foo.txt");
    // renameFile(SD, "/hello.txt", "/foo.txt");
    // readFile(SD, "/foo.txt");
    // testFileIO(SD, "/test.txt");
    printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void loop() {}
