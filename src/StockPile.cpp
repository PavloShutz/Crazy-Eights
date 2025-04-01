#include "../include/StockPile.hpp"

StockPile::StockPile(const std::filesystem::path& filename)
    : m_deckTexture{ std::make_shared<sf::Texture>(filename) } {
  m_deckTexture.get()->setSmooth(true);
  fillDeck();
}

void StockPile::shuffle() {
  for (int i{0}; i < 1000; ++i) {
    int pos1 = Random::get<int>(0, m_deck.size() - 1);
    int pos2 = Random::get<int>(0, m_deck.size() - 1);
    std::swap(m_deck[pos1], m_deck[pos2]);
  }
}

void StockPile::displayCards(sf::RenderWindow& window) const {
  for (const auto& card : m_deck) {
    if (card) {
      window.draw(*card);
    }
  }
}

void StockPile::showCard(sf::RenderWindow& window) const {
  window.draw(*m_deck[13]);
}

std::unique_ptr<Card>& StockPile::getRandomCard() {
  int pos = Random::get<int>(0, m_deck.size() - 1);

  while (!m_deck[pos]) {
    pos = Random::get<int>(0, m_deck.size() - 1);
  }
  
  return m_deck[pos];
}

std::array<std::unique_ptr<Card>, 52>& StockPile::getDeck() {
    return m_deck;
}

void StockPile::fillDeck() {
  int height = m_deckTexture.get()->getSize().y / 4;
  int width = m_deckTexture.get()->getSize().x / 13;

  for (int j{0}; j < 4; ++j) {
    for (int i{0}; i < 13; ++i) {
      m_deck[j * 13 + i] = std::move(std::make_unique<Card>(
          sf::IntRect({width * i, height * j}, {width, height}), m_deckTexture,
          true));
    }
  }
}