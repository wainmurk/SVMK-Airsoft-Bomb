void domination() {

  //SETUP INITIAL TIME
  int minutos = GAMEMINUTES - 1;
  boolean showGameTime = true;
  unsigned long a;
  unsigned long iTime = millis();  //  initialTime in millisec
  unsigned long aTime;
  redTime = 0;
  greenTime = 0;

  int largoTono = 50;
  // 0 = neutral, 1 = green team, 2 = red team
  a = millis();
  //Starting Game Code
  strip.clear();
  strip.show();
  while (1)  // this is the important code, is a little messy but works good.
  {
    keypad.getKey();
    aTime = millis() - iTime;
    //Code for led blinking
    timeCalcVar = (millis() - iTime) % 1000;
    if (timeCalcVar >= 0 && timeCalcVar <= 40) {
      if (team == 1) {
        strip.clear();  // заливаем жёлтым
        strip.show();
      }
      if (team == 2) {
        strip.clear();  // заливаем жёлтым
        strip.show();
      }
    }
    if (timeCalcVar >= 50 && timeCalcVar <= 100) {
      if (team == 1) {
        writeteamcolor(0);
      }
      if (team == 2) {
        writeteamcolor(1);
      }
    }
    // Sound!!! same as Destroy
    if (timeCalcVar >= 0 && timeCalcVar <= 40 && soundEnable) tone(tonepin, tonoActivada, largoTono);

    if (timeCalcVar >= 245 && timeCalcVar <= 255 && minutos - aTime / 60000 < 2 && soundEnable) tone(tonepin, tonoActivada, largoTono);
    if (timeCalcVar >= 495 && timeCalcVar <= 510 && minutos - aTime / 60000 < 4 && soundEnable) tone(tonepin, tonoActivada, largoTono);
    if (timeCalcVar >= 745 && timeCalcVar <= 760 && minutos - aTime / 60000 < 2 && soundEnable) tone(tonepin, tonoActivada, largoTono);
    //Help to count 3 secs
    if (a + 2000 < millis()) {
      a = millis();
      showGameTime = !showGameTime;
      cls();
    }
    //THE NEXT TO METHODS SHOW "GAME TIME" AND "CONTROLED ZONE TIME" IT SHOWS 2 AND 2 SEC EACH

    if (showGameTime) {  //THE SECOND IS /2
      lcd.setCursor(0, 0);
      lcdprint(gametime);
      lcd.setCursor(3, 1);
      printTime(minutos, aTime);
    } else if (!showGameTime) {

      lcd.setCursor(1, 0);
      if (team == 0) lcdprint(neutral);
      if (team == 1) teamzone(0);
      if (team == 2) teamzone(1);

      if (team > 0) {
        lcd.setCursor(3, 1);
        printTimeDom(millis() - iZoneTime, true);
      }
    }

    //###########################CHECKINGS##################

    //Check If Game End
    if (minutos - aTime / 60000 == 0 && 59 - ((aTime / 1000) % 60) == 0 or minutos - aTime / 60000 > 40000000) {
      gameOver();
    }

    //Check If IS neutral
    while ((defuseando || cancelando) && team > 0) {
      cls();
      lcd.setCursor(3, 0);
      if (team > 0) lcdprint(neutralizing);
      lcd.setCursor(0, 1);
      unsigned int percent = 0;
      unsigned long xTime = millis();  //start disabling time
      while (defuseando || cancelando) {
        keypad.getKey();
        timeCalcVar = (millis() - xTime) % 1000;

        if (timeCalcVar >= 0 && timeCalcVar <= 20) {
          if (soundEnable) tone(tonepin, tonoAlarma1, 200);
        }
        if (timeCalcVar >= 480 && timeCalcVar <= 500) {
          if (soundEnable) tone(tonepin, tonoAlarma2, 200);
          strip.clear();
          strip.show();
        }

        unsigned long seconds = millis() - xTime;
        percent = (seconds) / (ACTIVATESECONDS * 10);
        drawPorcent(percent);

        if (percent >= 100) {
          delay(1000);

          if (team == 1) {
            greenTime += millis() - iZoneTime;
            iZoneTime = 0;
          }
          if (team == 2) {
            redTime += millis() - iZoneTime;
            iZoneTime = 0;
          }
          team = 0;
          break;
        }
      }
      cls();
    }

    //Capturing red

    while (defuseando && team == 0) {
      cls();
      lcd.setCursor(2, 0);
      if (team == 0) lcdprint(capturing);
      lcd.setCursor(0, 1);
      unsigned int percent = 0;
      unsigned long xTime = millis();  //start disabling time
      while (defuseando) {
        keypad.getKey();
        timeCalcVar = (millis() - xTime) % 1000;

        if (timeCalcVar >= 0 && timeCalcVar <= 20) {
          strip.clear();
          strip.show();
          if (soundEnable) tone(tonepin, tonoAlarma1, 200);
        }
        if (timeCalcVar >= 480 && timeCalcVar <= 500) {
          if (soundEnable) tone(tonepin, tonoAlarma2, 200);
          writeteamcolor(1);
        }

        unsigned long seconds = millis() - xTime;
        percent = (seconds) / (ACTIVATESECONDS * 10);
        drawPorcent(percent);

        if (percent >= 100) {
          writeteamcolor(1);
          team = 2;
          iZoneTime = millis();
          delay(1000);
          break;
        }
      }
      cls();
      strip.clear();
      strip.show();
    }

    //getting to green zone
    while (cancelando && team == 0) {
      cls();
      lcd.setCursor(2, 0);
      if (team == 0) lcdprint(capturing);
      lcd.setCursor(0, 1);
      unsigned int percent = 0;
      unsigned long xTime = millis();  //start disabling time
      while (cancelando) {
        keypad.getKey();
        timeCalcVar = (millis() - xTime) % 1000;

        if (timeCalcVar >= 0 && timeCalcVar <= 20) {
          strip.clear();  // заливаем жёлтым
          strip.show();
          if (soundEnable) tone(tonepin, tonoAlarma1, 200);
        }
        if (timeCalcVar >= 480 && timeCalcVar <= 500) {
          if (soundEnable) tone(tonepin, tonoAlarma2, 200);
          writeteamcolor(0);
        }

        unsigned long seconds = millis() - xTime;
        percent = (seconds) / (ACTIVATESECONDS * 10);
        drawPorcent(percent);

        if (percent >= 100) {
          writeteamcolor(0);
          team = 1;
          iZoneTime = millis();
          delay(1000);
          break;
        }
      }
      cls();
      strip.clear();
      strip.show();
    }
  }
}

void gameOver() {
  int endc;
  if (team == 1) {
    greenTime += millis() - iZoneTime;
    writeteamcolor(0);
  }
  if (team == 2) {
    redTime += millis() - iZoneTime;
    writeteamcolor(1);
  }

  delay(100);
  tone(tonepin, 2500, 100);
  delay(100);
  tone(tonepin, 3500, 100);
  while (keypad.getKey() != BT_SEL) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcdprint(endtime);
    lcd.setCursor(0, 1);

    //check who team win the base
    if (greenTime > redTime) {
      //greenteam wins
      teamwin(0);
      writeteamcolor(0);
    } else {
      //redteam wins
      teamwin(1);
      writeteamcolor(1);
    }
    delay(3000);
    if (keypad.getKey() == BT_SEL) break;
    cls();
    teamtime(1);
    lcd.setCursor(5, 1);
    printTimeDom(redTime, false);
    delay(3000);
    if (keypad.getKey() == BT_SEL) break;
    cls();
    teamtime(0);
    lcd.setCursor(5, 1);
    printTimeDom(greenTime, false);
    delay(3000);
    if (keypad.getKey() == BT_SEL) break;
    cls();
    lcd.setCursor(2, 0);
    lcdprint(pressa);
    lcd.setCursor(3, 1);
    lcdprint(toleave);
    delay(3000);
    if (keypad.getKey() == BT_SEL) break;
  }
  redTime = 0;
  greenTime = 0;
  team = 0;
  lcd.clear();
  lcdprint(playagain);
  lcd.setCursor(0, 1);
  lcdprint(yesno);
  while (1) {
    var = keypad.waitForKey();
    if (var == BT_SEL) {
      tone(tonepin, 2400, 30);
      strip.clear();
      strip.show();
      lcd.clear();
      startGameCount();
      domination();
      break;
    }
    if (var == BT_CANCEL) {
      tone(tonepin, 2400, 30);
      strip.clear();
      strip.show();
      menuPrincipal();
      break;
    }
  }
}
