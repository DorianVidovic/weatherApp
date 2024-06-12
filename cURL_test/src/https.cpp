#include <cerrno>
#include <filesystem>

#include "https.h"
#include "curl/curl.h"



namespace api {

	CURL* curl = NULL;
	CURLcode result;

	Json::Reader reader;
	Json::Value parsedData;


	std::string baseURL = "https://api.weatherapi.com/v1/";
	std::string key = "key=911e86ff402045c3a2e211253242205"; //Add your key

	std::string urlCurrent = "current.json?";
	std::string urlForcast = "forecast.json?";
	std::string urlSearch = "search.json?";

	size_t WriteDataToString(void* ptr, size_t size, size_t nmemb, std::string* output) {
		size_t totalSize = size * nmemb;
		output->append(static_cast<char*>(ptr), totalSize);
		return totalSize;
	};

	size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
		size_t written = fwrite(ptr, size, nmemb, stream);
		return written;
	}

	api::weather weatherRequest(weatherType par_value, std::string par_place, bool par_aqi, int par_days, bool par_alerts) {

		std::string aqi = par_aqi == true ? "&aqi=yes" : "&aqi=no";
		std::string alerts = par_alerts == true ? "&alerts=yes" : "&alerts=no";

		std::string rawJson;
		api::weather weatherParsed{};

		curl = curl_easy_init();
		if (curl == NULL)
		{
			fprintf(stderr, "HTTP request fail\n");
		}


		if (par_value == weatherType::current){
			curl_easy_setopt(curl, CURLOPT_URL, baseURL.append(urlCurrent).append(key).append("&q=" + par_place).append(aqi).c_str());
		}

		if (par_value == weatherType::forecast){
			curl_easy_setopt(curl, CURLOPT_URL, baseURL.append(urlForcast).append(key).append("&q=" + par_place).append(aqi).append("&days=" + std::to_string(par_days)).append(alerts).c_str());
		}

		if (par_value == weatherType::search){
			curl_easy_setopt(curl, CURLOPT_URL, baseURL.append(urlSearch).append(key).append("&q=" + par_place).c_str());
		}

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteDataToString);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rawJson);

		result = curl_easy_perform(curl);
		if (result != CURLE_OK)
		{
			fprintf(stderr, "Error: %s\n", curl_easy_strerror(result));
		}
		curl_easy_cleanup(curl);


		//parsing data
		bool parsingSuccessful = reader.parse(rawJson, parsedData);
		if (!parsingSuccessful) {
			// handle error
		}

		weatherParsed.name = parsedData["location"]["name"].asString();

		weatherParsed.lastUpdateEpoch = parsedData["current"]["last_updated_epoch"].asInt64();
		weatherParsed.lastUpdate = parsedData["current"]["last_updated"].asString();
		weatherParsed.tempC = parsedData["current"]["temp_c"].asFloat();
		weatherParsed.isDay = parsedData["current"]["is_day"].asBool();

		weatherParsed.conditionText = parsedData["current"]["condition"]["text"].asString();
		weatherParsed.icon = parsedData["current"]["condition"]["icon"].asString();
		weatherParsed.code = parsedData["current"]["condition"]["code"].asInt();

		weatherParsed.windKph = parsedData["current"]["wind_kph"].asFloat();
		weatherParsed.windDir = parsedData["current"]["wind_dir"].asString();
		weatherParsed.pressureMb = parsedData["current"]["pressure_mb"].asFloat();
		weatherParsed.precipMm = parsedData["current"]["precip_mm"].asFloat();
		weatherParsed.humidity = parsedData["current"]["humidity"].asInt();
		weatherParsed.cloud = parsedData["current"]["cloud"].asInt();
		weatherParsed.feelsLikeC = parsedData["current"]["feelslike_c"].asFloat();
		weatherParsed.visKm = parsedData["current"]["vis_km"].asFloat();
		weatherParsed.uv = parsedData["current"]["uv"].asFloat();

		return weatherParsed;
	}

	void download_file(const char* url, std::string filePath, const char* outfilename)
	{
		CURL* curl;
		FILE* fp;
		errno_t err;
		CURLcode res;

		std::filesystem::path iconPath = filePath;
		const char* fullPath = filePath.append(outfilename).c_str();

		std::cout << "\n\n***url: " << url << std::endl;

		if (std::filesystem::exists(fullPath))
		{
			std::cout << "\n\n***file allready exists wont download\n\n\n";
			return;
		}

		std::cout << "\n\n***downloading... " << fullPath << "\n\n" << std::endl;

		curl = curl_easy_init();
		
		if (curl) {
			if (!std::filesystem::is_directory(iconPath))
			{
				std::cout << "***folder does not exists" << std::endl;
				std::filesystem::create_directories(iconPath);
				std::cout << "***folder created" << std::endl;
			}
			else
			{
				std::cout << "***folder exists" << std::endl;
			}

			if (err = fopen_s(&fp, fullPath, "wb") != 0)
			{
				char buffer[256];
				strerror_s(buffer, sizeof(buffer), err);
				std::cout << "\n*** didnt open/download file: " << fullPath << ", error: " << buffer << std::endl;
			}
			else
			{
				std::cout << "\n*** file opened: " << std::endl;

				curl_easy_setopt(curl, CURLOPT_URL, url);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
				res = curl_easy_perform(curl);
				if (res != CURLE_OK)
				{
					fprintf(stderr, "\n\n***curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
				}



				curl_easy_cleanup(curl);
				std::fclose(fp);
			}
		}
	}

	std::string get_filename(const std::string& url) {
		// Find the last slash in the URL
		size_t last_slash = url.find_last_of("/");

		// If a slash was found, return everything after it
		if (last_slash != std::string::npos) {
			return url.substr(last_slash + 1);
		}

		// If no slash was found, return the whole URL
		return url;
	}
	std::string fullPath(std::string s1, std::string s2)
	{
		std::string temp = s1 + s2;
		return temp;
	}
}