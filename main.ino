#define WIFI_SSID "" // Название WiFi или точки доступа
#define WIFI_PASS "" // Пароль от WiFi или точки доступа
#define BOT_TOKEN "" // Токен бота
#define ADMIN_ID "" // ID администратора


#include <FastBot.h>
#include <string>
FastBot bot(BOT_TOKEN);
String ban_list[50]; // Список забаненых
int len_of_ban_list = 0; // Количество забаненых пользователей

void setup() {
  connectWiFi();
  bot.attach(newMsg);
}

// Функция для блокирования пользователя
void user_ban(String user_ID) {
  ban_list[len_of_ban_list] = user_ID;
  len_of_ban_list++;
  bot.sendMessage("Пользователь "+user_ID+" заблокирован.", ADMIN_ID); 
}

// Функция для разблокирования пользователя
void user_unban(String user_ID) {
  for (int i=0; i < 50; i++) {
    if (ban_list[i] == user_ID) {                        
      ban_list[i] = "0";
      len_of_ban_list--;
    }
  }
  bot.sendMessage("Пользователь "+user_ID+" разблокирован.", ADMIN_ID);
}

// Действия над сообщениями пользователя
void newMsg(FB_msg& msg) {
  int flag = 0;
  
  for (int i=0; i<50; i++) {
    if (msg.userID == ban_list[i]) {
      flag = 1;
      break;
    }
  }

  // Начало работы с ботом
  if (msg.text == "/start") {
    bot.sendMessage("Привет! Если у тебя есть какая-либо идея, то напиши мне!", msg.chatID); 
  }
  // Если сообщение от администратора
  else if (msg.userID == ADMIN_ID) {

    // Если мы зажали сообщение с ID пользователя и выбрали пункт "ответить"
    if (!msg.replyText.isEmpty()) {

      // Блокирование пользователя с помощью команды /ban
      if (msg.text == "/ban") {
        user_ban(msg.replyText);
      }
      // Разблокирование пользователя с помощью команды /unban
      else if (msg.text == "/unban") { 
        user_unban(msg.replyText);
      }
      // Если администратор не использует никакие команды, то его сообщение пересылается пользователю
      else {
          bot.sendMessage("Ответ от администратора: "+msg.text, msg.replyText);
          bot.sendMessage("Ответ отправлен.", msg.userID);
      }
    }
    // Если мы не зажали сообщение с ID поьзователя и не выбрали пункт "ответить", то наше сообщение пересылается нам же. Обычно использую для проверки, жив ли бот
    else {
      bot.sendMessage(msg.userID, ADMIN_ID); 
      bot.sendMessage(msg.text, ADMIN_ID);
    }
  } 
  // Если пользователь заблокирован, то программа уведомляет его об этом и не пересылает сообщение администратору
  else if (flag == 1) {
    bot.sendMessage("Прошу прощения, но вы заблокированы.", msg.chatID); 
  }
  // Если пользователь не заблокирован, то программа пересылает сообщение администратору и уведомляет пользователя об этом
  else if (flag == 0){
    bot.replyMessage("Спасибо за сообщение! Я уже передал его члену ученического совета!", msg.messageID, msg.chatID); 
    bot.sendMessage(msg.userID, ADMIN_ID); 
    bot.sendMessage(msg.text, ADMIN_ID); 
  }
}
// Цикл, обновляющий информацию о сообщениях в чате
void loop() {
  bot.tick();
}
// Подключение к сети
void connectWiFi() {
  delay(2000);
  Serial.begin(9600);
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
}