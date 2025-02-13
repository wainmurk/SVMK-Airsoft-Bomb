/*
  ───────────────────────────────────────────────────────────────────────────────────────────────────
  ─██████████████────────██████──██████────────██████──────────██████────────██████──████████────────
  ─██░░░░░░░░░░██────────██░░██──██░░██────────██░░██████████████░░██────────██░░██──██░░░░██────────
  ─██░░██████████────────██░░██──██░░██────────██░░░░░░░░░░░░░░░░░░██────────██░░██──██░░████────────
  ─██░░██────────────────██░░██──██░░██────────██░░██████░░██████░░██────────██░░██──██░░██──────────
  ─██░░██████████────────██░░██──██░░██────────██░░██──██░░██──██░░██────────██░░██████░░██──────────
  ─██░░░░░░░░░░██────────██░░██──██░░██────────██░░██──██░░██──██░░██────────██░░░░░░░░░░██──────────
  ─██████████░░██────────██░░██──██░░██────────██░░██──██████──██░░██────────██░░██████░░██──────────
  ─────────██░░██────────██░░░░██░░░░██────────██░░██──────────██░░██────────██░░██──██░░██──────────
  ─██████████░░██─██████─████░░░░░░████─██████─██░░██──────────██░░██─██████─██░░██──██░░████─██████─
  ─██░░░░░░░░░░██─██░░██───████░░████───██░░██─██░░██──────────██░░██─██░░██─██░░██──██░░░░██─██░░██─
  ─██████████████─██████─────██████─────██████─██████──────────██████─██████─██████──████████─██████─
  ───────────────────────────────────────────────────────────────────────────────────────────────────

*/
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <GyverPower.h>
#include <microLED.h>
#include "EEPROM.h"


#define volt_min 3.0  //Минимальное напряжение на аккумуляторе
int real_5v = 5000;
#define STRIP_PIN 13  // пин ленты
#define NUMLEDS 9     // кол-во светодиодов
#define tonepin 3
#define mosfet 4
bool rtype = 1;  //(1 - обратный, 0 - прямой)





const uint8_t readytogame[] = { 0xe0, 0x6f, 0x20, 0xb4, 0x70, 0xb8, 0x20, 0xb4, 0x6f, 0xbf, 0x6f, 0xb3, 0xb8, 0xb9, 0x00 };                // До гри готовий
const uint8_t batterylow[] = { 0x48, 0xb8, 0xb7, 0xc4, 0xba, 0xb8, 0xb9, 0x20, 0xb7, 0x61, 0x70, 0xc7, 0xe3, 0x00 };                       //Низький заряд
const uint8_t poweroff[] = { 0x42, 0xb8, 0xbc, 0xba, 0xbd, 0x65, 0xbd, 0xbd, 0xc7, 0x00 };                                                 // Вимкнення
const uint8_t battery[] = { 0x48, 0x61, 0xbe, 0x70, 0x79, 0xb4, 0x61, 0x00 };                                                              //Напруга
const uint8_t bombdefused[] = { 0xa4, 0xbd, 0x65, 0xc1, 0xba, 0x6f, 0xe3, 0xb6, 0x65, 0xbd, 0x6f, 0x00 };                                  //Знешкоджено
const uint8_t gameover[] = { 0xa1, 0x70, 0x79, 0x20, 0xb7, 0x61, 0xb3, 0x65, 0x70, 0xc1, 0x65, 0xbd, 0x6f, 0x00 };                         //Гру завершено
const uint8_t rematch[] = { 0x50, 0x65, 0xb3, 0x61, 0xbd, 0xc1, 0x3f, 0x00 };                                                              //Реванш?
const uint8_t yesno[] = { 0x41, 0x3a, 0x20, 0x54, 0x61, 0xba, 0x20, 0x20, 0x20, 0x20, 0x42, 0x3a, 0x20, 0x48, 0x69, 0x00 };                //А: Так    В: Ні
const uint8_t bombexploded[] = { 0xa0, 0x6f, 0xbc, 0xb2, 0x61, 0x20, 0xb3, 0xb8, 0xb2, 0x79, 0x78, 0xbd, 0x79, 0xbb, 0x61, 0x00 };         //Бомба вибухнула
const uint8_t gametime[] = { 0xab, 0x61, 0x63, 0x79, 0x20, 0xb7, 0x61, 0xbb, 0xb8, 0xc1, 0xb8, 0xbb, 0x6f, 0x63, 0xc4, 0x00 };             //Часу залишилось
const uint8_t neutral[] = { 0x48, 0x65, 0xb9, 0xbf, 0x70, 0x61, 0xbb, 0xc4, 0xbd, 0x61, 0x20, 0xb7, 0x6f, 0xbd, 0x61, 0x00 };              //Нейтральна зона
const uint8_t neutralizing[] = { 0xa4, 0x61, 0xc0, 0xb8, 0x63, 0xbf, 0xba, 0x61, 0x2e, 0x2e, 0x2e, 0x00 };                                 //Зачистка...
const uint8_t capturing[] = { 0xa4, 0x61, 0x78, 0x6f, 0xbe, 0xbb, 0x65, 0xbd, 0xbd, 0xc7, 0x2e, 0x2e, 0x2e, 0x00 };                        //Захоплення...
const uint8_t team1wins[] = { 0xa8, 0x65, 0x70, 0x65, 0xbc, 0x6f, 0xb4, 0x61, 0x20, 0xb4, 0x70, 0x79, 0xbe, 0xb8, 0x20, 0x31, 0x00 };      //Перемога групи 1
const uint8_t team2wins[] = { 0xa8, 0x65, 0x70, 0x65, 0xbc, 0x6f, 0xb4, 0x61, 0x20, 0xb4, 0x70, 0x79, 0xbe, 0xb8, 0x20, 0x32, 0x00 };      //Перемога групи 2
const uint8_t red[] = { 0xab, 0x65, 0x70, 0xb3, 0x6f, 0xbd, 0xb8, 0xb9, 0x00 };                                                            //Червоний
const uint8_t yellow[] = { 0xa3, 0x6f, 0xb3, 0xbf, 0xb8, 0xb9, 0x00 };                                                                     //Жовтий
const uint8_t blue[] = { 0x43, 0xb8, 0xbd, 0x69, 0xb9, 0x00 };                                                                             //Синій
const uint8_t purple[] = { 0xaa, 0x69, 0x6f, 0xbb, 0x65, 0xbf, 0x6f, 0xb3, 0xb8, 0xb9, 0x00 };                                             //Фіолетовий
const uint8_t green[] = { 0xa4, 0x65, 0xbb, 0x65, 0xbd, 0xb8, 0xb9, 0x00 };                                                                //Зелений
const uint8_t white[] = { 0xa0, 0x69, 0xbb, 0xb8, 0xb9, 0x00 };                                                                            //Білий
const uint8_t seconds[] = { 0x63, 0x65, 0xba, 0x79, 0xbd, 0xe3, 0x00 };                                                                    //секунд
const uint8_t teamcolors[] = { 0x4b, 0x6f, 0xbb, 0xc4, 0x6f, 0x70, 0xb8, 0x20, 0xba, 0x6f, 0xbc, 0x61, 0xbd, 0xe3, 0x00 };                 //Кольори команд
const uint8_t teama[] = { 0x4b, 0x6f, 0xbc, 0x61, 0xbd, 0xe3, 0x61, 0x20, 0x41, 0x00 };                                                    //Команда А
const uint8_t teamb[] = { 0x4b, 0x6f, 0xbc, 0x61, 0xbd, 0xe3, 0x61, 0x20, 0xa0, 0x00 };                                                    //Команда Б
const uint8_t redwins[] = { 0xa8, 0x65, 0x70, 0x65, 0xbc, 0x6f, 0xb4, 0x61, 0x20, 0xab, 0x65, 0x70, 0xb3, 0x6f, 0xbd, 0x2e, 0x00 };        //Перемога Червон.
const uint8_t bluewins[] = { 0xa8, 0x65, 0x70, 0x65, 0xbc, 0x6f, 0xb4, 0x61, 0x20, 0x43, 0xb8, 0xbd, 0x69, 0x78, 0x00 };                   //Перемога Синіх
const uint8_t yellowwins[] = { 0xa8, 0x65, 0x70, 0x65, 0xbc, 0x6f, 0xb4, 0x61, 0x20, 0xa3, 0x6f, 0xb3, 0xbf, 0xb8, 0x78, 0x00 };           //Перемога Жовтих
const uint8_t purplewins[] = { 0xa8, 0x65, 0x70, 0x65, 0xbc, 0x6f, 0xb4, 0x61, 0x20, 0xaa, 0x69, 0x6f, 0xbb, 0x65, 0xbf, 0x2e, 0x00 };     //Перемога Фіолет.
const uint8_t greenwins[] = { 0xa8, 0x65, 0x70, 0x65, 0xbc, 0x6f, 0xb4, 0x61, 0x20, 0xa4, 0x65, 0xbb, 0x65, 0xbd, 0xb8, 0x78, 0x00 };      //Перемога Зелених
const uint8_t whitewins[] = { 0xa8, 0x65, 0x70, 0x65, 0xbc, 0x6f, 0xb4, 0x61, 0x20, 0xa0, 0x69, 0xbb, 0xb8, 0x78, 0x00 };                  //Перемога Білих
const uint8_t redtime[] = { 0xab, 0x61, 0x63, 0x20, 0xc0, 0x65, 0x70, 0xb3, 0x6f, 0xbd, 0xb8, 0x78, 0x00 };                                //Час червоних
const uint8_t bluetime[] = { 0xab, 0x61, 0x63, 0x20, 0x63, 0xb8, 0xbd, 0x69, 0x78, 0x00 };                                                 //Час синіх
const uint8_t yellowtime[] = { 0xab, 0x61, 0x63, 0x20, 0xb6, 0x6f, 0xb3, 0xbf, 0xb8, 0x78, 0x00 };                                         //Час жовтих
const uint8_t purpletime[] = { 0xab, 0x61, 0x63, 0x20, 0xe4, 0x69, 0x6f, 0xbb, 0x65, 0xbf, 0x6f, 0xb3, 0xb8, 0x78, 0x00 };                 //Час фіолетових
const uint8_t greentime[] = { 0xab, 0x61, 0x63, 0x20, 0xb7, 0x65, 0xbb, 0x65, 0xbd, 0xb8, 0x78, 0x00 };                                    //Час зелених
const uint8_t whitetime[] = { 0xab, 0x61, 0x63, 0x20, 0xb2, 0x69, 0xbb, 0xb8, 0x78, 0x00 };                                                //Час білих
const uint8_t redzone[] = { 0xab, 0x65, 0x70, 0xb3, 0x6f, 0xbd, 0x61, 0x20, 0xb7, 0x6f, 0xbd, 0x61, 0x00 };                                //Червона зона
const uint8_t bluezone[] = { 0x20, 0x20, 0x43, 0xb8, 0xbd, 0xc7, 0x20, 0xb7, 0x6f, 0xbd, 0x61, 0x00 };                                     //Синя зона
const uint8_t yellowzone[] = { 0x20, 0x20, 0xa3, 0x6f, 0xb3, 0xbf, 0x61, 0x20, 0xb7, 0x6f, 0xbd, 0x61, 0x00 };                             //Жовта зона
const uint8_t purplezone[] = { 0xaa, 0x69, 0x6f, 0xbb, 0x65, 0xbf, 0x6f, 0xb3, 0x61, 0x20, 0xb7, 0x6f, 0xbd, 0x61, 0x00 };                 //Фіолетова зона
const uint8_t greenzone[] = { 0x20, 0xa4, 0x65, 0xbb, 0x65, 0xbd, 0x61, 0x20, 0xb7, 0x6f, 0xbd, 0x61, 0x00 };                              //Зелена зона
const uint8_t whitezone[] = { 0x20, 0x20, 0xa0, 0x69, 0xbb, 0x61, 0x20, 0xb7, 0x6f, 0xbd, 0x61, 0x00 };                                    //Біла зона
const uint8_t time1[] = { 0xab, 0x61, 0x63, 0x20, 0xb4, 0x70, 0x79, 0xbe, 0xb8, 0x20, 0x31, 0x3a, 0x00 };                                  //Час групи 1
const uint8_t time2[] = { 0xab, 0x61, 0x63, 0x20, 0xb4, 0x70, 0x79, 0xbe, 0xb8, 0x20, 0x32, 0x3a, 0x00 };                                  //Час групи 2
const uint8_t playagain[] = { 0xa1, 0x70, 0x61, 0xbf, 0xb8, 0x20, 0xb7, 0xbd, 0x6f, 0xb3, 0x79, 0x3f, 0x00 };                              //Грати знову?
const uint8_t enternewpass[] = { 0x48, 0x6f, 0xb3, 0xb8, 0xb9, 0x20, 0xbe, 0x61, 0x70, 0x6f, 0xbb, 0xc4, 0x3a, 0x00 };                     //Новий пароль
const uint8_t retype[] = { 0x42, 0xb3, 0x65, 0xe3, 0x69, 0xbf, 0xc4, 0x20, 0xb7, 0xbd, 0x6f, 0xb3, 0x79, 0x3a, 0x00 };                     //Введіть знову
const uint8_t passsaved[] = { 0xa8, 0x61, 0x70, 0x6f, 0xbb, 0xc4, 0x20, 0xb7, 0xb2, 0x65, 0x70, 0x65, 0xb6, 0x65, 0xbd, 0x6f, 0x00 };      //Пароль збережено
const uint8_t passnotmatch[] = { 0xa8, 0x61, 0x70, 0x6f, 0xbb, 0xc4, 0x20, 0xbd, 0x65, 0x20, 0xb7, 0xb2, 0x69, 0xb4, 0x63, 0xc7, 0x00 };   //Пароль не збігся
const uint8_t textsearchdestroy[] = { 0xa8, 0x6f, 0xc1, 0x79, 0xba, 0x26, 0xa4, 0xbd, 0xb8, 0xe6, 0x65, 0xbd, 0xbd, 0xc7, 0x00 };          //Пошук знищення
const uint8_t textsabotage[] = { 0x43, 0x61, 0xb2, 0x6f, 0xbf, 0x61, 0xb6, 0x00 };                                                         //Саботаж
const uint8_t textdomination[] = { 0xa8, 0x61, 0xbd, 0x79, 0xb3, 0x61, 0xbd, 0xbd, 0xc7, 0x00 };                                           //Панування
const uint8_t settings[] = { 0x48, 0x61, 0xbb, 0x61, 0xc1, 0xbf, 0x79, 0xb3, 0x61, 0xbd, 0xbd, 0xc7, 0x00 };                               //Налаштування
const uint8_t teamcolor[] = { 0x4b, 0x6f, 0xbb, 0x69, 0x70, 0x20, 0xba, 0x6f, 0xbc, 0x61, 0xbd, 0xe3, 0xb8, 0x00 };                        //Колір команди
const uint8_t teamcolor1[] = { 0x4b, 0x6f, 0xbb, 0x69, 0x70, 0x20, 0xba, 0x6f, 0xbc, 0x61, 0xbd, 0xe3, 0xb8, 0x31, 0x00 };                 //Колір команди 1
const uint8_t teamcolor2[] = { 0x4b, 0x6f, 0xbb, 0x69, 0x70, 0x20, 0xba, 0x6f, 0xbc, 0x61, 0xbd, 0xe3, 0xb8, 0x32, 0x00 };                 //Колір команди 2
const uint8_t defaulttime[] = { 0xab, 0x61, 0x63, 0x20, 0xb7, 0x61, 0x20, 0xb7, 0x61, 0xbc, 0x6f, 0xb3, 0xc0, 0x2e, 0x00 };                //Час за замовч.
const uint8_t testbl[] = { 0x54, 0x65, 0x63, 0xbf, 0x20, 0xe3, 0x69, 0x6f, 0xe3, 0x69, 0xb3, 0x00 };                                       //Тест діодів
const uint8_t setbrightness[] = { 0xb1, 0x63, 0xba, 0x70, 0x61, 0xb3, 0x69, 0x63, 0xbf, 0xc4, 0x00 };                                      //Яскравість
const uint8_t testrelay[] = { 0xa8, 0x65, 0x70, 0x65, 0xb3, 0x69, 0x70, 0xba, 0x61, 0x20, 0x70, 0x65, 0xbb, 0x65, 0x00 };                  //Перевірка реле
const uint8_t setgametime[] = { 0x54, 0x70, 0xb8, 0xb3, 0x61, 0xbb, 0x69, 0x63, 0xbf, 0xc4, 0x20, 0xb4, 0x70, 0xb8, 0x00 };                //Тривалість гри
const uint8_t minutes[] = { 0x58, 0xb3, 0xb8, 0xbb, 0xb8, 0xbd, 0x00 };                                                                    //Хвилин
const uint8_t relayon[] = { 0x50, 0x65, 0xbb, 0x65, 0x20, 0x61, 0xba, 0xbf, 0xb8, 0xb3, 0x6f, 0xb3, 0x61, 0xbd, 0x6f, 0x00 };              //Реле активовано
const uint8_t relayoff[] = { 0x50, 0x65, 0xbb, 0x65, 0x20, 0xb3, 0xb8, 0xbc, 0xba, 0xbd, 0x79, 0xbf, 0x6f, 0x00 };                         //Реле вимкнуто
const uint8_t bombtime[] = { 0xab, 0x61, 0x63, 0x20, 0xe3, 0x6f, 0x20, 0xb3, 0xb8, 0xb2, 0x79, 0x78, 0x79, 0x00 };                         //Час до вибуху
const uint8_t armtime[] = { 0xab, 0x61, 0x63, 0x20, 0xbd, 0x61, 0x20, 0xe3, 0x69, 0xc6, 0x00 };                                            //Час на дію
const uint8_t actsound[] = { 0xa9, 0xb3, 0x69, 0xbc, 0xba, 0xbd, 0x79, 0xbf, 0xb8, 0x20, 0xb7, 0xb3, 0x79, 0xba, 0x3f, 0x00 };             //Увімкнути звук?
const uint8_t actrelay[] = { 0x41, 0xba, 0xbf, 0xb8, 0xb3, 0x79, 0xb3, 0x61, 0xbf, 0xb8, 0x20, 0x70, 0x65, 0xbb, 0x65, 0x3f, 0x00 };       //Активувати реле?
const uint8_t actcode[] = { 0xa9, 0xb3, 0x69, 0xbc, 0xba, 0xbd, 0x79, 0xbf, 0xb8, 0x20, 0xba, 0x6f, 0xe3, 0x3f, 0x00 };                    //Увімкнути код?
const uint8_t armingbomb[] = { 0x41, 0xba, 0xbf, 0xb8, 0xb3, 0x61, 0xe5, 0x69, 0xc7, 0x20, 0xb2, 0x6f, 0xbc, 0xb2, 0xb8, 0x00 };           //Активація бомби
const uint8_t enterpass[] = { 0x42, 0xb3, 0x65, 0xe3, 0x69, 0xbf, 0xc4, 0x20, 0xbe, 0x61, 0x70, 0x6f, 0xbb, 0xc4, 0x00 };                  //Введіть пароль
const uint8_t passerror[] = { 0xa8, 0x6f, 0xbc, 0xb8, 0xbb, 0xba, 0x61, 0x20, 0xbe, 0x61, 0x70, 0x6f, 0xbb, 0xc6, 0x00 };                  //Помилка паролю
const uint8_t bombactivated[] = { 0xa0, 0x6f, 0xbc, 0xb2, 0x79, 0x20, 0x61, 0xba, 0xbf, 0xb8, 0xb3, 0x6f, 0xb3, 0x61, 0xbd, 0x6f, 0x00 };  //Бомбу активовано
const uint8_t endtime[] = { 0xab, 0x61, 0x63, 0x20, 0xb3, 0xb8, 0xb9, 0xc1, 0x6f, 0xb3, 0x00 };                                            //Час вийшов
const uint8_t defusing[] = { 0xa4, 0xbd, 0x65, 0xc1, 0xba, 0x6f, 0xe3, 0xb6, 0x65, 0xbd, 0xbd, 0xc7, 0x00 };                               //Знешкодження
const uint8_t longdefusing[] = { 0xa4, 0xbd, 0x65, 0xc1, 0xba, 0x6f, 0xe3, 0xb6, 0x65, 0xbd, 0xbd, 0xc7, 0x2e, 0x2e, 0x2e, 0x00 };         //Знешкодження
const uint8_t readytobegin[] = { 0xa1, 0x6f, 0xbf, 0x6f, 0xb3, 0xb8, 0xb9, 0x20, 0xbe, 0x6f, 0xc0, 0xb8, 0xbd, 0x61, 0xbf, 0xb8, 0x00 };   //Готовий починати
const uint8_t pressbutton[] = { 0x48, 0x61, 0xbf, 0xb8, 0x63, 0xbd, 0x69, 0xbf, 0xc4, 0x20, 0xba, 0xbd, 0x6f, 0xbe, 0xba, 0x79, 0x00 };    //Натисніть кнопку
const uint8_t gamestarting[] = { 0xa8, 0x6f, 0xc0, 0x61, 0xbf, 0x6f, 0xba, 0x20, 0xb4, 0x70, 0xb8, 0x00 };                                 //Початок гри
const uint8_t in[] = { 0xc0, 0x65, 0x70, 0x65, 0xb7, 0x00 };                                                                               //через
const uint8_t expin[] = { 0x42, 0xb8, 0xb2, 0x79, 0x78, 0x20, 0xc0, 0x65, 0x70, 0x65, 0xb7, 0x00 };                                        //Вибух через
const uint8_t debug[] = { 0xe0, 0x69, 0x61, 0xb4, 0xbd, 0x6f, 0x63, 0xbf, 0xb8, 0xba, 0x61, 0x2e, 0xa1, 0x6f, 0x3f, 0x00 };                //Діагностика.Го?
const uint8_t setreal5vtext[] = { 0x4b, 0x6f, 0xbd, 0x63, 0xbf, 0x61, 0xbd, 0xbf, 0x61, 0x3A, 0x00 };                                      //Константа:
const uint8_t defenderwins[] = { 0xa8, 0x65, 0x70, 0x65, 0xbc, 0x69, 0xb4, 0x20, 0xa4, 0x61, 0x78, 0xb8, 0x63, 0xbf, 0x00 };               //Переміг Захист
const uint8_t dodefaults[] = { 0xa4, 0x61, 0x20, 0xb7, 0x61, 0xbc, 0x6f, 0xb3, 0xc0, 0x2e, 0x3f, 0x00 };                                   //За замовч.?
const uint8_t done[] = { 0xa1, 0x6f, 0xbf, 0x6f, 0xb3, 0x6f, 0x00 };                                                                       //Готово
const uint8_t pressa[] = { 0xa4, 0x61, 0xbf, 0xb8, 0x63, 0xbd, 0x69, 0xbf, 0xc4, 0x20, 0x41, 0x00 };                                       //Затисніть А
const uint8_t toleave[] = { 0xe6, 0x6f, 0xb2, 0x20, 0xb3, 0xb8, 0xb9, 0xbf, 0xb8, 0x00 };                                                  //щоб вийти




const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'a' },
  { '4', '5', '6', 'b' },
  { '7', '8', '9', 'c' },
  { '*', '0', '#', 'd' }
};

byte rowPins[ROWS] = { 9, 10, 11, 12 };
byte colPins[COLS] = { 5, 6, 7, 8 };

LiquidCrystal_I2C lcd(0x27, 16, 2);

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


char enteredText[8];
char password[8];
int key = -1;
char lastKey;
char var;
boolean passwordEnable = false;
char BT_UP = 'c';
char BT_DOWN = 'd';
char BT_SEL = 'a';
char BT_CANCEL = 'b';

microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB> strip;

boolean mosfetEnable = true;
//IS VERY IMPORTANT THAT YOU TEST THIS TIME. BY DEFAULT IS IN 1 SEC. THAT IS NOT TOO MUCH. SO TEST IT!
const int MOSFET_TIME = 3000;

//TIME INTS
int GAMEHOURS = 0;
int GAMEMINUTES = 45;
int BOMBMINUTES = 4;
int ACTIVATESECONDS = 5;
int bright = 100;
boolean endGame = false;
bool selteam;
bool is_voltage_been = 0;
boolean sdStatus = false;  //search and destroy game enable used in config
boolean saStatus = false;  //same but SAbotaghe
boolean doStatus = false;  //for DEmolition
boolean start = true;
boolean defuseando;
boolean cancelando;
// SOUND TONES
boolean soundEnable = true;
int tonoPitido = 3000;
int tonoAlarma1 = 700;
int tonoAlarma2 = 2600;
int tonoActivada = 1330;
int errorTone = 100;
byte colorofteama;
byte colorofteamb;

unsigned long iTime;
unsigned long timeCalcVar;
unsigned long redTime;
unsigned long greenTime;
unsigned long iZoneTime;
byte team = 0;
float input_voltage = 3.0;
void setup() {
  power.setSleepMode(POWERDOWN_SLEEP);
  delay(1);
  EEPROM.get(10, bright);
  EEPROM.get(15, GAMEHOURS);
  EEPROM.get(20, GAMEMINUTES);
  EEPROM.get(25, colorofteama);
  EEPROM.get(30, colorofteamb);
  real_5v = readIntFromEEPROM(40);


  Wire.setClock(400000L);
  strip.setBrightness(map(bright, 0, 100, 0, 255));
  strip.clear();
  strip.show();  // вывод изменений на ленту
  lcd.init();    //  Инициируем работу с LCD дисплеем
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("S.V.M.K. AIRSOFT"));
  lcd.setCursor(1, 1);
  lcdprint(readytogame);


  keypad.setHoldTime(50);
  keypad.setDebounceTime(50);
  keypad.addEventListener(keypadEvent);
  delay(1500);
  lcd.clear();

  pinMode(mosfet, OUTPUT);
  digitalWrite(mosfet, rtype);
  //
  byte bar1[8] = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
  };
  byte bar2[8] = {
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
  };
  byte bar3[8] = {
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
  };
  byte bar4[8] = {
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
  };
  byte bar5[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte up[8] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B11111,
    B00000,
    B00000,
  };

  byte down[8] = {
    B00000,
    B00000,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000,
  };
  lcd.createChar(0, bar1);
  lcd.createChar(1, bar2);
  lcd.createChar(2, bar3);
  lcd.createChar(3, bar4);
  lcd.createChar(4, bar5);
  lcd.createChar(5, up);
  lcd.createChar(6, down);
}

void loop() {
  if (keypad.getKey() == '0') {
    debugtest();
  } else {
    menuPrincipal();
  }
}


void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:
      switch (key) {
      }
      break;
    case RELEASED:
      switch (key) {
        case 'd':
          defuseando = false;
          break;
        case 'c':
          cancelando = false;
          break;
      }
      break;
    case HOLD:
      switch (key) {
        case 'd':
          defuseando = true;
          break;
        case 'c':
          cancelando = true;
          break;
      }
      break;
  }
}


void disarmedSplash() {
  endGame = false;
  if (sdStatus || saStatus) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcdprint(bombdefused);
    lcd.setCursor(2, 1);
    lcdprint(gameover);

    delay(100);
    tone(tonepin, 2500, 100);
    delay(100);
    tone(tonepin, 3500, 100);


    delay(5000);
    strip.clear();
    strip.show();
  }
  //end code
  lcd.clear();
  lcd.setCursor(4, 0);
  lcdprint(rematch);
  lcd.setCursor(0, 1);
  lcdprint(yesno);

  while (1) {
    var = keypad.waitForKey();
    if (var == BT_SEL) {
      strip.clear();
      strip.show();  // вывод изменений на ленту
      tone(tonepin, 2400, 30);
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
      tone(tonepin, 2400, 30);
      menuPrincipal();
      strip.clear();
      strip.show();  // вывод изменений на ленту
      break;
    }
  }
}

void explodeSplash() {
  strip.fill(mRed);
  strip.show();  // вывод изменений на ленту
  cls();
  delay(100);
  endGame = false;
  lcd.setCursor(1, 0);
  lcdprint(bombexploded);
  lcd.setCursor(1, 1);
  lcdprint(gameover);
  for (int i = 200; i > 0; i--)  //
  {
    tone(tonepin, i);
    delay(20);
  }
  noTone(tonepin);
  if (mosfetEnable) {
    activateMosfet();
  }
  delay(500);
  cls();

  //end code
  lcd.setCursor(4, 0);
  lcdprint(rematch);
  lcd.setCursor(0, 1);
  lcdprint(yesno);
  strip.clear();
  strip.show();  // вывод изменений на ленту
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
