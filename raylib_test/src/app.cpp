#include <iostream>
#include <string>
#include <chrono>

#include "gui.h"
#include "https.h"

#ifndef _DEBUG
    int WinMain() {
#else
    int main(){
#endif // !_Debug
    
    // Getting the weather data from weatherapi.com
    api::weather result = api::weatherRequest(api::weatherType::current, "Mostar");

    Color niceBlue = { 64, 137, 190, 255 };  //rgba(64, 137, 190, 1)
    Color coolBlue = { 113, 234, 250, 255 }; //rgba(113, 234, 250, 1)



    std::string imgPathDay   = "/Weather/res/img/day/";
    std::string imgPathNight = "/Weather/res/img/night/";
    std::string fontPath = "/Weather/res/font/";
    std::string fontUrl = "https://raw.githubusercontent.com/DorianVidovic/fonts/main/Inter-Bold.ttf";

    std::string iconUrl = result.isDay == true ? "https://cdn.weatherapi.com/weather/128x128/day/" : "https://cdn.weatherapi.com/weather/128x128/night/";
    
    //Getting the icon
    Image icon;
    std::string iconName = api::get_filename(result.icon);

    if (result.isDay)
    {
        api::download_file(api::fullPath(iconUrl,iconName).c_str(), imgPathDay, iconName.c_str());
        icon = LoadImage(api::fullPath(imgPathDay,iconName).c_str() );
    }
    else
    {
        api::download_file(api::fullPath(iconUrl, iconName).c_str(), imgPathNight, iconName.c_str());
        icon = LoadImage(api::fullPath(imgPathNight, iconName).c_str());
    }
   
    std::cout << "icon Name: " << iconName << ", full path: " << api::fullPath(imgPathNight, iconName) << std::endl;


    //Initializing the app
    gui app(1200, 700, true, "Weather", icon);


    //Loading fonts
    api::download_file(fontUrl.c_str(), fontPath, "Inter.ttf");
    app.fonts.push_back(LoadFontEx("/Weather/res/font/Inter.ttf", 128, 0, 250));
    app.fonts.push_back((GetFontDefault()));

    int textStartX = (app.windowWidth / 2)/2;
    int textStartY = (app.windowHeight / 2)/1.8;



    //Adding ui elements
    app.createElement(GenImageGradientLinear(app.windowWidth, app.windowHeight, 0, niceBlue, coolBlue), {1,1});

    app.createElement(result.name,0, 90, { (float)textStartX,50.f });
    app.createElement(result.conditionText, 0, 25, { (float)textStartX+5,120.f });
    app.createElement(std::to_string((int)result.tempC) + "°", 0, 90, {(float)textStartX + 400,50.f});
    app.createElement("Powerd by: weatherApi\n\nMade by: Dorian Vidović", 0, 20, app.windowWidth-245, app.windowHeight-50);
 
    //app.createElement(LoadImage(api::fullPath(imgPath,iconName).c_str()), {0.4, 0.4}, {(float)textStartX + 50 , (float)textStartY});
    app.createElement(icon, { 0.4, 0.4 }, { (float)textStartX + 50 , (float)textStartY });





    //App loop

    auto next_update = std::chrono::steady_clock::now();

    while (!app.windowExit())
    {
        auto now = std::chrono::steady_clock::now();
        if (now >= next_update) {
            
            result = api::weatherRequest(api::weatherType::current, "Mostar");
            next_update = now + std::chrono::minutes(5);
        }

        app.update();
        app.draw();
    }
     
	return 0;
}