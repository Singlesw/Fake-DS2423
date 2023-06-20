# Fake-DS2423



    [Руководство пользователя](http://honey.com.ua/counter/manual.htm)
    [Обзорная статья](https://habr.com/ru/post/568314/)
    [Библиотека для Arduino с примерами скетчей](https://github.com/honechko/DS2423/)
    [Статья о подключении и использовании 1-wire](https://habr.com/ru/post/529720/)
    [Магазин](https://arduino.ua/prod4180-schetchik-impylsov-s-interfeisom-1-wire-sdelano-v-ykraine)



# 1-wire pulse counter

![1-wire pulse counter](https://github.com/honechko/DS2423/raw/main/counter.jpg)

This device is based on ATtiny13A with closed-source firmware with author's
features. It is not DS2423 or its replacement, but it is developed to be
possible to use it with a lot of software written for DS2423
(such as [OWFS](https://github.com/owfs/owfs) or Linux kernel modules).

## Main usage

* electricity, water, heat, gas accounting

## Key features

* direct connection of analog sensors (no need in comparators)
* integrated battery backup
* low power consumption even with connected sensors
* internal pull-ups with pulse mode operation
* sensors' power supply with pulse mode operation
* adjustable parameters

## Example of web-interface

![Web-interface](https://github.com/honechko/DS2423/raw/main/Docs/setup.png)
