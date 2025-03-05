#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

const int HEIGHT = 500;
const int WIDTH = 1000;

const int PADDING_TOP_BOTTOM = 60;
const int PADDING_LEFT_RIGHT = 20;

const int SQUARE_BOX = 20;
const int BORDER = 1;

const int BOXES_ROW = (WIDTH - 2 * PADDING_LEFT_RIGHT) / SQUARE_BOX;
const int BOXES_COL = (HEIGHT - 2 * PADDING_TOP_BOTTOM) / SQUARE_BOX;

int startingPointX = 5;
int startingPointY = 10;
int endingPointX = 30;
int endingPointY = 20;

class Point {
  int x;
  int y;
};

void drawGrid(RenderWindow *window, IntRect (&graph)[BOXES_COL][BOXES_ROW]) {

  int startX = PADDING_LEFT_RIGHT;
  int startY = PADDING_TOP_BOTTOM;

  for (int j = 0; j < BOXES_COL; j++) {
    for (int i = 0; i < BOXES_ROW; i++) {

      int x = startX + i * SQUARE_BOX;
      int y = startY + j * SQUARE_BOX;

      sf::IntRect rect({x, y}, {SQUARE_BOX, SQUARE_BOX});
      sf::RectangleShape drawableRect(sf::Vector2f(SQUARE_BOX, SQUARE_BOX));
      drawableRect.setPosition(sf::Vector2f(x, y));
      drawableRect.setFillColor(sf::Color::Green);
      window->draw(drawableRect);

      if (i == 5 && j == 10) {
        sf::RectangleShape drawableRect1(
            sf::Vector2f(SQUARE_BOX - 2 * BORDER, SQUARE_BOX - 2 * BORDER));
        drawableRect1.setPosition(sf::Vector2f(x + BORDER, y + BORDER));
        drawableRect1.setFillColor(sf::Color::Red);
        window->draw(drawableRect1);
      } else if (i == 30 && j == 13) {
        sf::RectangleShape drawableRect1(
            sf::Vector2f(SQUARE_BOX - 2 * BORDER, SQUARE_BOX - 2 * BORDER));
        drawableRect1.setPosition(sf::Vector2f(x + BORDER, y + BORDER));
        drawableRect1.setFillColor(sf::Color::Blue);
        window->draw(drawableRect1);
      } else {

        sf::RectangleShape drawableRect1(
            sf::Vector2f(SQUARE_BOX - 2 * BORDER, SQUARE_BOX - 2 * BORDER));
        drawableRect1.setPosition(sf::Vector2f(x + BORDER, y + BORDER));
        drawableRect1.setFillColor(sf::Color::White);
        window->draw(drawableRect1);
      }
    }
  }
}

void initGraph(IntRect (&graph)[BOXES_COL][BOXES_ROW]) {
  int startX = PADDING_LEFT_RIGHT;
  int startY = PADDING_TOP_BOTTOM;

  for (int j = 0; j < BOXES_COL; j++) {
    for (int i = 0; i < BOXES_ROW; i++) {

      int x = startX + i * SQUARE_BOX;
      int y = startY + j * SQUARE_BOX;
      IntRect rect({x + BORDER, y + BORDER},
                   {SQUARE_BOX - 2 * BORDER, SQUARE_BOX - 2 * BORDER});
      // graph[x][y] = rect;
    }
  }
}

int main() {
  RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Path Finding Algorithm");
  IntRect graph[BOXES_COL][BOXES_ROW];

  initGraph(graph);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<Event::Closed>())
        window.close();
    }

    window.clear(Color::Black);

    // Draw Start
    drawGrid(&window, graph);

    // starting and ending point

    // Draw Stop
    window.display();
  }
}
