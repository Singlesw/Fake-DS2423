# Fake-DS2423 Web Config Arduino Lan

Данный скетч предназначен для эмуляции owserver на arduino для работы данных датчиков и их настройки. Arduino в консоли сообщит о выданом ip адресе. В [Web-интерфейс](http://honey.com.ua/counter/setup.htm) вводи этот адрес и работаем :). Данное ПО выведет список всех OneWire устройств.

## Подключение Arduino к ENC28j60:

    1. ENC28j60(VCC) - Arduino(3V3)
    2. ENC28j60(GND) - Arduino(GND)
    3. ENC28j60(CS)  - Arduino(D10)
    4. ENC28j60(SI)  - Arduino(D11)
    6. ENC28j60(SO)  - Arduino(D12)
    6. ENC28j60(SCK) - Arduino(D13)

## Подключение к датчику OneWire:

![1-wire pulse counter](https://github.com/Singlesw/Fake-DS2423/blob/main/img/large_AOC741-8.jpg)

## Links

* [Web-интерфейс](http://honey.com.ua/counter/setup.htm)
* [Руководство пользователя](http://honey.com.ua/counter/manual.htm)
* [Обзорная статья](https://habr.com/ru/post/568314/)
* [Библиотека для Arduino с примерами скетчей](https://github.com/honechko/DS2423/)
* [Статья о подключении и использовании 1-wire](https://habr.com/ru/post/529720/)
* [Магазин](https://arduino.ua/prod4180-schetchik-impylsov-s-interfeisom-1-wire-sdelano-v-ykraine)

## Железо

![1-wire pulse counter](https://github.com/honechko/DS2423/raw/main/counter.jpg)

## Example of web-interface

![Web-interface](https://github.com/honechko/DS2423/raw/main/Docs/setup.png)
