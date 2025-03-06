#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <vector>

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
int endingPointY = 10;
bool isDragging_start = false;
bool isDragging_end = false;
int isbuildingWall = 0;
int dragOffsetStartX = 0;
int dragOffsetStartY = 0;
int dragOffsetEndX = 0;
int dragOffsetEndY = 0;
bool walled[BOXES_ROW][BOXES_COL];
enum Dir { up = 1, down, left, right };

struct Point {
  int x;
  int y;
};

struct MinimumPoint {
  int x;
  int y;
  int distance;
};

bool allVisited(bool (&visitedRecord)[BOXES_ROW][BOXES_COL],
                bool (&walled)[BOXES_ROW][BOXES_COL]) {

  for (int i = 0; i < BOXES_ROW; i++) {
    for (int j = 0; j < BOXES_COL; j++) {
      if (visitedRecord[i][j] == false && !walled[i][j])
        return false;
    }
  }

  return true;
}

struct MinimumPoint
minimumUnvisited(int (&distance)[BOXES_ROW][BOXES_COL],
                 bool (&visitedRecord)[BOXES_ROW][BOXES_COL]) {

  MinimumPoint shortest = {0, 0, INT_MAX};

  for (int i = 0; i < BOXES_ROW; i++) {
    for (int j = 0; j < BOXES_COL; j++) {
      if (distance[i][j] < shortest.distance && !visitedRecord[i][j]) {
        shortest = {i, j, distance[i][j]};
      }
    }
  }
  return shortest;
}

void walk(Dir direction, MinimumPoint node,
          int (&distance)[BOXES_ROW][BOXES_COL],
          Point (&prev)[BOXES_ROW][BOXES_COL],
          bool (&walled)[BOXES_ROW][BOXES_COL]) {

  Point p;
  if (direction == Dir::up) {
    p = {node.x, node.y - 1};
  } else if (direction == Dir::down) {
    p = {node.x, node.y + 1};
  } else if (direction == Dir::left) {
    p = {node.x - 1, node.y};
  } else {
    p = {node.x + 1, node.y};
  }

  if (p.x > BOXES_ROW - 1 || p.x < 0 || p.y > BOXES_COL - 1 || p.y < 0 ||
      walled[p.x][p.y]) {
    return;
  }

  if (distance[p.x][p.y] > node.distance + 1) {
    distance[p.x][p.y] = node.distance + 1;
    prev[p.x][p.y] = {node.x, node.y};
  }
}

int dijkstra(Point start, vector<Point> &animationBuffer,
             bool (&walled)[BOXES_ROW][BOXES_COL], vector<Point> &path) {
  bool visitedRecord[BOXES_ROW][BOXES_COL];
  int distance[BOXES_ROW][BOXES_COL];
  Point prev[BOXES_ROW][BOXES_COL];

  for (int i = 0; i < BOXES_ROW; i++) {
    for (int j = 0; j < BOXES_COL; j++) {
      visitedRecord[i][j] = false;
    }
  }

  for (int i = 0; i < BOXES_ROW; i++) {
    for (int j = 0; j < BOXES_COL; j++) {
      distance[i][j] = INT_MAX;
    }
  }
  bool found = true;
  distance[start.x][start.y] = 0;
  int loopCounter = 0;
  while (!allVisited(visitedRecord, walled)) {
    loopCounter++;
    MinimumPoint node = minimumUnvisited(distance, visitedRecord);

    if (node.distance == INT_MAX)
      break;

    walk(Dir::up, node, distance, prev, walled);
    walk(Dir::down, node, distance, prev, walled);
    walk(Dir::left, node, distance, prev, walled);
    walk(Dir::right, node, distance, prev, walled);

    visitedRecord[node.x][node.y] = true;
    if (node.x == endingPointX && node.y == endingPointY) {
      // found the node

      found = true;
      break;
    } else {
      animationBuffer.push_back({node.x, node.y});
    }
  }

  Point node = {endingPointX, endingPointY};

  while (!(node.x == startingPointX && node.y == startingPointY)) {
    path.push_back(node);
    node = prev[node.x][node.y];
  }
  path.push_back({startingPointX, startingPointY});
  reverse(path.begin(), path.end());
  return distance[endingPointX][endingPointY] + 1;
}

void drawGrid(RenderWindow *window, bool (&walled)[BOXES_ROW][BOXES_COL]) {

  int startX = PADDING_LEFT_RIGHT;
  int startY = PADDING_TOP_BOTTOM;

  for (int i = 0; i < BOXES_ROW; i++) {
    for (int j = 0; j < BOXES_COL; j++) {

      int x = startX + i * SQUARE_BOX;
      int y = startY + j * SQUARE_BOX;

      sf::IntRect rect({x, y}, {SQUARE_BOX, SQUARE_BOX});
      sf::RectangleShape drawableRect(sf::Vector2f(SQUARE_BOX, SQUARE_BOX));
      drawableRect.setPosition(sf::Vector2f(x, y));
      drawableRect.setFillColor(sf::Color::Green);
      window->draw(drawableRect);

      if (i == startingPointX && j == startingPointY) {
        sf::RectangleShape drawableRect1(
            sf::Vector2f(SQUARE_BOX - 2 * BORDER, SQUARE_BOX - 2 * BORDER));
        drawableRect1.setPosition(sf::Vector2f(x + BORDER, y + BORDER));
        drawableRect1.setFillColor(sf::Color::Red);
        window->draw(drawableRect1);
      } else if (i == endingPointX && j == endingPointY) {
        sf::RectangleShape drawableRect1(
            sf::Vector2f(SQUARE_BOX - 2 * BORDER, SQUARE_BOX - 2 * BORDER));
        drawableRect1.setPosition(sf::Vector2f(x + BORDER, y + BORDER));
        drawableRect1.setFillColor(sf::Color::Blue);
        window->draw(drawableRect1);
      } else if (walled[i][j]) {

        sf::RectangleShape drawableRect1(
            sf::Vector2f(SQUARE_BOX - 2 * BORDER, SQUARE_BOX - 2 * BORDER));
        drawableRect1.setPosition(sf::Vector2f(x + BORDER, y + BORDER));
        drawableRect1.setFillColor(sf::Color::Magenta);
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

void drawAnimation(RenderWindow *window, vector<Point> &animationBuffer,
                   vector<Point> &path, int counter) {

  int pointsCounter = 0;

  for (int i = 0; i < animationBuffer.size(); i++) {

    if (pointsCounter > counter)
      return;
    pointsCounter++;
    Point p = animationBuffer[i];
    int startX = PADDING_LEFT_RIGHT;
    int startY = PADDING_TOP_BOTTOM;

    int x = startX + p.x * SQUARE_BOX;
    int y = startY + p.y * SQUARE_BOX;

    if (!(p.x == startingPointX && p.y == startingPointY) &&
        !(p.x == endingPointX && p.y == endingPointY)) {
      sf::RectangleShape drawableRect1(
          sf::Vector2f(SQUARE_BOX - 2 * BORDER, SQUARE_BOX - 2 * BORDER));
      drawableRect1.setPosition(sf::Vector2f(x + BORDER, y + BORDER));
      drawableRect1.setFillColor(sf::Color::Yellow);
      window->draw(drawableRect1);
    }
  }

  for (int i = 0; i < path.size(); i++) {

    if (pointsCounter > counter)
      return;

    pointsCounter++;
    Point p = path[i];
    int startX = PADDING_LEFT_RIGHT;
    int startY = PADDING_TOP_BOTTOM;

    int x = startX + p.x * SQUARE_BOX;
    int y = startY + p.y * SQUARE_BOX;

    if (!(p.x == startingPointX && p.y == startingPointY) &&
        !(p.x == endingPointX && p.y == endingPointY)) {
      sf::RectangleShape drawableRect1(
          sf::Vector2f(SQUARE_BOX - 2 * BORDER, SQUARE_BOX - 2 * BORDER));
      drawableRect1.setPosition(sf::Vector2f(x + BORDER, y + BORDER));
      drawableRect1.setFillColor(sf::Color::Cyan);
      window->draw(drawableRect1);
    }
  }
}
void initwalls() {
  for (int i = 0; i < BOXES_ROW; i++) {
    for (int j = 0; j < BOXES_COL; j++) {
      walled[i][j] = false;
      // Test wall
      // if (i == 15 && j != 0) {
      //   walled[i][j] = true;
      // }
    }
  }
}
int main() {

  RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Path Finding Algorithm");

  vector<Point> animationBuffer;

  sf::Font font;
  font.loadFromFile("font.ttf"); // You must provide the correct font path!

  // Start & Reset Buttons (Above the Grid)
  sf::RectangleShape startButton(sf::Vector2f(200, 50));
  startButton.setPosition(
      sf::Vector2f(WIDTH / 2 - 220, PADDING_TOP_BOTTOM / 2 - 25));
  startButton.setFillColor(sf::Color::Green);

  sf::Text startText;
  startText.setFont(font);
  startText.setString("START");
  startText.setCharacterSize(24);
  startText.setFillColor(sf::Color::Black);
  startText.setPosition(startButton.getPosition().x + 60,
                        startButton.getPosition().y + 10);

  sf::RectangleShape resetButton(sf::Vector2f(200, 50));
  resetButton.setPosition(
      sf::Vector2f(WIDTH / 2 + 20, PADDING_TOP_BOTTOM / 2 - 25));
  resetButton.setFillColor(sf::Color::Red);

  sf::Text resetText;
  resetText.setFont(font);
  resetText.setString("RESET");
  resetText.setCharacterSize(24);
  resetText.setFillColor(sf::Color::Black);
  resetText.setPosition(resetButton.getPosition().x + 60,
                        resetButton.getPosition().y + 10);

  // Speed Buttons (Below the Grid)
  sf::RectangleShape speedButton1x(sf::Vector2f(80, 40));
  speedButton1x.setPosition(
      sf::Vector2f(WIDTH / 2 - 120, HEIGHT - PADDING_TOP_BOTTOM / 2 - 20));
  speedButton1x.setFillColor(sf::Color::Cyan);

  sf::Text speedText1x;
  speedText1x.setFont(font);
  speedText1x.setString("1x");
  speedText1x.setCharacterSize(20);
  speedText1x.setFillColor(sf::Color::Black);
  speedText1x.setPosition(speedButton1x.getPosition().x + 25,
                          speedButton1x.getPosition().y + 10);

  sf::RectangleShape speedButton05x(sf::Vector2f(80, 40));
  speedButton05x.setPosition(
      sf::Vector2f(WIDTH / 2, HEIGHT - PADDING_TOP_BOTTOM / 2 - 20));
  speedButton05x.setFillColor(sf::Color::Magenta);

  sf::Text speedText05x;
  speedText05x.setFont(font);
  speedText05x.setString("0.5x");
  speedText05x.setCharacterSize(20);
  speedText05x.setFillColor(sf::Color::Black);
  speedText05x.setPosition(speedButton05x.getPosition().x + 15,
                           speedButton05x.getPosition().y + 10);

  sf::RectangleShape speedButton025x(sf::Vector2f(80, 40));
  speedButton025x.setPosition(
      sf::Vector2f(WIDTH / 2 + 120, HEIGHT - PADDING_TOP_BOTTOM / 2 - 20));
  speedButton025x.setFillColor(sf::Color::Yellow);

  sf::Text speedText025x;
  speedText025x.setFont(font);
  speedText025x.setString("0.25x");
  speedText025x.setCharacterSize(20);
  speedText025x.setFillColor(sf::Color::Black);
  speedText025x.setPosition(speedButton025x.getPosition().x + 10,
                            speedButton025x.getPosition().y + 10);

  vector<Point> path;

  initwalls();

  int counter = 0;
  int speed = 0;
  int speedMode = 5;
  bool animationStarted = false;
  while (window.isOpen()) {
    sf::Event event; // Declare an sf::Event object to hold event data
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (resetButton.getGlobalBounds().contains(
                window.mapPixelToCoords(mousePos))) {
          animationBuffer.clear();
          animationStarted = false;
          counter = 0;
          speed = 0;
          speedMode = 5;
          initwalls();
          path.clear();
        }

        // Speed button interactions
        if (speedButton1x.getGlobalBounds().contains(
                window.mapPixelToCoords(mousePos))) {
          speedMode = 5;
        } else if (speedButton05x.getGlobalBounds().contains(
                       window.mapPixelToCoords(mousePos))) {
          speedMode = 10;
        } else if (speedButton025x.getGlobalBounds().contains(
                       window.mapPixelToCoords(mousePos))) {
          speedMode = 20;
        }

        if (startButton.getGlobalBounds().contains(
                window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
          animationStarted = true;
          animationBuffer.clear();
          dijkstra({startingPointX, startingPointY}, animationBuffer, walled,
                   path);
        }

        // Start point dragging
        if (animationStarted) {
          continue;
        }

        if (mousePos.x >= PADDING_LEFT_RIGHT + startingPointX * SQUARE_BOX &&
            mousePos.x <=
                PADDING_LEFT_RIGHT + (startingPointX + 1) * SQUARE_BOX &&
            mousePos.y >= PADDING_TOP_BOTTOM + startingPointY * SQUARE_BOX &&
            mousePos.y <=
                PADDING_TOP_BOTTOM + (startingPointY + 1) * SQUARE_BOX) {

          isDragging_start = true;
          dragOffsetStartX =
              mousePos.x - (PADDING_LEFT_RIGHT + startingPointX * SQUARE_BOX);
          dragOffsetStartY =
              mousePos.y - (PADDING_TOP_BOTTOM + startingPointY * SQUARE_BOX);
        }
        // End point dragging
        else if (mousePos.x >= PADDING_LEFT_RIGHT + endingPointX * SQUARE_BOX &&
                 mousePos.x <=
                     PADDING_LEFT_RIGHT + (endingPointX + 1) * SQUARE_BOX &&
                 mousePos.y >= PADDING_TOP_BOTTOM + endingPointY * SQUARE_BOX &&
                 mousePos.y <=
                     PADDING_TOP_BOTTOM + (endingPointY + 1) * SQUARE_BOX) {

          isDragging_end = true;
          dragOffsetEndX =
              mousePos.x - (PADDING_LEFT_RIGHT + endingPointX * SQUARE_BOX);
          dragOffsetEndY =
              mousePos.y - (PADDING_TOP_BOTTOM + endingPointY * SQUARE_BOX);
        } else {
          isbuildingWall = true;
        }
      }

      if (event.type == sf::Event::MouseButtonReleased) {
        isDragging_end = false;
        isDragging_start = false;
        isbuildingWall = false;
      }

      if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (isDragging_start) {
          int newStartX =
              (mousePos.x - dragOffsetStartX - PADDING_LEFT_RIGHT) / SQUARE_BOX;
          int newStartY =
              (mousePos.y - dragOffsetStartY - PADDING_TOP_BOTTOM) / SQUARE_BOX;

          if (newStartX >= 0 && newStartX < BOXES_ROW && newStartY >= 0 &&
              newStartY < BOXES_COL) {
            startingPointX = newStartX;
            startingPointY = newStartY;
          }
        }

        if (isDragging_end) {
          int newEndX =
              (mousePos.x - dragOffsetEndX - PADDING_LEFT_RIGHT) / SQUARE_BOX;
          int newEndY =
              (mousePos.y - dragOffsetEndY - PADDING_TOP_BOTTOM) / SQUARE_BOX;

          if (newEndX >= 0 && newEndX < BOXES_ROW && newEndY >= 0 &&
              newEndY < BOXES_COL) {
            endingPointX = newEndX;
            endingPointY = newEndY;
          }
        }

        if (isbuildingWall) {
          int gridX = (mousePos.x - PADDING_LEFT_RIGHT) / SQUARE_BOX;
          int gridY = (mousePos.y - PADDING_TOP_BOTTOM) / SQUARE_BOX;

          if (gridX >= 0 && gridX < BOXES_ROW && gridY >= 0 &&
              gridY < BOXES_COL) {
            if (!(gridX == startingPointX && gridY == startingPointY) &&
                !(gridX == endingPointX && gridY == endingPointY)) {
              walled[gridX][gridY] = !walled[gridX][gridY];
            }
          }
        }
      }
    }

    window.clear(sf::Color::White);

    // Draw Start
    drawGrid(&window, walled);
    // Draw buttons
    window.draw(startButton);
    window.draw(resetButton);
    window.draw(speedButton1x);
    window.draw(speedButton05x);
    window.draw(speedButton025x);

    // Draw text on buttons
    window.draw(startText);
    window.draw(resetText);
    window.draw(speedText1x);
    window.draw(speedText05x);
    window.draw(speedText025x);
    if (animationStarted == true) {
      drawAnimation(&window, animationBuffer, path, counter);
      if (speed % speedMode == 0) {
        counter++;
      }
      speed++;
    }

    // Draw Stop
    window.display();
  }
}
