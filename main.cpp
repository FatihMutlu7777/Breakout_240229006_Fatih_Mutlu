#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Breakout-Tuglalar");
    window.setFramerateLimit(60);

    // --- TOP VE CUBUK TANIMLARI ---
    sf::CircleShape ball(10.f);
    ball.setFillColor(sf::Color::Cyan);
    ball.setOrigin(10.f, 10.f);
    ball.setPosition(400.f, 300.f);
    sf::Vector2f ballVelocity(5.f, -5.f);

    sf::RectangleShape paddle(sf::Vector2f(120.f, 15.f));
    paddle.setFillColor(sf::Color::White);
    paddle.setOrigin(60.f, 7.5f);
    paddle.setPosition(400.f, 570.f);

    // --- TUĞLA (BRICK) YAPISI ---
    struct Brick {
        sf::RectangleShape shape;
        bool destroyed = false;
    };

    std::vector<Brick> bricks;
    int columns = 10; // Yan yana 10 tuğla
    int rows = 4;    // Üst üste 4 sıra
    float brickWidth = 70.f;
    float brickHeight = 20.f;
    float spacing = 10.f; // Tuğlalar arası boşluk

    // Tuğlaları oluşturup diziyoruz
    for (int i = 0; i < columns; ++i) {
        for (int j = 0; j < rows; ++j) {
            Brick b;
            b.shape.setSize(sf::Vector2f(brickWidth, brickHeight));
            b.shape.setFillColor(sf::Color::Yellow);
            // Tuğlaları ekranın ortasına hizalayarak diziyoruz
            b.shape.setPosition(i * (brickWidth + spacing) + 35.f, j * (brickHeight + spacing) + 50.f);
            bricks.push_back(b);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // --- GÜNCELLEME ---
        // Çubuk Kontrolü
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && paddle.getPosition().x > 60.f) paddle.move(-10.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && paddle.getPosition().x < 740.f) paddle.move(10.f, 0.f);

        ball.move(ballVelocity);

        // Ekran ve Çubuk Çarpışmaları
        if (ball.getPosition().x - 10.f < 0 || ball.getPosition().x + 10.f > 800) ballVelocity.x = -ballVelocity.x;
        if (ball.getPosition().y - 10.f < 0) ballVelocity.y = -ballVelocity.y;
        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            ballVelocity.y = -std::abs(ballVelocity.y);
        }

        // --- TUĞLA ÇARPIŞMA KONTROLÜ ---
        for (auto& brick : bricks) {
            if (!brick.destroyed && ball.getGlobalBounds().intersects(brick.shape.getGlobalBounds())) {
                brick.destroyed = true; // Tuğlayı kırılmış olarak işaretle
                ballVelocity.y = -ballVelocity.y; // Topu geri sektir
                break; // Bir karede sadece bir tuğla kırılsın
            }
        }

        // --- ÇİZİM ---
        window.clear(sf::Color::Black);
        window.draw(paddle);
        window.draw(ball);

        // Sadece kırılmamış tuğlaları çiz
        for (const auto& brick : bricks) {
            if (!brick.destroyed) {
                window.draw(brick.shape);
            }
        }

        window.display();
    }
    return 0;
}