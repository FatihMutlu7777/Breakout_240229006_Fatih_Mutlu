#include <SFML/Graphics.hpp>
#include <cmath> // abs fonksiyonu için gerekli

int main() {
    // 1. Pencere Oluşturma
    sf::RenderWindow window(sf::VideoMode(800, 600), "Breakout - Carpisma Kontrolü");
    window.setFramerateLimit(60); // Oyunun hızını sabitle

    // --- TOP (BALL) ---
    sf::CircleShape ball(10.f);
    ball.setFillColor(sf::Color::Cyan);
    ball.setOrigin(10.f, 10.f); // Merkezi orta nokta yap
    ball.setPosition(400.f, 300.f);
    sf::Vector2f ballVelocity(5.f, 5.f); // Topun hızı

    // --- DIKDORTGEN CUBUK (PADDLE) ---
    sf::RectangleShape paddle(sf::Vector2f(120.f, 15.f)); // İnce bir çubuk
    paddle.setFillColor(sf::Color::White);
    paddle.setOrigin(60.f, 7.5f); // Merkezi orta nokta yap
    paddle.setPosition(400.f, 570.f); // En altın biraz üstü
    float paddleSpeed = 10.f;

    // --- ANA DÖNGÜ ---
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- GÜNCELLEME (MANTIK) ---

        // 1. Çubuk Kontrolleri
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && paddle.getPosition().x > 60.f) {
            paddle.move(-paddleSpeed, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && paddle.getPosition().x < 740.f) {
            paddle.move(paddleSpeed, 0.f);
        }

        // 2. Topun Hareketi
        ball.move(ballVelocity);

        // 3. Topun Ekran Kenarlarından Sekmesi
        if (ball.getPosition().x - 10.f < 0 || ball.getPosition().x + 10.f > 800) {
            ballVelocity.x = -ballVelocity.x;
        }
        if (ball.getPosition().y - 10.f < 0) {
            ballVelocity.y = -ballVelocity.y;
        }

        // 4. KRITIK ADIM: Topun Çubuğa Çarpıp Yukarı Sekmesi
        // getGlobalBounds nesnelerin kapladığı alanı kontrol eder
        if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
            // Topun Y hızını yukarı yöne (negatif) çeviriyoruz
            ballVelocity.y = -std::abs(ballVelocity.y);
            
            // Topun çubuğun içinde takılı kalmaması için konumunu biraz yukarı itiyoruz
            ball.setPosition(ball.getPosition().x, paddle.getPosition().y - 18.f);
        }

        // --- ÇİZİM ---
        window.clear(sf::Color::Black);
        
        window.draw(ball);
        window.draw(paddle);
        
        window.display();
    }

    return 0;
}