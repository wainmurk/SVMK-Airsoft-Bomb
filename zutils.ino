
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
