#include "../include/Player.hpp"

void Player::takeCard(std::unique_ptr<Card>& card) {
  card->setPosition(sf::Vector2f(25, 350));
  m_cards.push_back(std::move(card));
}

void Player::displayCards(sf::RenderWindow& window) const {
  float shiftX = 0.0;
  for (const auto& card : m_cards) {
    sf::Transform shift;
    shift.translate(sf::Vector2f(shiftX, 0.0));

    sf::RenderStates states(shift);
    
    window.draw(*card, states);

    shiftX += 25.0;
  }
}

void Player::placeCardsInStockPile(StockPile& stockPile) {
  for (auto& card : stockPile.getDeck()) {
    if (!card) {
      std::unique_ptr<Card> m_card = std::move(m_cards.back());
      m_cards.pop_back();
      std::swap(card, m_card);
    }
  }
  m_cards.clear();
}
