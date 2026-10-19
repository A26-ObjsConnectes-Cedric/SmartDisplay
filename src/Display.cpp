#include "outputs/Display.h"
#include <Wire.h>

#include "config/Config.h"
#include "outputs/Icons.h"

namespace {

// Mise en page (écran 128x64) : en-tête de 0 à 30 px, texte du message en dessous
const int16_t TITLE_X = 32;
const int16_t TITLE_Y = 6;
const int16_t SEPARATOR_Y = 30;
const int16_t BODY_X = 3;
const int16_t BODY_Y = 36;
const int16_t LINE_HEIGHT = 8;

// Police par défaut (taille 1) : 6 px par caractère, soit 20 caractères par ligne
// à partir de BODY_X. Trois lignes permettent d'afficher les 60 caractères maximum
// d'un message valide (Message::MAX_TEXT_LENGTH).
const unsigned int LINE_CHARS = 20;
const int MAX_LINES = 3;

// Découpe 'text' en lignes d'au plus LINE_CHARS caractères (positions début/fin).
// Si breakOnSpaces est vrai, on coupe de préférence sur une espace.
// Retourne true si tout le texte tient dans MAX_LINES lignes.
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
    // Les espaces de coupure ne s'affichent pas en début de ligne suivante
    while (breakOnSpaces && start < length && text.charAt(start) == ' ') {
      start++;
    }
  }
  return start >= length;
}

}  // namespace

// Le constructeur ne touche pas au matériel : le bus I2C est démarré dans begin()
Display::Display()
    : screen(Config::OLED_WIDTH, Config::OLED_HEIGHT, &Wire, -1), ready(false) {}

bool Display::begin() {
  Wire.begin(Config::OLED_SDA_PIN, Config::OLED_SCL_PIN);

  // La bibliothèque ne détecte pas un écran absent : on vérifie la présence sur le bus
  Wire.beginTransmission(Config::OLED_I2C_ADDRESS);
  if (Wire.endTransmission() != 0) {
    return false;
  }

  // Les deux derniers paramètres : réinitialiser l'écran, mais ne pas relancer Wire.begin()
  if (!screen.begin(SSD1306_SWITCHCAPVCC, Config::OLED_I2C_ADDRESS, true, false)) {
    return false;
  }

  screen.setTextColor(SSD1306_WHITE);
  ready = true;
  showIdle();
  return true;
}

void Display::drawHeader(const unsigned char* icon, const char* title) {
  screen.drawBitmap(2, 2, icon, Icons::SIZE, Icons::SIZE, SSD1306_WHITE);
  screen.setTextSize(2);
  screen.setCursor(TITLE_X, TITLE_Y);
  screen.print(title);
  screen.drawFastHLine(0, SEPARATOR_Y, Config::OLED_WIDTH, SSD1306_WHITE);
}

// Écrit le texte sur 3 lignes maximum, positionnées une à une.
// On n'utilise pas le retour à la ligne automatique de la bibliothèque : il ramène
// le curseur à x = 0 (décalage avec la première ligne, et contact avec le cadre d'alerte).
void Display::drawBody(const String& text) {
  unsigned int starts[MAX_LINES];
  unsigned int ends[MAX_LINES];
  int lineCount = 0;

  // D'abord une coupure sur les espaces ; si le texte ne tient pas, coupure fixe
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

  // Un cadre autour de l'écran distingue visuellement une alerte
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
