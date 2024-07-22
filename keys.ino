//Used to get keys, here you can configure how works the input without modify the other code

boolean isPressed(char key) {

  if (keypad.getKey() == key) {
    //    Serial.println(" TRUE");
    return true;
  } else if (keypad.getKey() == key && keypad.getState() == 2) {
    //    Serial.print(" Hold!!");
    //    Serial.println(key);
    return true;
  }
  //  Serial.println(" Falso");
  return false;
}
//This fuction compare enteredText[8] and password[8] variables
boolean comparePassword() {

  for (int i = 0; i < 8; i++) {
    if (enteredText[i] != password[i]) return false;
  }
  return true;
}

//Set the password variable
void setCode() {

  lcd.setCursor(0, 1);
  for (int i = 0; i < 8; i++) {
    while (1) {
      var = getNumber();
      if (var != 'x') {
        enteredText[i] = var;

        if (i != 0) {
          lcd.setCursor(i - 1, 1);
          lcd.print("*");
          lcd.print(var);
        } else {
          lcd.print(var);
        }
        tone(tonepin, 2400, 30);
        break;
      }
    }
    if (i == 7) delay(300);
  }
}
void setPass() {
  lcd.setCursor(0, 1);

  for (int i = 0; i < 8; i++) {
    while (1) {
      var = getNumber();
      if (var != 'x') {
        password[i] = var;
        if (i != 0) {
          lcd.setCursor(i - 1, 1);
          lcd.print("*");
          lcd.print(var);
        } else {
          lcd.print(var);
        }
        tone(tonepin, 2400, 30);
        break;
      }
    }
    if (i == 7) delay(300);
  }
}


void setNewPass() {

  while (1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcdprint(enternewpass);
    setPass();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcdprint(retype);

    setCode();

    if (comparePassword()) {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcdprint(passsaved);
      delay(2000);
      break;
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcdprint(passnotmatch);
      if (soundEnable) tone(tonepin, errorTone, 200);
      delay(2000);
    }
  }
}
//Whait until a button is pressed is is a number return the number 'char' if not return x
char getNumber() {

  var = keypad.waitForKey();

  switch (var) {
    case 'a':
      return 'x';
      break;
    case 'b':
      return 'x';
      break;

    case 'c':
      return 'x';
      break;
    case 'd':
      return 'x';
      break;
    case '*':
      return 'x';
      break;
    case '#':
      return 'x';
      break;
    default:
      return var;
      break;
  }
}
