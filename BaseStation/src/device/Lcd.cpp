#include "device/Lcd.h"

using namespace lcd;

Lcd::Lcd()
	: tft(Lcd::tft_cs, Lcd::tft_dc, Lcd::tft_rst)
	, current_printable_elements(0)
	, next_element_to_print(0)
	, text_size(1)
	, text_color(ST7735_WHITE)
{
	this->tft.initR(INITR_BLACKTAB);
	this->PrintHeader();
	this->tft.setTextWrap(true);

	this->captured_time = millis();
}

uint16_t Lcd::SetCursorToCenter(const char* str, int16_t y)
{
	int16_t x1, y1;
	uint16_t w, h;
	this->tft.getTextBounds(str, 0, y, &x1, &y1, &w, &h);
	this->tft.setCursor((tft.width() / 2) - (w / 2), y1);

	return h;
}

void Lcd::EnablePrinting()
{
	this->is_printing_disabled = false;
	this->text_size = 1;
	this->text_color = ST7735_WHITE;
}

void Lcd::PrintHeader()
{
	this->tft.fillScreen(ST7735_BLACK);
	this->tft.drawFastHLine(0, 44, tft.width(), ST7735_BLUE);
	this->tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
	this->tft.setTextSize(1);
	this->SetCursorToCenter("STACJA BAZOWA", 22);
	this->tft.print("STACJA BAZOWA");
}

void Lcd::Print(const char* str)
{
	if(this->is_printing_disabled)
		return;

	this->PrintHeader();
	this->tft.setTextSize(this->text_size);
	this->tft.setTextColor(this->text_color);
	this->SetCursorToCenter(str, 77);
	this->tft.print(str);
}

void Lcd::Println(size_t count, const char* str, ...)
{
	if(this->is_printing_disabled)
		return;

	va_list vl;
	const char* _str = str;
	va_start(vl, str);

	this->PrintHeader();
	this->tft.setTextSize(this->text_size);
	this->tft.setTextColor(this->text_color);
	int16_t start_y = 77;

	for(size_t i{}; i < count; i++)
	{
		start_y += this->SetCursorToCenter(_str, start_y) + 3;
		this->tft.print(_str);
		_str = va_arg(vl, const char*);
	}

	va_end(vl);
}

void Lcd::PrintError(definitions::ERROR_TYPE error)
{
	char tmp_buff[10];
	sprintf(tmp_buff, "0x%02X", error);
	this->tft.setTextColor(ST7735_RED, ST7735_BLACK);
	this->text_size = 2;
	this->text_color = ST7735_RED;
	this->Println(2, "ERROR", tmp_buff);

	this->is_printing_disabled = true;
}

void Lcd::AddPrintableElement(PrintableElement element)
{
	if(this->current_printable_elements < definitions::max_report_stations)
	{
		this->printableElementsList[this->current_printable_elements++] = element;
	}
}

void Lcd::UpdatePrintableElement(uint8_t uid, String name, double temp, double humidity)
{
	for(size_t i{}; i < this->current_printable_elements; i++)
	{
		if(this->printableElementsList[i].uid == uid)
		{
			this->printableElementsList[i].name = name;
			this->printableElementsList[i].temperature = temp;
			this->printableElementsList[i].humidity = humidity;
		}
	}
}

void Lcd::Service()
{
	if(millis() - this->captured_time >= static_cast<unsigned long>(1000 * 5))
	{
		if(this->current_printable_elements > 0)
		{
			char nameBuff[definitions::max_report_station_name_length];
			this->printableElementsList[this->next_element_to_print].name.toCharArray(
				nameBuff, sizeof(nameBuff));
			char temperatureBuff[10];
			sprintf(temperatureBuff,
					"%.2f C",
					this->printableElementsList[this->next_element_to_print].temperature);
			char humidityBuff[10];
			sprintf(humidityBuff,
					"%.2f %%",
					this->printableElementsList[this->next_element_to_print].humidity);

			this->Println(3, nameBuff, temperatureBuff, humidityBuff);
			this->next_element_to_print =
				(this->next_element_to_print + 1) % this->current_printable_elements;
			Serial.println(this->current_printable_elements);
			Serial.println(this->next_element_to_print);
		}
		else
		{
			this->Print("Brak danych.");
		}

		this->captured_time = millis();
	}
}