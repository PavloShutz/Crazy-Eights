#pragma once

#include <SFML/Graphics.hpp>

class Card : public sf::Drawable {
 public:
  enum class Suit { Diamond, Heart, Club, Spade };

  Card();

  Card(sf::IntRect&& deckRect, std::shared_ptr<sf::Texture> deckTexture,
       bool isVisible);

  ~Card();

  void setPosition(sf::Vector2f newPos);

 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(m_card, states);
  }

  std::weak_ptr<sf::Texture> m_deckTexture;
  static const sf::Texture m_cardBack;
  sf::Sprite m_card;
  sf::IntRect m_deckRect{};
  sf::Vector2f m_pos{};
  Suit m_suit{Suit::Diamond};
  int m_rank{1};
  bool m_isVisible{false};
};
