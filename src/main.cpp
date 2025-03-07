// Copyright 2025 Pavlo Shutz

#include <SFML/Graphics.hpp>

#include "../include/Random.hpp"
#include "../include/Card.hpp"
#include "../include/StockPile.hpp"
#include "../include/Player.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode({ 934u, 523u }), "Crazy 8");
	window.setIcon(sf::Image("Card Asset/Standard 52 Cards/rect_cards/individual/spade/8spade.png"));

	sf::RenderTexture backgroundTexture(window.getSize());
	sf::Texture canvasTexture("Card Asset/Backgrounds/background_1.png");
	sf::Texture tableTexture("Card Asset/Tables/table_blue.png");
	tableTexture.setSmooth(true);

	sf::Sprite canvas(canvasTexture);
	sf::Sprite table(tableTexture);
	table.setPosition({ 0, static_cast<float>(window.getSize().y - table.getTextureRect().size.y) / 2.f });

	backgroundTexture.clear(sf::Color::Black);

	backgroundTexture.draw(canvas);
	backgroundTexture.draw(table);

	backgroundTexture.display();
	sf::Sprite background(backgroundTexture.getTexture());

	StockPile stockPile("Card Asset/Standard 52 Cards/solitaire/all_cards.png");
	stockPile.shuffle();

	Player player;

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
					player.placeCardsInStockPile(stockPile);
				}
			}
		}

		window.clear();

		window.draw(background);
		stockPile.displayCards(window);

		window.display();
	}
}
