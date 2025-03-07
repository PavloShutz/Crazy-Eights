#pragma once

#include <SFML/Graphics.hpp>

#include "../include/StockPile.hpp"

class Player {
 public:
  Player() = default;

  void takeCard(std::unique_ptr<Card>& card);

  void displayCards(sf::RenderWindow& window) const;

  void placeCardsInStockPile(StockPile& stockPile);

 private:
  std::vector<std::unique_ptr<Card>> m_cards;
};
