//##################MENUS###############################

void menuPrincipal() {  //MAIN MENU
  strip.clear();
  //Draw menu
  cls();  //clear lcd and set cursor to 0,0
  int i = 0;
  char* menu1[] = { textsearchdestroy, textsabotage, textdomination, settings };  // HERE YOU CAN ADD MORE ITEMS ON THE MAIN MENU
  lcdprint(menu1[i]);
  lcd.setCursor(15, 1);
  checkArrows(i, 2);
  while (1) {

    var = keypad.waitForKey();
    if (var == BT_UP && i > 0) {
      tone(tonepin, 2400, 30);
      i--;
      cls();
      lcdprint(menu1[i]);
      checkArrows(i, 3);
      delay(100);
    }
    if (var == BT_DOWN && i < 3) {
      tone(tonepin, 2400, 30);
      i++;
      cls();
      lcdprint(menu1[i]);
      checkArrows(i, 3);
      delay(100);
    }

    if (var == BT_SEL) {
      tone(tonepin, 2400, 30);
      cls();
      switch (i) {

        case 0:
          sdStatus = true;
          configQuickGame();
          startGameCount();
          search();
          break;
        case 1:
          saStatus = true;
          configQuickGame();
          startGameCount();
          sabotage();
          break;
        case 2:
          doStatus = true;
          configQuickGame();
          startGameCount();
          domination();
          break;
        case 3:
          config();
          break;
      }
    }
  }
}

void config() {
  //Draw menu
  lcd.clear();
  lcd.setCursor(0, 0);
  int i = 0;
  char* menu2[] = { teamcolors, defaulttime, testbl, setbrightness, testrelay };  // HERE YOU CAN ADD MORE ITEMS ON THE MENU
  delay(500);
  lcd.print(menu2[i]);
  checkArrows(i, 4);

  while (1) {
    var = keypad.waitForKey();
    if (var == BT_UP && i > 0) {
      tone(tonepin, 2400, 30);
      i--;
      lcd.clear();
      lcdprint(menu2[i]);
      checkArrows(i, 3);
      delay(50);
    }
    if (var == BT_DOWN && i < 4) {
      tone(tonepin, 2400, 30);
      i++;
      lcd.clear();
      lcdprint(menu2[i]);
      checkArrows(i, 3);
      delay(50);
    }
    if (var == BT_CANCEL) {
      tone(tonepin, 2400, 30);
      menuPrincipal();
    }
    if (var == BT_SEL) {
      tone(tonepin, 2400, 30);
      lcd.clear();
      switch (i) {
        case 0: changeteamcolors(); break;
        case 1:
          lcdprint(setgametime);
          delay(100);
          while (1) {
            lcd.clear();
            lcdprint(setgametime);
            delay(100);

            lcd.setCursor(0, 1);
            lcd.print(GAMEMINUTES);
            lcd.setCursor(5, 1);
            lcdprint(minutes);


            var = keypad.waitForKey();
            if (var == BT_UP) {
              GAMEMINUTES += 1;
              tone(tonepin, 2400, 30);
            }
            if (var == BT_DOWN and GAMEMINUTES > 1) {
              GAMEMINUTES -= 1;
              tone(tonepin, 2400, 30);
            }
            if (var == BT_SEL) {
              tone(tonepin, 2400, 30);
              EEPROM.put(20, GAMEMINUTES);
              delay(50);
              config();
            }
            if (var == BT_CANCEL) {
              tone(tonepin, 2400, 30);
              EEPROM.get(20, GAMEMINUTES);
              delay(50);
              config();
            }
          }

          break;
        case 2:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcdprint(testbl);
          for (int i = 0; i < NUMLEDS; i++) strip.set(i, mWheel8(i * 255 / NUMLEDS));  // полный круг от 0 до 255
          strip.show();
          delay(2000);
          strip.clear();
          strip.show();
          config();
          break;

        case 3:
          //gameConfigMenu
          lcdprint(setbrightness);
          delay(100);
          while (1) {
            lcd.clear();
            lcdprint(setbrightness);
            delay(100);

            lcd.setCursor(0, 1);
            lcd.print(bright);
            lcd.setCursor(3, 1);
            lcd.print("%");
            strip.setBrightness(map(bright, 0, 100, 0, 255));
            strip.clear();
            strip.fill(mWhite);
            strip.show();
            delay(200);
            var = keypad.waitForKey();
            if (var == BT_UP) {
              bright += 10;
              tone(tonepin, 2400, 30);
            }
            if (var == BT_DOWN) {
              bright -= 10;
              tone(tonepin, 2400, 30);
            }
            if (var == BT_SEL) {
              tone(tonepin, 2400, 30);
              strip.setBrightness(map(bright, 0, 100, 0, 255));
              EEPROM.put(10, bright);
              strip.clear();
              strip.show();
              delay(50);
              config();
            }
            if (var == BT_CANCEL) {
              tone(tonepin, 2400, 30);
              EEPROM.get(10, bright);
              strip.setBrightness(map(bright, 0, 100, 0, 255));
              strip.clear();
              strip.show();
              delay(50);

              config();
            }
            if (bright > 100) bright = 0;
            if (bright < 0) bright = 100;
          }

          break;

        case 4:
          //test rele menu
          cls();
          lcdprint(relayon);
          digitalWrite(mosfet, !rtype);  // turn the LED on (HIGH is the voltage level)
          delay(3000);                   // wait for 3 second
          cls();
          lcdprint(relayoff);
          digitalWrite(mosfet, rtype);
          delay(2000);
          config();
          break;
      }
    }
  }
}


void changeteamcolors() {
  lcd.clear();
  lcd.setCursor(0, 0);
  int i = 0;
  char* menu3[] = { teama, teamb };  // HERE YOU CAN ADD MORE ITEMS ON THE MENU
  delay(500);
  lcd.print(menu3[i]);
  checkArrows(i, 4);

  while (1) {
    var = keypad.waitForKey();
    if (var == BT_UP && i > 0) {
      tone(tonepin, 2400, 30);
      i--;
      lcd.clear();
      lcdprint(menu3[i]);
      checkArrows(i, 1);
      delay(50);
    }
    if (var == BT_DOWN && i < 1) {
      tone(tonepin, 2400, 30);
      i++;
      lcd.clear();
      lcdprint(menu3[i]);
      checkArrows(i, 1);
      delay(50);
    }
    if (var == BT_CANCEL) {
      tone(tonepin, 2400, 30);
      config();
    }
    if (var == BT_SEL) {
      tone(tonepin, 2400, 30);
      lcd.clear();

      switch (i) {
        case 0: selteam = 0; break;
        case 1: selteam = 1; break;
      }
      selectcolor(selteam);
    }
  }
}

void selectcolor(bool selectedteam) {
  lcd.clear();
  lcd.setCursor(0, 0);
  int i;
  if (selectedteam == 1) { i = colorofteama; }
  if (selectedteam == 0) { i = colorofteamb; }
  if (i > 6 or i < 0) i = 0;

  char* menu4[] = { red, yellow, blue, purple, green, white };  // HERE YOU CAN ADD MORE ITEMS ON THE MENU
  delay(500);
  lcd.print(menu4[i]);
  checkArrows(i, 5);

  while (1) {
    switch (i) {
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



    var = keypad.waitForKey();
    if (var == BT_UP && i > 0) {
      tone(tonepin, 2400, 30);
      i--;
      lcd.clear();
      lcdprint(menu4[i]);
      checkArrows(i, 5);
      delay(50);
    }
    if (var == BT_DOWN && i < 5) {
      tone(tonepin, 2400, 30);
      i++;
      lcd.clear();
      lcdprint(menu4[i]);
      checkArrows(i, 5);
      delay(50);
    }
    if (var == BT_CANCEL) {
      tone(tonepin, 2400, 30);
      strip.clear();
      strip.show();
      changeteamcolors();
    }
    if (var == BT_SEL) {
      tone(tonepin, 2400, 30);
      strip.clear();
      strip.show();
      lcd.clear();
      if (selectedteam == 1) {
        colorofteama = i;
        EEPROM.put(25, colorofteama);
      }
      if (selectedteam == 0) {
        colorofteamb = i;
        EEPROM.put(30, colorofteamb);
      }
      changeteamcolors();
    }
  }
}







void configQuickGame() {

  cls();
  //GAME TIME
  if (sdStatus || doStatus || saStatus) {
    lcdprint(setgametime);
    lcd.setCursor(0, 1);
    checkArrows(1, 2);
    delay(100);

    while (1) {

      lcd.clear();
      lcdprint(setgametime);
      lcd.setCursor(0, 1);
      checkArrows(1, 2);
      lcd.setCursor(0, 1);
      lcd.print(GAMEMINUTES);
      lcd.setCursor(4, 1);
      lcdprint(minutes);

      var = keypad.waitForKey();

      if (var == BT_UP && GAMEMINUTES < 180) {
        tone(tonepin, 2400, 30);
        GAMEMINUTES++;
        delay(50);
      }
      if (var == BT_DOWN && GAMEMINUTES > 1) {
        tone(tonepin, 2400, 30);
        GAMEMINUTES--;
        delay(50);
      }
      if (var == BT_CANCEL)  // Cancel or Back Button :')
      {
        tone(tonepin, 2400, 30);
        menuPrincipal();
      }
      if (var == BT_SEL)  // Cancel or Back Button :')
      {
        tone(tonepin, 2400, 30);
        break;
      }
    }
    tone(tonepin, 2400, 30);
    cls();
  }
  //BOMB TIME
  if (sdStatus || saStatus) {
    lcdprint(bombtime);
    lcd.setCursor(0, 1);
    checkArrows(1, 2);
    delay(100);

    while (1) {
      lcd.clear();
      lcdprint(bombtime);
      lcd.setCursor(0, 1);
      checkArrows(1, 2);
      lcd.setCursor(0, 1);
      lcd.print(BOMBMINUTES);
      lcd.setCursor(4, 1);
      lcdprint(minutes);
      var = keypad.waitForKey();


      if (var == BT_DOWN && BOMBMINUTES > 1) {
        tone(tonepin, 2400, 30);
        BOMBMINUTES--;
        delay(50);
      }
      if (var == BT_UP && BOMBMINUTES < 20) {
        tone(tonepin, 2400, 30);
        BOMBMINUTES++;
        delay(50);
      }
      if (var == BT_CANCEL)  // Cancel or Back Button :')
      {
        tone(tonepin, 2400, 30);
        menuPrincipal();
      }
      if (var == BT_SEL)  // Cancel or Back Button :')
      {
        tone(tonepin, 2400, 30);
        break;
      }
    }
    tone(tonepin, 2400, 30);
  }
  cls();
  //ARMING TIME
  if (sdStatus || doStatus || saStatus) {
    lcd.setCursor(2, 0);
    lcdprint(armtime);
    lcd.setCursor(0, 1);
    checkArrows(1, 2);
    delay(100);

    while (1) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcdprint(armtime);
      lcd.setCursor(0, 1);
      checkArrows(1, 2);
      lcd.setCursor(0, 1);
      lcd.print(ACTIVATESECONDS);
      lcd.setCursor(4, 1);
      lcdprint(seconds);
      var = keypad.waitForKey();

      if (var == BT_DOWN && ACTIVATESECONDS > 5) {
        tone(tonepin, 2400, 30);
        ACTIVATESECONDS--;
        delay(50);
      }
      if (var == BT_UP && ACTIVATESECONDS < 30) {
        ACTIVATESECONDS++;
        tone(tonepin, 2400, 30);
        delay(50);
      }
      if (var == BT_CANCEL)  // Cancel or Back Button :')
      {
        tone(tonepin, 2400, 30);
        menuPrincipal();
      }
      if (var == BT_SEL)  // Cancel or Back Button :')
      {
        tone(tonepin, 2400, 30);
        break;
      }
    }
    tone(tonepin, 2400, 30);
    ACTIVATESECONDS -= 1;  // Just a fix
  }
  //Want sound??
  if (sdStatus || saStatus || doStatus) {
    cls();
    lcdprint(actsound);
    lcd.setCursor(0, 1);
    lcdprint(yesno);


    while (1) {
      var = keypad.waitForKey();
      if (var == BT_SEL) {
        soundEnable = true;
        tone(tonepin, 2400, 30);
        break;
      }

      if (var == BT_CANCEL) {
        soundEnable = false;
        tone(tonepin, 2400, 30);
        break;
      }
    }
  }
  //Activate Mosfet at Terrorist game ends??? Boom!

  if (sdStatus || saStatus) {
    cls();
    lcdprint(actrelay);
    lcd.setCursor(0, 1);
    lcdprint(yesno);
    while (1) {
      var = keypad.waitForKey();
      if (var == BT_SEL) {
        mosfetEnable = true;
        tone(tonepin, 2400, 30);
        break;
      }
      if (var == BT_CANCEL) {
        mosfetEnable = false;
        tone(tonepin, 2400, 30);
        break;
      }
    }
  }
  //You Want a password enable-disable game?
  if (sdStatus || saStatus) {
    cls();
    lcdprint(actcode);
    lcd.setCursor(0, 1);
    lcdprint(yesno);

    while (1) {
      var = keypad.waitForKey();
      if (var == BT_SEL) {
        tone(tonepin, 2400, 30);
        setNewPass();
        passwordEnable = true;
        break;
      }
      if (var == BT_CANCEL) {
        tone(tonepin, 2400, 30);
        passwordEnable = false;
        break;
      }
    }
    tone(tonepin, 2400, 30);
  }
  //Continue the game :D
}
