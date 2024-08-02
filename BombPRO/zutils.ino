
void drawPorcent(byte porcent) {
  //TODO: Optimize this code
  byte i = 1;
  int aDibujar = (8 * porcent) / 10;
  lcd.setCursor(0, 1);

  if (aDibujar < 5) {
    switch (aDibujar) {
      case 0:
        break;
      case 1:
        lcd.write((uint8_t)0);
        break;
      case 2:
        lcd.write(1);
        break;
      case 3:
        lcd.write(2);
        break;
      case 4:
        lcd.write(3);
        break;
    }
  }
  while (aDibujar >= 5) {
    if (aDibujar >= 5) {
      lcd.write(4);
      aDibujar -= 5;
    }
    if (aDibujar < 5) {
      switch (aDibujar) {
        case 0:
          break;
        case 1:
          lcd.write((uint8_t)0);
          break;
        case 2:
          lcd.write(1);
          break;
        case 3:
          lcd.write(2);
          break;
        case 4:
          lcd.write(3);
          break;
      }
    }
  }
}
void cls() {
  lcd.clear();
  lcd.setCursor(0, 0);
}

void debugtest() {
  bool flag;
  cls();
  lcdprint(debug);
  lcd.setCursor(0, 1);
  lcdprint(yesno);
  var = keypad.waitForKey();
  if (var == BT_SEL) {
    cls();
    lcdprint(dodefaults);

    while (1) {
      var = keypad.waitForKey();
      if (var == BT_SEL) {
        cls();
        lcd.setCursor(4, 0);
        SetDefaultValues();
        delay(10);
        lcdprint(done);
        delay(500);
        break;
      }
      if (var == BT_CANCEL) {
        break;
      }
      if (var == BT_DOWN) return;
    }

    cls();
    lcdprint(testrelay);
    lcd.print("?");

    while (1) {
      var = keypad.waitForKey();
      if (var == BT_SEL) {
        flag = !flag;
        if (flag) {
          cls();
          lcdprint(relayon);
          digitalWrite(mosfet, !rtype);
        } else {
          cls();
          lcdprint(relayoff);
          digitalWrite(mosfet, rtype);
        }
      }
      if (var == BT_CANCEL) {
        digitalWrite(mosfet, rtype);
        break;
      }
      if (var == BT_DOWN) {
        digitalWrite(mosfet, rtype);
        return;
      }
    }
    SetReal5v();
    cls();
    lcdprint(testbl);
    lcd.print("?");

    while (1) {
      var = keypad.waitForKey();
      if (var == BT_SEL) {
        DoTestBacklight();
      }
      if (var == BT_CANCEL) {
        break;
      }
    }
    if (var == BT_CANCEL) {
      return;
    }
    if (var == BT_DOWN) return;
  }
}

void SetDefaultValues() {
  EEPROM.put(10, 100);
  EEPROM.put(20, 5);
  EEPROM.put(25, 2);
  EEPROM.put(30, 1);
  writeIntIntoEEPROM(40, 5000);
  delay(20);
  EEPROM.get(10, bright);
  EEPROM.get(15, GAMEHOURS);
  EEPROM.get(20, GAMEMINUTES);
  EEPROM.get(25, colorofteama);
  EEPROM.get(30, colorofteamb);
  real_5v = readIntFromEEPROM(40);
}

void DoTestBacklight() {
  int testbltime = 200;
  strip.clear();
  strip.fill(mRed);
  strip.show();
  delay(testbltime);
  strip.clear();
  strip.fill(mYellow);
  strip.show();
  delay(testbltime);
  strip.clear();
  strip.fill(mBlue);
  strip.show();
  delay(testbltime);
  strip.clear();
  strip.fill(mPurple);
  strip.show();
  delay(testbltime);
  strip.clear();
  strip.fill(mGreen);
  strip.show();
  delay(testbltime);
  strip.clear();
  strip.fill(mWhite);
  strip.show();
  delay(testbltime);
  strip.clear();
  strip.show();
}

void SetReal5v() {
  cls();
  input_voltage = (analogRead(A0) * (real_5v / 1000.0)) / 1023.0;  //вольты
  lcdprint(battery);
  lcd.setCursor(8, 0);
  lcd.print(input_voltage);
  lcd.setCursor(13, 0);
  lcd.print("?");

  while (1) {
    var = keypad.waitForKey();

    if (var == BT_CANCEL) {
      cls();
      lcdprint(setreal5vtext);

      int digits[4] = { 0, 0, 0, 0 };
      int index = 0;

      while (index < 4) {
        char key = keypad.getKey();
        if (key != NO_KEY && isDigit(key)) {
          digits[index] = key - '0';  // Преобразуем символ цифры в целое число
          lcd.setCursor(index, 1);    // Устанавливаем курсор на второй строке
          lcd.print(key);             // Выводим цифру на дисплей
          index++;
        }
        if (index == 4) delay(300);
      }

      // Преобразование массива цифр в 4-значное число и запись в глобальную переменную
      real_5v = 0;
      for (int i = 0; i < 4; i++) {
        real_5v = real_5v * 10 + digits[i];
      }
      cls();
      writeIntIntoEEPROM(40, real_5v);
      delay(20);
      SetReal5v();
      break;
    }

    if (var == BT_SEL) {
      break;
    }
    if (var == BT_DOWN) return;
  }
}


int readIntFromEEPROM(int address) {
  byte byte1;
  byte byte2;
  EEPROM.get(address, byte1);
  EEPROM.get(address + 1, byte2);
  return (byte1 << 8) + byte2;
}

void writeIntIntoEEPROM(int address, int number) {
  EEPROM.put(address, number >> 8);
  EEPROM.put(address + 1, number & 0xFF);
}


void printTime(unsigned long minutos, unsigned long aTiempo) {
  //minutes
  if ((minutos - aTiempo / 60000) < 10) {
    lcd.print(F("0"));
    lcd.print(minutos - aTiempo / 60000);
  } else {
    lcd.print(minutos - aTiempo / 60000);
  }
  lcd.print(F(":"));
  //seconds
  if ((59 - ((aTiempo / 1000) % 60)) < 10) {
    lcd.print(F("0"));
    lcd.print(59 - ((aTiempo / 1000) % 60));
  } else {
    lcd.print(59 - ((aTiempo / 1000) % 60));
  }
  lcd.print(F(":"));
  //this not mach with real time, is just a effect, it says 999 because millis%1000 sometimes give 0 LOL
  lcd.print(999 - (millis() % 1000));
}

void printTimeDom(unsigned long aTiempo, boolean showMillis) {
  //minutes
  if ((aTiempo / 60000) < 10) {
    lcd.print(F("0"));
    lcd.print(aTiempo / 60000);
  } else {
    lcd.print(aTiempo / 60000);
  }
  lcd.print(F(":"));
  //seconds
  if (((aTiempo / 1000) % 60) < 10) {
    lcd.print(F("0"));
    lcd.print((aTiempo / 1000) % 60);
  } else {
    lcd.print((aTiempo / 1000) % 60);
  }
  if (showMillis) {
    lcd.print(F(":"));
    //this not mach with real time, is just a effect, it says 999 because millis%1000 sometimes give 0 LOL
    lcd.print(999 - millis() % 1000);
  }
}


void startGameCount() {
  cls();
  lcd.setCursor(0, 0);
  lcdprint(readytobegin);
  lcd.setCursor(0, 1);
  lcdprint(pressbutton);
  keypad.waitForKey();  //if you press a button game start

  cls();
  lcd.setCursor(2, 0);
  lcdprint(gamestarting);
  for (int i = 5; i > 0; i--) {  // START COUNT GAME INIT
    lcd.setCursor(2, 1);
    tone(tonepin, 2000, 100);
    lcdprint(in);
    lcd.print("   ");
    lcd.print(i);
    delay(1000);
    if (i == 1) {
      tone(tonepin, 3000, 100);
    }
  }
  cls();
}

void writeteamcolor(bool team) {
  int color;
  if (team == 1) color = colorofteama;
  if (team == 0) color = colorofteamb;
  if (color > 6 or color < 0) color = 0;
  switch (color) {
    case 0:
      strip.clear();
      strip.fill(mRed);
      strip.show();
      break;
    case 1:
      strip.clear();
      strip.fill(mYellow);
      strip.show();
      break;
    case 2:
      strip.clear();
      strip.fill(mBlue);
      strip.show();
      break;
    case 3:
      strip.clear();
      strip.fill(mPurple);
      strip.show();
      break;
    case 4:
      strip.clear();
      strip.fill(mGreen);
      strip.show();
      break;
    case 5:
      strip.clear();
      strip.fill(mWhite);
      strip.show();
      break;
  }
}


void teamwin(bool team) {
  int text;
  if (team == 1) text = colorofteama;
  if (team == 0) text = colorofteamb;
  if (text > 6 or text < 0) text = 0;
  switch (text) {
    case 0:
      lcdprint(redwins);
      break;
    case 1:
      lcdprint(yellowwins);
      break;
    case 2:
      lcdprint(bluewins);
      break;
    case 3:
      lcdprint(purplewins);
      break;
    case 4:
      lcdprint(greenwins);
      break;
    case 5:
      lcdprint(whitewins);
      break;
  }
}


void teamtime(bool team) {
  int text;
  if (team == 1) text = colorofteama;
  if (team == 0) text = colorofteamb;
  if (text > 6 or text < 0) text = 0;
  switch (text) {
    case 0:
      lcdprint(redtime);
      break;
    case 1:
      lcdprint(yellowtime);
      break;
    case 2:
      lcdprint(bluetime);
      break;
    case 3:
      lcdprint(purpletime);
      break;
    case 4:
      lcdprint(greentime);
      break;
    case 5:
      lcdprint(whitetime);
      break;
  }
}

void teamzone(bool team) {
  int text;
  if (team == 1) text = colorofteama;
  if (team == 0) text = colorofteamb;
  if (text > 6 or text < 0) text = 0;
  switch (text) {
    case 0:
      lcdprint(redzone);
      break;
    case 1:
      lcdprint(yellowzone);
      break;
    case 2:
      lcdprint(bluezone);
      break;
    case 3:
      lcdprint(purplezone);
      break;
    case 4:
      lcdprint(greenzone);
      break;
    case 5:
      lcdprint(whitezone);
      break;
  }
}






void checkArrows(byte i, byte maxx) {

  if (i == 0) {
    lcd.setCursor(15, 1);
    lcd.write(6);
  }
  if (i == maxx) {
    lcd.setCursor(15, 0);
    lcd.write(5);
  }
  if (i > 0 && i < maxx) {
    lcd.setCursor(15, 1);
    lcd.write(6);
    lcd.setCursor(15, 0);
    lcd.write(5);
  }
}

void lcdprint(const uint8_t text[]) {

  for (int i = 0; i < 16; i++) {
    if (text[i] == 0x00) break;
    lcd.write(text[i]);
  }
}


void activateMosfet() {

  //lcd.print(F("Mosfet ON!"));
  digitalWrite(mosfet, !rtype);  // turn the LED on (HIGH is the voltage level)
  delay(MOSFET_TIME);            // wait for 4 second
  //lcd.print(F("Mosfet OFF!"));
  digitalWrite(mosfet, rtype);
}
