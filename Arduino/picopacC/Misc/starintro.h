#include <Adafruit_GFX.h>
#include "airstrike16pt7b.h"

class StarIntro {
	private:
    Adafruit_ST7789 tft;
	GFXcanvas16 canvas;
	int gameID = 1;
	public:
    StarIntro (Adafruit_ST7789 &tftP): tft(tftP), canvas(240, 280) {
	}
	
	uint16_t getStringWidth(String str) {
		int16_t x1, y1;
		uint16_t w, h;
		canvas.getTextBounds(str, 0, 0, &x1, &y1, &w, &h);
		return w;
	}
	int starIntro()
	{
		canvas.setFont(&airstrike16pt7b);
		const int numstars = 	500;
		unsigned long lastSelection = millis();
		float stars[numstars][4];
		
		int eescoreS = eepromReadInt(2);
		int eescoreT = eepromReadInt(0);
		int eescoreTM = eepromReadInt(4);
		
		for (int i = 0; i < numstars; i++)
		{
			stars[i][0] = 67;
			stars[i][1] = 120;
			float angle = (random(3600) / 1800.0) * PI;
			float speed = random(450) + 450;
			stars[i][2] = speed * sin(angle);
			stars[i][3] = speed * cos(angle);
		}
		
		while (digitalRead(KEY_CENTER) == HIGH)
		{
			canvas.fillScreen(ST77XX_BLACK);
			canvas.setCursor(120 - getStringWidth("PicoPac") / 2, 50);
			canvas.println("PicoPac");
			if (millis() % 4500 < 1500)
			{
				canvas.setCursor(120 - getStringWidth("PICTRIS") / 2, 130);
				canvas.println("PICTRIS");
				canvas.setCursor(120 - getStringWidth("HI-SCORE") / 2, 100);
				canvas.println("HI-SCORE");
				char cstr[16];
				itoa(eescoreT, cstr, 10);
				canvas.setCursor(120 - getStringWidth(cstr) / 2, 160);
				canvas.println(cstr);
			}
			else if (millis() % 4500 < 3000)
			{
				canvas.setCursor(120 - getStringWidth("SCLANGE") / 2, 130);
				canvas.println("SCLANGE");
				canvas.setCursor(120 - getStringWidth("HI-SCORE") / 2, 100);
			    canvas.println("HI-SCORE");
				char cstr[16];
				itoa(eescoreS, cstr, 10);
				canvas.setCursor(120 - getStringWidth(cstr) / 2, 160);
				canvas.println(cstr);
			}
			else //if (millis() % 400 < 4500)
			{
				canvas.setCursor(120 - getStringWidth("TABMAN") / 2, 130);
				canvas.println("TABMAN");
				canvas.setCursor(120 - getStringWidth("HI-SCORE") / 2, 100);
			    canvas.println("HI-SCORE");
				char cstr[16];
				itoa(eescoreTM, cstr, 10);
				canvas.setCursor(120 - getStringWidth(cstr) / 2, 160);
				canvas.println(cstr);
			}
			
			if ( millis() % 600 < 150) {
				analogWrite(LEDR, 65);
				analogWrite(LEDY, 0);
				analogWrite(LEDG, 0);
				} else if (millis() % 600 < 300) {
				analogWrite(LEDR, 0);
				analogWrite(LEDY, 15);
				analogWrite(LEDG, 0);
				} else if (millis() % 600 < 450) {
				analogWrite(LEDR, 0);
				analogWrite(LEDY, 0);
				analogWrite(LEDG, 185);
				} else {
				analogWrite(LEDR, 0);
				analogWrite(LEDG, 30);
				analogWrite(LEDG, 0);
			}
			
			if (millis() % 500 < 250)
			{
				canvas.setCursor(120 - getStringWidth("Select") / 2, 216);
				canvas.println("Select");
				if (gameID == 1)
				{
					canvas.setCursor(120 - getStringWidth("<                    >") / 2, 246);
					canvas.println("<                    >");
					canvas.setCursor(120 - getStringWidth("PICTRIS") / 2, 246);
					canvas.println("PICTRIS");
				}
				else if (gameID == 2)
				{
					canvas.setCursor(120 - getStringWidth("<                    >") / 2, 246);
					canvas.println("<                    >");
					canvas.setCursor(120 - getStringWidth("SCLANGE") / 2, 246);
					canvas.println("SCLANGE");
				}
				else
				{
					canvas.setCursor(120 - getStringWidth("<                    >") / 2, 246);
					canvas.println("<                    >");
					canvas.setCursor(120 - getStringWidth("TABMAN") / 2, 246);
					canvas.println("TABMAN");
				}
				
			}    
			for (int i = 0; i < numstars; i++)
			{
				
				canvas.drawPixel(stars[i][1], stars[i][0],ST77XX_WHITE);
				stars[i][0] += stars[i][2] / 1000;
				stars[i][1] += stars[i][3] / 1000;
				
				stars[i][2] *= 1.1;
				stars[i][3] *= 1.1;
				
				if (stars[i][0] < 0 || stars[i][0] > 280 || stars[i][1] < 0 || stars[i][1] > 240)
				{
					stars[i][0] = 67;
					stars[i][1] = 120;
					float angle = (random(3600) / 1800.0) * PI;
					float speed = random(450) + 450;
					stars[i][2] = speed * sin(angle);
					stars[i][3] = speed * cos(angle);
				}
			}
			tft.drawRGBBitmap(0, 0, canvas.getBuffer(), canvas.width(), canvas.height());
			
			if (digitalRead(KEY_DOWN) == LOW && millis() > lastSelection + 200)
			{
				gameID = gameID - 1;
				if (gameID == 0)
				gameID = 3;//2
				lastSelection = millis();
				// break;
			}
			else if (digitalRead(KEY_UP) == LOW && millis() > lastSelection + 200)
			{
				gameID = gameID + 1;
				if (gameID == 4)//3
				gameID = 1;
				lastSelection = millis();
				// break;
			}
		}
		delay(10);
		analogWrite(LEDR, 0);
		analogWrite(LEDG, 0);
		analogWrite(LEDY, 0);
		while (digitalRead(KEY_CENTER) == LOW) {}
		delay(90);
		return gameID;
	}
	
	
};						