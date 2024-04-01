#include <SFML/Graphics.hpp>
#include <cmath>
#include "imgui.h"
#include "imgui-SFML.h"
#include <vector>


using namespace sf;

int main()
{


    const int windowWidth = 800;
    const int windowHeight = 600;
    bool simulation = false;
    bool graph;
    RenderWindow window(VideoMode(windowWidth, windowHeight), "Mathematical Pendulum", Style::None);
   
    window.setFramerateLimit(75);

    ImGui::SFML::Init(window);

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("SegoeUI.ttf", 16, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
    ImGui::SFML::UpdateFontTexture();

    const float g = 9.81;      // ускорение свободного падения
    float l = 200;             // длина маятника
    float damping = 0.02;      // коэффициент затухания
    float angle = 180;         // начальный угол отклонения
    float angularVelocity = 0; // начальная скорость

    CircleShape pendulum(20);
    pendulum.setOrigin(pendulum.getRadius(), 0);
    pendulum.setPosition(windowWidth / 2, windowHeight / 2 + l);
                                                                          //создания маятника и нити
    VertexArray line(Lines, 2);
    line[0].position = Vector2f(windowWidth / 2, windowHeight / 5);

    Clock deltaClock, clock;

    std::vector<float> angleValues; // Вектор для хранения значений угла

    Sprite tripod;
    Texture tripodtexture;

    tripodtexture.loadFromFile("tripod.png");
    tripod.setTexture(tripodtexture);
    tripod.setPosition(190, -30);

    while (window.isOpen())
    {
        float deltaTime = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        deltaTime /= 40000;
        Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == Event::Closed)
                window.close();

        }

        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::SetNextWindowSize(ImVec2(350, 170));
        ImGui::Begin(u8"Настройки", nullptr, ImGuiWindowFlags_NoResize);

        if (ImGui::SliderFloat(u8"Угол отклонения", &angle, 0, 180))
        {
            angularVelocity = 0;
        }

        ImGui::SliderFloat(u8"Сила затухания", &damping, 0, 0.02);

        ImGui::SliderFloat(u8"Длина нити", &l, 50, 400);

        ImGui::Checkbox(u8"Показывать график", &graph);

        if (ImGui::Button(u8"Старт"))
        {
            simulation = true;
        }

        ImGui::SameLine();

        if (ImGui::Button(u8"Стоп"))
        {
            simulation = false;
        }
        ImGui::SameLine();
        if (simulation)
        {
            float angularAcceleration = -g / l * sin(angle * 3.14 / 180) - damping * angularVelocity;    //физ модель маятника
            angularVelocity += angularAcceleration * deltaTime;
            angle += angularVelocity * deltaTime * 180 / 3.14;
            angleValues.push_back(angle);
        }
        if (ImGui::Button(u8"Сброс"))
        {
            simulation = false;
            angle = 180;
            angularVelocity = 0;
            angleValues.clear();
        }
        ImGui::SameLine();
        if (ImGui::Button(u8"Выход"))
            window.close();
        ImGui::SameLine();
        

        if (graph)
        {
            // Рисуем график зависимости угла от времени
            ImGui::SetNextWindowSize(ImVec2(600, 250));
            ImGui::Begin(u8"График", nullptr, ImGuiWindowFlags_NoResize);
            ImGui::PlotLines(u8"Зависимость угла отклонения от времени\n(Наведите курсор для отображения данных)", angleValues.data(), angleValues.size(), 0, nullptr, -180, 180, ImVec2(300, 200), sizeof(float));


            ImGui::End();
        }

        ImGui::End();

        if (angle < -180)
            angle++;
        if (angle > 180)
            angle--;



        float x = l * sin(angle * 3.14 / 180);
        float y = l * cos(angle * 3.14 / 180);
        line[1].position = Vector2f(windowWidth / 2 + x, windowHeight / 5 + y);
        line[1].color = Color::White;

        pendulum.setPosition(line[1].position.x, line[1].position.y - 20);

        window.clear(Color(32, 46, 64));
        window.draw(tripod);
        window.draw(pendulum);
        window.draw(line);

       

        ImGui::SFML::Render(window);
        
        window.display();

    }

    ImGui::SFML::Shutdown();

    return 0;
}
