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

	 Card()
		: m_card(m_cardBack) {

	 }

	 Card(sf::IntRect&& deckRect, std::shared_ptr<sf::Texture> deckTexture, bool isVisible)
		 : m_deckTexture(deckTexture)
		 , m_deckRect(deckRect)
		 , m_pos({500, 250})
		 , m_card(m_cardBack)
		 , m_isVisible(isVisible) {
		 m_suit = static_cast<Suit>(m_deckRect.position.y / m_deckRect.size.y);
		 m_rank = static_cast<int>(m_deckRect.position.x / m_deckRect.size.x) + 1;

		 if (m_isVisible) {
			 m_card.setTexture(*m_deckTexture.lock());
			 m_card.setTextureRect(m_deckRect);
			 m_card.setScale({ 0.5f, 0.5f });
			 m_card.setPosition(m_pos);
		 }
	 }

	 ~Card() {
		 m_deckTexture.reset();
	 }

 private:
	 void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		 target.draw(m_card, states);
	 }

	 std::weak_ptr<sf::Texture> m_deckTexture;
	 static const sf::Texture m_cardBack;
	 sf::Sprite m_card;
	 sf::IntRect m_deckRect{};
	 sf::Vector2f m_pos{};
	 Suit m_suit{ Suit::Diamond };
	 int m_rank{ 1 };
	 bool m_isVisible{ false };
};

const sf::Texture Card::m_cardBack{ "Card Asset/Standard 52 Cards/solitaire/individuals/card back/card_back.png" };

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
			 if (card) {
				 window.draw(*card);
			 }
		 }
	 }

	 void showCard(sf::RenderWindow& window) const {
		 window.draw(*m_deck[13]);
	 }

	 std::unique_ptr<Card>& getRandomCard() {
		 int pos = Random::get<int>(0, m_deck.size() - 1);
		 
		 if (!m_deck[pos]) {
			 return getRandomCard();  // this is not good at all, but it's funny :)
		 }
		 
		 return m_deck[pos];
	 }

     std::array<std::unique_ptr<Card>, 52>& getDeck() {
         return m_deck;
     }

 private:
	 void fillDeck() {
		 int height = m_deckTexture.get()->getSize().y / 4;
		 int width = m_deckTexture.get()->getSize().x / 13;

		 for (int j{ 0 }; j < 4; ++j) {
			 for (int i{ 0 }; i < 13; ++i) {
				 m_deck[j * 13 + i] = std::move(std::make_unique<Card>(
					 sf::IntRect({ width * i, height * j }, { width, height }),
					 m_deckTexture, true));
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
	 
	 void takeCard(std::unique_ptr<Card>& card) {
		 m_cards.push_back(std::move(card));
	 }

	 void displayCards(sf::RenderWindow& window) const {
		 for (const auto& card : m_cards) {
			 window.draw(*card);
		 }
	 }

	 void placeCardsInStockPile(StockPile& stockPile) {
		 for (auto& card : stockPile.getDeck()) {
			 if (!card) {
				 std::unique_ptr<Card> m_card = std::move(m_cards.back());
				 m_cards.pop_back();
				 std::swap(card, m_card);
			 }
		 }
		 m_cards.clear();
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
