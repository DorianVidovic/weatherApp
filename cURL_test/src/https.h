#pragma once
#include <iostream>
#include <string>
#include <fstream>


#include "json/json.h"

namespace api {

	enum class weatherType {
		current,
		forecast,
		search
	};

	struct weather
	{
		std::string name;

		uint64_t lastUpdateEpoch;
		std::string lastUpdate;
		float tempC;
		bool isDay;

		std::string conditionText;
		std::string icon;
		int code;

		float windKph;
		std::string windDir;
		float pressureMb;
		float precipMm;
		int humidity;
		int cloud;
		float feelsLikeC;
		float visKm;
		float uv;


	};

	size_t WriteDataToString(void* ptr, size_t size, size_t nmemb, std::string* output);
	size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream);

	api::weather weatherRequest(weatherType par_value, std::string par_place, bool par_aqi = false, int par_days = 1, bool par_alerts = false);
	void download_file(const char* url,std::string filePath ,const char* outfilename);
	

	std::string get_filename(const std::string& url);
	
	std::string fullPath(std::string s1, std::string s2);
}