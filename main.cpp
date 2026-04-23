#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

struct Brick {
    sf::RectangleShape shape;
    int maxHealth;
    int currentHealth;
    bool destroyed = false;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Breakout-Dinamik Fizik ve Centikler");
    window.setFramerateLimit(60);

    // --- NESNE TANIMLARI ---
    sf::CircleShape ball(10.f);
    ball.setFillColor(sf::Color::Cyan);
    ball.setOrigin(10.f, 10.f);
    ball.setPosition(400.f, 400.f);
    sf::Vector2f ballVelocity(0.f, -5.f); // Başta dik yukarı gitsin

    sf::RectangleShape paddle(sf::Vector2f(120.f, 15.f));
    paddle.setFillColor(sf::Color::White);
    paddle.setOrigin(60.f, 7.5f);
    paddle.setPosition(400.f, 570.f);

    // --- TUĞLALARI DİZME ---
    std::vector<Brick> bricks;
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 3; ++j) {
            Brick b;
            b.shape.setSize(sf::Vector2f(70.f, 25.f));
            b.shape.setPosition(i * 80.f + 5.f, j * 35.f + 50.f);
            b.maxHealth = 3 - j; // En üst 3, orta 2, alt 1 can
            b.currentHealth = b.maxHealth;
            b.shape.setFillColor(j == 0 ? sf::Color(100, 100, 100) : (j == 1 ? sf::Color(150, 150, 150) : sf::Color::White));
            bricks.push_back(b);
        }
    }

    // --- ÇENTİK ÇİZME FONKSİYONU ---
    auto drawCentik = [&](sf::RenderWindow& win, const Brick& b) {
        int vurusSayisi = b.maxHealth - b.currentHealth;
        if (vurusSayisi >= 1) { // Birinci Çentik
            sf::Vertex line1[] = {
                sf::Vertex(sf::Vector2f(b.shape.getPosition().x + 10, b.shape.getPosition().y), sf::Color::Black),
                sf::Vertex(sf::Vector2f(b.shape.getPosition().x + 20, b.shape.getPosition().y + 25), sf::Color::Black)
            };
            win.draw(line1, 2, sf::Lines);
        }
        if (vurusSayisi >= 2) { // İkinci Çentik
            sf::Vertex line2[] = {
                sf::Vertex(sf::Vector2f(b.shape.getPosition().x + 50, b.shape.getPosition().y), sf::Color::Black),
                sf::Vertex(sf::Vector2f(b.shape.getPosition().x + 40, b.shape.getPosition().y + 25), sf::Color::Black)
            };
            win.draw(line2, 2, sf::Lines);
        }
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // --- GÜNCELLEME ---
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && paddle.getPosition().x > 60.f) paddle.move(-10.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && paddle.getPosition().x < 740.f) paddle.move(10.f, 0.f);

        ball.move(ballVelocity);

        // Kenar Sekmeleri
        if (ball.getPosition().x - 10.f < 0 || ball.getPosition().x + 10.f > 800) ballVelocity.x = -ballVelocity.x;
        if (ball.getPosition().y - 10.f < 0) ballVelocity.y = -ballVelocity.y;

        // --- DİNAMİK ÇUBUK SEKMESİ (Açı Ayarı) ---
        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            // Çubuğun merkezine göre çarpma noktasını bul (-60 ile +60 arası)
            float hitLocation = ball.getPosition().x - paddle.getPosition().x;
            
            // Çarpma noktasına göre X hızını ayarla (Sağa çarparsa pozitif, sola çarparsa negatif X)
            ballVelocity.x = hitLocation / 10.f; 
            ballVelocity.y = -std::abs(ballVelocity.y); // Her zaman yukarı sek
            ball.setPosition(ball.getPosition().x, paddle.getPosition().y - 18.f);
        }

        // --- TUĞLA ÇARPIŞMA VE ÇENTİK ---
        for (auto& brick : bricks) {
            if (!brick.destroyed && ball.getGlobalBounds().intersects(brick.shape.getGlobalBounds())) {
                brick.currentHealth--;
                ballVelocity.y = -ballVelocity.y;
                if (brick.currentHealth <= 0) brick.destroyed = true;
                break;
            }
        }

        // --- ÇİZİM ---
        window.clear(sf::Color::Black);
        window.draw(paddle);
        window.draw(ball);
        for (const auto& b : bricks) {
            if (!b.destroyed) {
                window.draw(b.shape);
                drawCentik(window, b); // Çentikleri tuğlanın üstüne çiz
            }
        }
        window.display();
    }
    return 0;
}