#include "outputs/Display.h"

#include <Wire.h>

#include "config/Config.h"
#include "outputs/Icons.h"

namespace {

const int16_t TITLE_X = 32;
const int16_t TITLE_Y = 6;
const int16_t SEPARATOR_Y = 30;
const int16_t BODY_X = 3;
const int16_t BODY_Y = 36;
const int16_t LINE_HEIGHT = 8;


const unsigned int LINE_CHARS = 20;
const int MAX_LINES = 3;

bool wrapText(const String& text, bool breakOnSpaces, unsigned int starts[],
              unsigned int ends[], int& lineCount) {
  unsigned int length = text.length();
  unsigned int start = 0;
  lineCount = 0;

  while (start < length && lineCount < MAX_LINES) {
    unsigned int end = start + LINE_CHARS;
    if (end >= length) {
      end = length;
    } else if (breakOnSpaces) {
      int space = text.lastIndexOf(' ', end);
      if (space > static_cast<int>(start)) {
        end = static_cast<unsigned int>(space);
      }
    }

    starts[lineCount] = start;
    ends[lineCount] = end;
    lineCount++;

    start = end;
    while (breakOnSpaces && start < length && text.charAt(start) == ' ') {
      start++;
    }
  }
  return start >= length;
}

}  // namespace

Display::Display()
    : screen(Config::OLED_WIDTH, Config::OLED_HEIGHT, &Wire, -1), ready(false) {}

bool Display::begin() {
  Wire.begin(Config::OLED_SDA_PIN, Config::OLED_SCL_PIN);

  Wire.beginTransmission(Config::OLED_I2C_ADDRESS);
  if (Wire.endTransmission() != 0) {
    return false;
  }

  if (!screen.begin(SSD1306_SWITCHCAPVCC, Config::OLED_I2C_ADDRESS, true, false)) {
    return false;
  }

  screen.setTextColor(SSD1306_WHITE);
  ready = true;
  setBrightness(Config::DEFAULT_BRIGHTNESS_PCT);
  showIdle();
  return true;
}

void Display::setBrightness(uint8_t percent) {
  if (!ready) return;

  if (percent > Config::MAX_BRIGHTNESS_PCT) {
    percent = Config::MAX_BRIGHTNESS_PCT;
  }
  uint8_t contrast = static_cast<uint8_t>(percent * 255 / 100);
  screen.ssd1306_command(SSD1306_SETCONTRAST);
  screen.ssd1306_command(contrast);
}

void Display::drawHeader(const unsigned char* icon, const char* title) {
  screen.drawBitmap(2, 2, icon, Icons::SIZE, Icons::SIZE, SSD1306_WHITE);
  screen.setTextSize(2);
  screen.setCursor(TITLE_X, TITLE_Y);
  screen.print(title);
  screen.drawFastHLine(0, SEPARATOR_Y, Config::OLED_WIDTH, SSD1306_WHITE);
}

void Display::drawBody(const String& text) {
  unsigned int starts[MAX_LINES];
  unsigned int ends[MAX_LINES];
  int lineCount = 0;

  if (!wrapText(text, true, starts, ends, lineCount)) {
    wrapText(text, false, starts, ends, lineCount);
  }

  screen.setTextSize(1);
  for (int i = 0; i < lineCount; i++) {
    screen.setCursor(BODY_X, BODY_Y + i * LINE_HEIGHT);
    screen.print(text.substring(starts[i], ends[i]));
  }
}

void Display::showIdle() {
  if (!ready) return;

  screen.clearDisplay();
  drawHeader(Icons::IDLE, "READY");
  screen.setTextSize(1);
  screen.setCursor(BODY_X, BODY_Y);
  screen.println("SmartDisplay");
  screen.setCursor(BODY_X, BODY_Y + 10);
  screen.print("Waiting for message");
  screen.display();
}

void Display::showMessage(const Message& message) {
  if (!ready) return;

  bool isAlert = message.getType() == MessageType::ALERT;

  screen.clearDisplay();
  drawHeader(isAlert ? Icons::ALERT : Icons::INFO, isAlert ? "ALERT" : "INFO");
  drawBody(message.getText());

  if (isAlert) {
    screen.drawRect(0, 0, Config::OLED_WIDTH, Config::OLED_HEIGHT, SSD1306_WHITE);
  }
  screen.display();
}

void Display::showError(const String& reason) {
  if (!ready) return;

  screen.clearDisplay();
  drawHeader(Icons::ERROR, "ERROR");
  drawBody(reason);
  screen.display();
}
