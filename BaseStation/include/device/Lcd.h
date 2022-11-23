#pragma once

#include "definitions.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Arduino.h>
#include <cstdarg>

namespace lcd
{
class Lcd
{
public:
	struct PrintableElement
	{
		uint8_t uid;
		String name;
		double temperature;
		double humidity;

		PrintableElement(uint8_t uid, String name, double temperature, double humidity)
			: uid(uid)
			, name(name)
			, temperature(temperature)
			, humidity(humidity)
		{ }

		PrintableElement()
			: PrintableElement(0xFF, "", 0.0, 0.0)
		{ }
	};

private:
	static constexpr uint8_t tft_rst = 4;
	static constexpr uint8_t tft_cs = 5;
	static constexpr uint8_t tft_dc = 15;
	static constexpr uint8_t tft_mosi = 23;
	static constexpr uint8_t tft_sclk = 18;

	Adafruit_ST7735 tft;
	PrintableElement printableElementsList[definitions::max_report_stations];

	uint8_t current_printable_elements;
	uint8_t next_element_to_print;
	bool is_printing_disabled = false;
	uint8_t text_size;
	uint16_t text_color;
	unsigned long captured_time;

	void PrintHeader();
	uint16_t SetCursorToCenter(const char* str, int16_t y);

public:
	Lcd();
	void Print(const char* str);
	void Println(size_t count, const char* str, ...);
	void PrintError(definitions::ERROR_TYPE error);
	void AddPrintableElement(PrintableElement element);
	void UpdatePrintableElement(uint8_t uid, String name, double temp, double humidity);
	void Service();
	void EnablePrinting();
};
} // namespace lcd