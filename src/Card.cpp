#include "../include/Card.hpp"

Card::Card() : m_card(m_cardBack) {}

Card::Card(sf::IntRect&& deckRect, std::shared_ptr<sf::Texture> deckTexture,
           bool isVisible)
    : m_deckTexture(deckTexture),
      m_deckRect(deckRect),
      m_pos({500, 250}),
      m_card(m_cardBack),
      m_isVisible(isVisible) {
  m_suit = static_cast<Suit>(m_deckRect.position.y / m_deckRect.size.y);
  m_rank = static_cast<int>(m_deckRect.position.x / m_deckRect.size.x) + 1;

  if (m_isVisible) {
    m_card.setTexture(*m_deckTexture.lock());
    m_card.setTextureRect(m_deckRect);
    m_card.setScale({0.5f, 0.5f});
    m_card.setPosition(m_pos);
  }
}

Card::~Card() { m_deckTexture.reset(); }

const sf::Texture Card::m_cardBack{
    "Card Asset/Standard 52 Cards/solitaire/individuals/card "
    "back/card_back.png"};
