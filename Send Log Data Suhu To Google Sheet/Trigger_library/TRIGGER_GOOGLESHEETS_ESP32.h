#ifndef __TRIGGER_GOOGLESHEETS_H__
#define __TRIGGER_GOOGLESHEETS_H__

#include <stdarg.h>
#include <string.h>
#include <HTTPClient.h>


void sheets_initialization();
void Send_Data();
void Google_Sheets_Init(char array_2d[10][20], String sheets_gas_id, int param_size);

String url;

char column_name[ ][20]={"temperature","humidity"};;

double random_values[100];

void float_to_string();
const char* host = "https://script.google.com";
const int httpsPort = 443;

char Sensor_Values[10][10];
// const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";
const char * root_ca=\
"-----BEGIN CERTIFICATE-----\n" \
"MIIDujCCAqKgAwIBAgILBAAAAAABD4Ym5g0wDQYJKoZIhvcNAQEFBQAwTDEgMB4G\n" \
"A1UECxMXR2xvYmFsU2lnbiBSb290IENBIC0gUjIxEzARBgNVBAoTCkdsb2JhbFNp\n" \
"Z24xEzARBgNVBAMTCkdsb2JhbFNpZ24wHhcNMDYxMjE1MDgwMDAwWhcNMjExMjE1\n" \
"MDgwMDAwWjBMMSAwHgYDVQQLExdHbG9iYWxTaWduIFJvb3QgQ0EgLSBSMjETMBEG\n" \
"A1UEChMKR2xvYmFsU2lnbjETMBEGA1UEAxMKR2xvYmFsU2lnbjCCASIwDQYJKoZI\n" \
"hvcNAQEBBQADggEPADCCAQoCggEBAKbPJA6+Lm8omUVCxKs+IVSbC9N/hHD6ErPL\n" \
"v4dfxn+G07IwXNb9rfF73OX4YJYJkhD10FPe+3t+c4isUoh7SqbKSaZeqKeMWhG8\n" \
"eoLrvozps6yWJQeXSpkqBy+0Hne/ig+1AnwblrjFuTosvNYSuetZfeLQBoZfXklq\n" \
"tTleiDTsvHgMCJiEbKjNS7SgfQx5TfC4LcshytVsW33hoCmEofnTlEnLJGKRILzd\n" \
"C9XZzPnqJworc5HGnRusyMvo4KD0L5CLTfuwNhv2GXqF4G3yYROIXJ/gkwpRl4pa\n" \
"zq+r1feqCapgvdzZX99yqWATXgAByUr6P6TqBwMhAo6CygPCm48CAwEAAaOBnDCB\n" \
"mTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUm+IH\n" \
"V2ccHsBqBt5ZtJot39wZhi4wNgYDVR0fBC8wLTAroCmgJ4YlaHR0cDovL2NybC5n\n" \
"bG9iYWxzaWduLm5ldC9yb290LXIyLmNybDAfBgNVHSMEGDAWgBSb4gdXZxwewGoG\n" \
"3lm0mi3f3BmGLjANBgkqhkiG9w0BAQUFAAOCAQEAmYFThxxol4aR7OBKuEQLq4Gs\n" \
"J0/WwbgcQ3izDJr86iw8bmEbTUsp9Z8FHSbBuOmDAGJFtqkIk7mpM0sYmsL4h4hO\n" \
"291xNBrBVNpGP+DTKqttVCL1OmLNIG+6KYnX3ZHu01yiPqFbQfXf5WRDLenVOavS\n" \
"ot+3i9DAgBkcRcAtjOj4LaR0VknFBbVPFd5uRHg5h6h+u/N5GJG79G+dwfCMNYxd\n" \
"AfvDbbnvRG15RjF+Cv6pgsH/76tuIMRQyV+dTZsXjAzlAcmgQWpzU/qlULRuJQ/7\n" \
"TBj0/VLZjmmx6BEP3ojY+x1J96relc8geMJgEtslQIxq/H5COEBkEveegeGTLg==\n" \
"-----END CERTIFICATE-----\n";

String GAS_ID;
int Count;
String sheetName;

void Google_Sheets_Init(char test[ ][20], String sheets_gas_id, int param_size)
{
	GAS_ID = sheets_gas_id;
	Count = param_size;
	
	for (int i = 0; i < Count; i++)
    {
	 for (int j = 0; j < 20; j++)
     {
		column_name[i][j] = test[i][j];
     }
    }
	
	#ifdef Debug_Serial_Mon
	   for(int i=0;i<Count;i++)
	   {
			Serial.print("column_name= ");
			Serial.println(column_name[i]);
	   }
   #endif
	
}

void Data_to_Sheets(int num,...)
{
	va_list lst;
	va_start(lst,num);
	
	for(int i=0;i<num;i++)
	{
		random_values[i]= va_arg(lst,double);
	}
	va_end(lst);	
	
	float_to_string();
	Send_Data();
}

void float_to_string()
{
  for(int j=0;j<Count;j++)
  {
    sprintf(Sensor_Values[j],"%.02f",random_values[j]);
	#ifdef Debug_Serial_Mon
		Serial.print("Sensor Values : ");
		Serial.println(Sensor_Values[j]);
	#endif
  }
}
void Send_Data()
{
	HTTPClient http;

	String url = "https://script.google.com/macros/s/" + GAS_ID + "/exec?";
	int i=0;
	while(i!=Count)
	{
		if(i==0)
		{
		  url = url+column_name[i]+"="+Sensor_Values[i];
		  i++;
		}
		if(i==Count)
		  break;
		url = url+"&"+column_name[i]+"="+Sensor_Values[i];
		i++;    
	}
  
	Serial.print("requesting URL: ");
	Serial.println(url);

	http.begin(url, root_ca); //Specify the URL and certificate
    int httpCode = http.GET();  
    http.end();
}

#endif
