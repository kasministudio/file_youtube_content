#include "TRIGGER_WIFI.h"               
#include "TRIGGER_GOOGLESHEETS.h"      
#include "DHT.h"
#define DHTTYPE DHT11
#define dht_pin D3

DHT dht(dht_pin, DHTTYPE);

/**********Google Sheets Definations***********/
char column_name_in_sheets[ ][20] = {"value1","value2"};                        /*1. The Total no of column depends on how many value you have created in Script of Sheets;2. It has to be in order as per the rows decided in google sheets*/
String Sheets_GAS_ID = "AKfycbzIsoi7sdWnUkX73bXaEVbZDgBN4WyeOt0mCCPZEaibMgmmgzs";                                         /*This is the Sheets GAS ID, you need to look for your sheets id*/
int No_of_Parameters = 2;                                                                /*Here No_of_Parameters decides how many parameters you want to send it to Google Sheets at once, change it according to your needs*/
/*********************************************/

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  dht.begin();

  WIFI_Connect("SSID","Pass");                                                     /*Provide you Wi-Fi SSID and password to connect to Wi-Fi*/
  Google_Sheets_Init(column_name_in_sheets, Sheets_GAS_ID, No_of_Parameters );         /*Sets the column name for Google Sheets, the GAS ID, and the No of Parameter we want to send*/        
}

void loop() 
{
  float s = dht.readTemperature();
  float h = dht.readHumidity();                          

  Data_to_Sheets(No_of_Parameters,  s,  h);         /*1. This function accepts multiple float parameter, here No_of_Parameters decides how many parameters you want to send to Google Sheets; 2. The values sent should be in order as per the column in Google Sheets*/

  Serial.println();
  delay(10000);                                       /*10 Sec Delay, Here 10 second delay is just used so that we can see the data pushed to sheets one by one
                                                        There is nothing like a minimum delay between two data push*/
}
