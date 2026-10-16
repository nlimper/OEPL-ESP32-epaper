#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>

#include "commstructs.h"
#include "main.h"
#include "udp.h"
#include "EPDcommon.h"

//
// *** select your display type in main.h !!
//

#ifdef EPD_401_ACEP
#include "EPD_4in01f.h"
#endif
#ifdef EPD_730_SPECTRA
#include "EPD_7in3e.h"
#endif
#ifdef EPD_266_G
#include "EPD_2in66g.h"
#endif

#define WAKEUP_REASON_FIRSTBOOT 0xFC

const char *ssid = "****";
const char *password = "****";

void advertiseTagTask(void *parameter) {
	sendAvail(WAKEUP_REASON_FIRSTBOOT);
	while (true) {
		vTaskDelay(60000 / portTICK_PERIOD_MS);
		sendAvail(0);
		Serial.println("checkin");
	}
}

void drawImage(uint8_t *buffer, uint8_t dataType) {
	Serial.println("init datatype " + String(dataType));

#ifdef EPD_401_ACEP
	EPD_4IN01F_Init();
	Serial.println("display image");
	EPD_4IN01F_Display(buffer);
	Serial.println("sleep");
	EPD_4IN01F_Sleep();
#endif

#ifdef EPD_730_SPECTRA
	EPD_7IN3E_Init();
	Serial.println("display image");
	EPD_7IN3E_Display(buffer);
	Serial.println("sleep");
	EPD_7IN3E_Sleep();
#endif

#ifdef EPD_266_G
	// two bit planes from the AP; a black/white image comes as a single plane
	EPD_2IN66G_Init();
	Serial.println("display image");
	const uint8_t *plane1 = buffer + EPD_2IN66G_WIDTH * EPD_2IN66G_HEIGHT / 8;
	EPD_2IN66G_DisplayPlanes(buffer, dataType == 0x21 ? plane1 : NULL);
	Serial.println("sleep");
	EPD_2IN66G_Sleep();
#endif

}

void setup() {
	Serial.begin(115200);
	Serial.println("ACEP/Spectra demo");

	Serial.printf("Heap size: %u\n", ESP.getHeapSize());
	Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
	Serial.printf("Largest free block: %u\n", ESP.getMaxAllocHeap());

	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.println("wifi connected");

	Module_Init();

	init_udp();
	xTaskCreate(advertiseTagTask, "Tag alive", 6000, NULL, 2, NULL);
}

void loop() {
	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("WiFi disconnected. Attempting to reconnect...");

		WiFi.disconnect();
		WiFi.reconnect();

		int attempts = 0;
		while (WiFi.status() != WL_CONNECTED && attempts < 10) {
			Serial.print(".");
			delay(500);
			attempts++;
		}

		if (WiFi.status() == WL_CONNECTED) {
			Serial.println("\nReconnected to WiFi");
			init_udp();
		} else {
			Serial.println("\nFailed to reconnect");
			delay(5000);
		}
	}
	vTaskDelay(10000 / portTICK_PERIOD_MS);
}
