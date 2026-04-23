#include <SFML/Graphics.hpp>

int main() {
    // 800x600 boyutunda bir pencere oluşturuyoruz
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Top Testi");
    window.setFramerateLimit(60); // Oyunun hızını sabitle (60 FPS)

    // Topu oluşturuyoruz (Yarıçapı 20 birim olan bir daire)
    sf::CircleShape ball(20.f);
    ball.setFillColor(sf::Color::Cyan); // Topun rengini Turkuaz yap
    ball.setPosition(400.f, 300.f);     // Topun başlangıç konumu (Ekranın ortası)

    // Topun hareket hızı (X ve Y eksenlerinde saniyede kaç piksel gideceği)
    sf::Vector2f ballVelocity(4.f, 4.f);

    // Ana Oyun Döngüsü
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Pencere kapatılırsa döngüden çık
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- GÜNCELLEME (MANTIK) BÖLÜMÜ ---
        
        // Topu mevcut hızıyla hareket ettir
        ball.move(ballVelocity);

        // Pencerenin sol veya sağ kenarına çarparsa X yönünü tersine çevir
        if (ball.getPosition().x < 0 || ball.getPosition().x + ball.getRadius() * 2 > 800) {
            ballVelocity.x = -ballVelocity.x;
        }

        // Pencerenin üst veya alt kenarına çarparsa Y yönünü tersine çevir
        if (ball.getPosition().y < 0 || ball.getPosition().y + ball.getRadius() * 2 > 600) {
            ballVelocity.y = -ballVelocity.y;
        }

        // --- ÇİZİM BÖLÜMÜ ---
        
        window.clear(sf::Color::Black); // Önce ekranı siyah ile temizle
        window.draw(ball);              // Topu çiz
        window.display();               // Çizilenleri ekrana yansıt
    }

    return 0;
}