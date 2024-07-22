void search() {
  cls();
  strip.fill(mPurple);  // заливаем жёлтым
  strip.show();
  //SETUP INITIAL TIME
  int minutos = GAMEMINUTES - 1;
  unsigned long iTime = millis();  //  initialTime in millisec
  unsigned long aTime;
  //var='o';

  //Starting Game Code
  while (1) {  // this is the important code, is a little messy but works good.
    //Code for led blinking
    timeCalcVar = (millis() - iTime) % 1000;
    if (timeCalcVar >= 0 && timeCalcVar <= 50) {
      strip.clear();  // заливаем жёлтым
      strip.show();
    }
    if (timeCalcVar >= 90 && timeCalcVar <= 130) {
      strip.fill(mGreen);  // заливаем жёлтым
      strip.show();
    }

    lcd.setCursor(0, 0);
    lcdprint(gametime);
    aTime = millis() - iTime;
    lcd.setCursor(3, 1);

    //PRINT TIME ON LCD

    printTime(minutos, aTime);

    //###########################CHECKINGS##################

    //Check If Game End
    if (minutos - aTime / 60000 == 0 && 59 - ((aTime / 1000) % 60) == 0) {
      lcd.clear();
      while (1) {
        lcdprint(endtime);
        lcd.setCursor(0, 1);
        lcd.print(F("DEFENDERES WIN "));

        delay(100);
        tone(tonepin, 2500, 100);
        delay(100);
        tone(tonepin, 3500, 100);

        for (int i = 1000; i > 200; i--) {
          if (soundEnable) tone(tonepin, i);
          delay(5);
        }
        noTone(tonepin);
        delay(500);
        cls();
        lcd.setCursor(4, 0);
        lcdprint(rematch);
        lcd.setCursor(0, 1);
        lcdprint(yesno);
        while (1) {
          var = keypad.waitForKey();
          if (var == BT_SEL) {
            tone(tonepin, 2400, 30);
            strip.clear();
            strip.show();  // вывод изменений на ленту
            //
            if (sdStatus) {
              startGameCount();
              search();
            }
            if (saStatus) {
              saStatus = true;
              startGameCount();
              start = true;  //
              sabotage();
            }
          }
          if (var == BT_CANCEL) {
            strip.clear();
            strip.show();  // вывод изменений на ленту
            tone(tonepin, 2400, 30);
            menuPrincipal();

            break;
          }
        }
      }
    }
    //Serial.println(keypad.getKey());
    //USED IN PASSWORD GAME
    if ('d' == keypad.getKey() && passwordEnable) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcdprint(armingbomb);
      delay(1000);  //a little delay to think in the password
      lcd.clear();
      lcd.setCursor(1, 0);
      lcdprint(enterpass);

      setCode();  // we need to set the comparation variable first it writes on enteredText[]

      //then compare :D

      if (comparePassword()) {
        destroy();
      }
      lcd.clear();
      lcd.setCursor(1, 0);
      lcdprint(passerror);
      if (soundEnable) tone(tonepin, errorTone, 200);
      delay(500);
      cls();
    }
    //Check If Is Activating
    while (defuseando && !passwordEnable) {
      strip.fill(mGreen);  // заливаем жёлтым
      strip.show();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcdprint(armingbomb);
      lcd.setCursor(0, 1);
      unsigned int percent = 0;
      unsigned long xTime = millis();  //start disabling time
      while (defuseando) {
        keypad.getKey();
        timeCalcVar = (millis() - xTime) % 1000;

        if (timeCalcVar >= 0 && timeCalcVar <= 40) {
          strip.clear();  // заливаем жёлтым
          strip.show();
          if (soundEnable) tone(tonepin, tonoAlarma1, 200);
        }
        if (timeCalcVar >= 480 && timeCalcVar <= 520) {
          if (soundEnable) tone(tonepin, tonoAlarma2, 200);
          strip.fill(mRed);
          strip.show();
        }

        unsigned long seconds = millis() - xTime;
        percent = (seconds) / (ACTIVATESECONDS * 10);
        drawPorcent(percent);

        if (percent >= 100) {
          strip.fill(mGreen);  // заливаем жёлтым
          strip.show();
          destroy();  // jump to the next gamemode
        }
      }
      cls();
      strip.fill(mRed);  // заливаем жёлтым
      strip.show();
    }
  }
}

void destroy() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcdprint(bombactivated);
  delay(1000);
  lcd.clear();
  int minutos = BOMBMINUTES - 1;
  unsigned long iTime = millis();
  unsigned long aTime;
  int largoTono = 50;

  //MAIN LOOP
  while (1) {

    //If you fail disarm.
    if (endGame) {
      explodeSplash();
    }

    //Led Blink

    timeCalcVar = (millis() - iTime) % 1000;
    if (timeCalcVar >= 0 && timeCalcVar <= 40) {
      strip.clear();  // заливаем жёлтым
      strip.show();
      if (soundEnable) tone(tonepin, tonoActivada, largoTono);
    }
    if (timeCalcVar >= 180 && timeCalcVar <= 220) {
      strip.fill(mRed);  // заливаем жёлтым
      strip.show();
    }
    //Sound
    aTime = millis() - iTime;
    timeCalcVar = (millis() - iTime) % 1000;
    if (timeCalcVar >= 245 && timeCalcVar <= 255 && minutos - aTime / 60000 < 2 && soundEnable) tone(tonepin, tonoActivada, largoTono);
    if (timeCalcVar >= 495 && timeCalcVar <= 510 && minutos - aTime / 60000 < 4 && soundEnable) tone(tonepin, tonoActivada, largoTono);
    if (timeCalcVar >= 745 && timeCalcVar <= 760 && minutos - aTime / 60000 < 2 && soundEnable) tone(tonepin, tonoActivada, largoTono);
    if (minutos - aTime / 60000 == 0 && 59 - ((aTime / 1000) % 60) < 10) largoTono = 300;

    lcd.setCursor(2, 0);
    lcdprint(expin);
    //Passed Time

    lcd.setCursor(3, 1);

    ////////HERE ARE THE TWO OPTIONS THAT ENDS THE GAME///////////

    ////TIME PASED AWAY AND THE BOMB EXPLODES
    if (minutos - aTime / 60000 == 0 && 59 - ((aTime / 1000) % 60) == 0)  // Check if game ends
    {
      explodeSplash();
    }
    //print time

    printTime(minutos, aTime);

    //// SECOND OPTION: YOU PRESS DISARMING BUTTON

    //IF IS A PASSWORD GAME

    if ('d' == keypad.getKey() && passwordEnable) {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcdprint(defusing);
      delay(1000);  //a little delay to think in the password

      lcd.clear();
      lcd.setCursor(1, 0);
      lcdprint(enterpass);

      setCode();  // we need to set the compare variable first

      //then compare :D

      if (comparePassword()) {
        disarmedSplash();
      }
      lcd.clear();
      lcd.setCursor(3, 0);
      lcdprint(passerror);
      if (soundEnable) tone(tonepin, errorTone, 200);
      delay(500);
      cls();
    }

    if (defuseando && !passwordEnable)  // disarming bomb
    {
      lcd.clear();
      strip.fill(mRed);  // заливаем жёлтым
      strip.show();
      lcd.setCursor(1, 0);
      lcdprint(longdefusing);
      lcd.setCursor(0, 1);
      unsigned int percent = 0;
      unsigned long xTime = millis();
      while (defuseando) {
        keypad.getKey();
        //check if game time runs out during the disabling
        aTime = millis() - iTime;
        if ((minutos - aTime / 60000 == 0 && 59 - ((aTime / 1000) % 60) == 0) || minutos - aTime / 60000 > 4000000000) {
          endGame = true;
        }
        timeCalcVar = (millis() - xTime) % 1000;
        if (timeCalcVar >= 0 && timeCalcVar <= 20) {
          strip.clear();  // заливаем жёлтым
          strip.show();
          if (soundEnable) tone(tonepin, tonoAlarma1, 200);
        }
        if (timeCalcVar >= 480 && timeCalcVar <= 500) {
          if (soundEnable) tone(tonepin, tonoAlarma2, 200);
          strip.fill(mGreen);  // заливаем жёлтым
          strip.show();
        }
        unsigned long seconds = (millis() - xTime);
        percent = seconds / (ACTIVATESECONDS * 10);
        drawPorcent(percent);

        //BOMB DISARMED GAME OVER
        if (percent >= 100) {
          disarmedSplash();
        }
      }
      strip.fill(mRed);  // заливаем жёлтым
      strip.show();
      cls();
    }
  }
}
