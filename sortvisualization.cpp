#include <SFML/Graphics.hpp>
#include "imgui-SFML.h"
#include "imgui.h"
#include <iostream>
#include <cstdlib> // For std::rand()
#include <ctime>   // For std::time()
#include <thread>
#include <vector>
#include <algorithm>

struct Bar
{
    int height;
    sf::Color color;

    bool operator<(const Bar &other) const
    {
        return height < other.height;
    }
};

void setBars(std::vector<Bar> &barArray);
void displayBars(const std::vector<Bar> &barArray, sf::RenderWindow &window);
sf::Color getRandomColor();

int main()
{
    const static int SCREEN_WIDTH = 1920;
    const static int SCREEN_HEIGHT = 1080;
    const static char *WINDOW_TITLE = "Image Processor";
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), WINDOW_TITLE);
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    int slider = 25;
    std::vector<Bar> barArray(slider); // Initial array setup
    setBars(barArray);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::Begin(WINDOW_TITLE);

        if (ImGui::SliderInt("Array Size", &slider, 1, 500, "%d"))
        {
            barArray.resize(slider); // Resize the array when slider changes
            setBars(barArray); // Recalculate the bar heights
        }

        if (ImGui::Button("Run Sort"))
        {
            std::sort(barArray.begin(), barArray.end());
        }

        ImGui::End();

        window.clear();
        displayBars(barArray, window);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}

void displayBars(const std::vector<Bar> &barArray, sf::RenderWindow &window)
{
    int startX = 0;
    int barWidth =window.getSize().x / barArray.size();
    int startY = 60;
    for (const auto &bar : barArray)
    {
        sf::RectangleShape rect(sf::Vector2f(barWidth, bar.height));
        rect.setFillColor(bar.color);
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(1);
        rect.setPosition(startX, startY);
        window.draw(rect);
        startX += barWidth+1;
    }
}

sf::Color getRandomColor()
{
    uint8_t r = std::rand() % 255;
    uint8_t g = std::rand() % 255;
    uint8_t b = std::rand() % 255;
    return sf::Color(r, g, b, 255);
}

void setBars(std::vector<Bar> &barArray)
{
    std::srand(std::time(nullptr)); // Seed the random number generator
    for (auto &bar : barArray)
    {
        bar.height = std::rand() % 500; // Generate numbers between 100 and 199
        //bar.color = getRandomColor();
        bar.color = sf::Color::White;
    }
}
