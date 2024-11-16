#include "card.h"

Card::Card() {}

void Card::setPoint(CardPoint point) { _point = point; }
void Card::setSuit(CardSuit suit) { _suit = suit; }

Card::CardPoint Card::point() const { return _point; }
Card::CardSuit Card::suit() const { return _suit; }
