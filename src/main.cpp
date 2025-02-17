// Copyright 2025 Pavlo Shutz

#include <memory>
#include <utility>

#include <SFML/Graphics.hpp>

#include "../include/Random.hpp"

class Card : public sf::Drawable {
 public:
	 enum class Suit {
		Diamond,
		Heart,
		Club,
		Spade
	 };

	 Card() = default;

	 Card(sf::IntRect&& deckRect, std::shared_ptr<sf::Texture> deckTexture)
		 : m_deckTexture(deckTexture)
		 , m_deckRect(deckRect)
		 , m_pos(m_deckRect.position.x / 2, m_deckRect.position.y / 2) {
		 m_suit = static_cast<Suit>(m_deckRect.position.y / m_deckRect.size.y);
		 m_rank = static_cast<int>(m_deckRect.position.x / m_deckRect.size.x) + 1;
	 }

	 ~Card() {
		 m_deckTexture.reset();
	 }

 private:
	 void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		 sf::Sprite card(*m_deckTexture.lock());
		 card.setTextureRect(m_deckRect);
		 card.setScale({ 0.5f, 0.5f });
		 card.setPosition(m_pos);

		 target.draw(card, states);
	 }

	 std::weak_ptr<sf::Texture> m_deckTexture;
	 sf::IntRect m_deckRect{};
	 sf::Vector2f m_pos{};
	 Suit m_suit{ Suit::Diamond };
	 int m_rank{ 1 };
};

class StockPile {
 public:
	 explicit StockPile(const std::filesystem::path& filename)
		 : m_deckTexture{ std::make_shared<sf::Texture>(filename) } {
		 m_deckTexture.get()->setSmooth(true);
		 fillDeck();
	 }

	 void shuffle() {
		 for (int i{ 0 }; i < 1000; ++i) {
			 int pos1 = Random::get<int>(0, m_deck.size() - 1);
			 int pos2 = Random::get<int>(0, m_deck.size() - 1);
			 std::swap(m_deck[pos1], m_deck[pos2]);
		 }
	 }

	 void displayCards(sf::RenderWindow& window) const {
		 for (const auto& card : m_deck) {
			 window.draw(*card);
		 }
	 }

	 void showCard(sf::RenderWindow& window) const {
		 window.draw(*m_deck[13]);
	 }

 private:
	 void fillDeck() {
		 int height = m_deckTexture.get()->getSize().y / 4;
		 int width = m_deckTexture.get()->getSize().x / 13;

		 for (int j{ 0 }; j < 4; ++j) {
			 for (int i{ 0 }; i < 13; ++i) {
				 m_deck[j * 13 + i] = std::move(std::make_unique<Card>(
					 sf::IntRect({ width * i, height * j }, { width, height }),
					 m_deckTexture));
			 }
		 }
	 }

 private:
	 std::shared_ptr<sf::Texture> m_deckTexture;
	 std::array<std::unique_ptr<Card>, 52> m_deck;
};

class Player {
 public:
	 Player() = default;
	 
	 void addCard(std::unique_ptr<Card> card) {
		 m_cards.push_back(std::move(card));
	 }

	 void displayCards(sf::RenderWindow& window) const {
		 for (const auto& card : m_cards) {
			 window.draw(*card);
		 }
	 }

 private:
	std::vector<std::unique_ptr<Card>> m_cards;
};

class Heap {

};

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

	StockPile stockPile("Card Asset/Standard 52 Cards/solitaire/all_cards.png");
	stockPile.shuffle();

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		backgroundTexture.clear(sf::Color::Black);

		backgroundTexture.draw(canvas);
		backgroundTexture.draw(table);

		backgroundTexture.display();

		window.clear();

		sf::Sprite background(backgroundTexture.getTexture());
		window.draw(background);
		stockPile.showCard(window);

		window.display();
	}
}
