#include <SPI.h>
#include <SD.h>
#include <M5Cardputer.h>
#include <string>
#include "MjpegClass.h"
#include "cqueue.h"

#define FPS 30
#define AUDIO_SAMPLES_RATE 44100
#define AUDIO_BUFFER_SIZE 2940
#define MJPEG_BUFFER_SIZE (240 * 134 * 2 / 4)

#define SD_SPI_SCK_PIN  40
#define SD_SPI_MISO_PIN 39
#define SD_SPI_MOSI_PIN 14
#define SD_SPI_CS_PIN   12

M5Canvas canvas(&M5Cardputer.Display);

bool init_sdcard();
void printf_log(const char *format, ...);
void println_log(const char *str);

void setup() {
    Serial.begin(115200);
    M5Cardputer.begin();
    M5Cardputer.Display.setRotation(1);
    canvas.setColorDepth(1);

    canvas.createSprite(M5Cardputer.Display.width(),
                        M5Cardputer.Display.height());
    canvas.setPaletteColor(1, GREEN);
    canvas.setTextSize((float)canvas.width() / 160);
    canvas.setTextScroll(true);

    if (!init_sdcard()) {
        println_log("SD card initialization failed!");
        return;
    }

    CircularQueue<std::string> videoQueue;
    File root = SD.open("/");
    if (!root || !root.isDirectory()) {
        println_log("Failed to open root directory or not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        String fileName = file.name();
        if (!file.isDirectory() && fileName.endsWith(".mjpeg")) {
            int dotIndex = fileName.lastIndexOf('.');
            if (dotIndex > 0) {
                String nameWithoutExtension = fileName.substring(0, dotIndex);
                println_log(nameWithoutExtension.c_str());
                videoQueue.push(("/" + nameWithoutExtension).c_str());
            }
        }
        file = root.openNextFile();
    }

    M5Cardputer.Speaker.begin();
    M5Cardputer.Speaker.setVolume(64);

    static MjpegClass mjpeg;
    static uint8_t *aBuf = (uint8_t *)malloc(AUDIO_BUFFER_SIZE);
    static uint8_t *mjpeg_buf = (uint8_t *)malloc(MJPEG_BUFFER_SIZE);

    delay(500);

    while (true) {
        delay(750);
        canvas.fillSprite(0);
        canvas.pushSprite(0, 0);
        const std::string videoName = videoQueue.next();
        File aFile = SD.open((videoName + ".pcm").c_str());
        File vFile = SD.open((videoName + ".mjpeg").c_str());
        if (!aFile || aFile.isDirectory()) {
            printf_log("ERROR: Failed to open %s.pcm file for reading!\n", videoName.c_str());
            continue;
        } else if (!vFile || vFile.isDirectory()) {
            printf_log("ERROR: Failed to open %s.mjpeg file for reading!\n", videoName.c_str());
            continue;
        }
        mjpeg.reset();
        mjpeg.setup(vFile, mjpeg_buf, &M5Cardputer.Display, false);
        bool playing = true;
        while (aFile.available() && vFile.available()) {
            if (playing) {
                aFile.read(aBuf, AUDIO_BUFFER_SIZE);
                M5Cardputer.Speaker.playRaw(aBuf, AUDIO_BUFFER_SIZE, AUDIO_SAMPLES_RATE, false);
                mjpeg.readMjpegBuf();
                mjpeg.readMjpegBuf();
                mjpeg.drawJpg();
            }
            while (M5Cardputer.Speaker.isPlaying() || !playing) {
                M5Cardputer.update();
                if (M5Cardputer.BtnA.wasClicked()) {
                    playing = !playing;
                }
                if (M5Cardputer.Keyboard.isChange()) {
                    if (M5Cardputer.Keyboard.isKeyPressed('-')) {
                        M5Cardputer.Speaker.setVolume(M5Cardputer.Speaker.getVolume() - 15);
                    } else if (M5Cardputer.Keyboard.isKeyPressed('=')) {
                        M5Cardputer.Speaker.setVolume(M5Cardputer.Speaker.getVolume() + 15);
                    }
                    if (M5Cardputer.Keyboard.isKeyPressed('/')) {
                        aFile.close();
                        vFile.close();
                        break;
                    } else if (M5Cardputer.Keyboard.isKeyPressed(',')) {
                        videoQueue.rollback();
                        aFile.close();
                        vFile.close();
                        break;
                    }
                }
            }
        }
        aFile.close();
        vFile.close();
    }
}

void loop() {
}

bool init_sdcard() {
    SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);

    if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
        println_log("Card failed, or not present");
        return false;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        println_log("No SD card attached");
        return false;
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) {
        println_log("MMC");
    } else if (cardType == CARD_SD) {
        println_log("SDSC");
    } else if (cardType == CARD_SDHC) {
        println_log("SDHC");
    } else {
        println_log("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    printf_log("SD Card Size: %lluMB\n", cardSize);

    printf_log("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    printf_log("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
    return true;
}

void printf_log(const char *format, ...) {
    char buf[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buf, 256, format, args);
    va_end(args);
    Serial.print(buf);
    canvas.printf(buf);
    canvas.pushSprite(0, 0);
}

void println_log(const char *str) {
    Serial.println(str);
    canvas.println(str);
    canvas.pushSprite(0, 0);
}
