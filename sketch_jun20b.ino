#include <EtherCard.h>
#include <OneWire.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x70,0x69,0x69,0x28,0x30,0x31 };

byte Ethernet::buffer[1000];
BufferFiller bfill;

OneWire ow(6);

void setup() {
  Serial.begin(115200);

  Serial.println("\n[Start Network]\n");

  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println( "Failed to access Ethernet controller");
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");

  ether.printIp("  IP:  ", ether.myip);
}

void printhex(byte data[], int length)
{
  for (int i =0; i < length;i++)
  {
    if (data[i] < 0x10) Serial.print("0");
    Serial.print(data[i], HEX);
  }
}

void loop() {
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  if (pos)  // check if valid tcp data is received
  {
    char *data = (char *) Ethernet::buffer + pos;

    char* start = strstr(data, "GET ");               // Находим начало и конец запроса
    char* end   = strstr(data, " HTTP/1.1");

    if (start == NULL || end == NULL)                 // Выходим если не найдены начало либо конец запроса
    {
      bfill = ether.tcpOffset();
      goto end_http;
    }

    size_t urlSize = end - start - 4;             // Вычисляем размер запроса

    char url[urlSize + 1];                    // Создаем новый буфер для запроса

    strncpy(url, start + 4, urlSize);         // Копируем запрос в новый буфер
    url[urlSize] = '\0';

    Serial.println(url);
    Serial.println(urlSize);

    bfill = ether.tcpOffset();

    if (urlSize == 15 && strstr(url, "/json/uncached/"))      //Send List of Devices on 1Wire
    {
      uint8_t address[8];
      uint8_t count = 0;

	    bfill.emit_p(PSTR(
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Pragma: no-cache\r\n"
        "\r\n"
        "{"
	    ));

      while (ow.search(address))
        if (ow.crc8(address, 7) == address[7])  // CHECK CRC
        {
          count++;
          if (count > 1) bfill.emit_p(PSTR(", "));
          bfill.emit_p(PSTR("\""));
          for (uint8_t i = 0; i < 8; i++)
          {
            bfill.emit_p(PSTR("$H"), address[i]);
            if (i == 0) bfill.emit_p(PSTR("."));
          }
        bfill.emit_p(PSTR("\": \"ok\""));
        }
      bfill.emit_p(PSTR("}"));

      goto end_http;
    }
 
    if ((urlSize == 45 || urlSize == 117) && strstr(url, "/json/uncached/"))
    {
      uint8_t address[8];                                     // Адрес устройства из URL
      uint8_t off = 15;
      char hexPairs[3] = {0,0,0};
      for (int i = 0; i < 8; i++) 
      {
        hexPairs[0] = url[i*2+off];
        hexPairs[1] = url[i*2+off+1];
        address[i] = strtoul(hexPairs, NULL, 16);
        if (i==0) off = 16;
      }

      hexPairs[0] = '0';                                      // Номер страницы чтения
      hexPairs[1] = url[44];
      int pageRNumber = strtoul(hexPairs, NULL, 16);
      uint16_t readAddr = pageRNumber * 32;

      uint8_t data_w[14];

      if (urlSize == 117)
      {
        off = 53;                                             // Данные для записи из URL
        for (int i = 0; i < 14; i++) 
        {
          hexPairs[0] = url[i*2+off];
          hexPairs[1] = url[i*2+off+1];
          data_w[i] = strtoul(hexPairs, NULL, 16);
        }

        hexPairs[0] = '0';                                     // Номер страницы записи
        hexPairs[1] = url[51];
        int pageWNumber = strtoul(hexPairs, NULL, 16);
        uint16_t writeAddr = pageWNumber * 32;

        ow.reset();
        ow.select(address);
        ow.write(0x0f); // WRITE SCRATCHPAD
        ow.write(writeAddr & 0xff);
        ow.write(writeAddr >> 8);
        ow.write_bytes(data_w, 14);
        ow.reset();
        ow.select(address);
        ow.write(0x5a); // COPY SCRATCHPAD
        ow.write(writeAddr & 0xff);
        ow.write(writeAddr >> 8);
        ow.write((writeAddr & 0x1f) + (14 - 1));

      }

	    bfill.emit_p(PSTR(
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Pragma: no-cache\r\n"
        "\r\n"
        "{\"0\":\""
	    ));

      uint8_t data1[32];

      ow.reset();
      ow.select(address);
      ow.write(0xA5); // READ MEM COUNTER
      ow.write(readAddr & 0xff); // low addr
      ow.write(readAddr >> 8); // hih addr
      ow.read_bytes(data1,32);

      //printhex(data1, 32);

      for (uint8_t i = 0; i < 32; i++)
        bfill.emit_p(PSTR("$H"), data1[i]);
      bfill.emit_p(PSTR("\"}"));

      goto end_http;
    }


    end_http:
    ether.httpServerReply(bfill.position());
  }

}
