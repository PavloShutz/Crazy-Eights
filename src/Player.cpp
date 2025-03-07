#include "../include/Player.hpp"

void Player::takeCard(std::unique_ptr<Card>& card) {
  m_cards.push_back(std::move(card));
}

void Player::displayCards(sf::RenderWindow& window) const {
  for (const auto& card : m_cards) {
    window.draw(*card);
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
