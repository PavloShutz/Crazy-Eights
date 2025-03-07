#pragma once

#include <SFML/Graphics.hpp>

#include "../include/Card.hpp"
#include "../include/Random.hpp"

class StockPile {
 public:
  explicit StockPile(const std::filesystem::path& filename);

  void shuffle();

  void displayCards(sf::RenderWindow& window) const;

  void showCard(sf::RenderWindow& window) const;

  std::unique_ptr<Card>& getRandomCard();

  std::array<std::unique_ptr<Card>, 52>& getDeck();

 private:
  void fillDeck();

 private:
  std::shared_ptr<sf::Texture> m_deckTexture;
  std::array<std::unique_ptr<Card>, 52> m_deck;
};
