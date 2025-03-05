#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
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

bool allVisited(bool (&visitedRecord)[BOXES_ROW][BOXES_COL]) {
  for (int i = 0; i < BOXES_ROW; i++) {
    for (int j = 0; j < BOXES_COL; j++) {
      if (visitedRecord[i][j] == false)
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
          Point (&prev)[BOXES_ROW][BOXES_COL], bool (&walled)[BOXES_ROW][BOXES_COL]) {
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

  if (p.x > BOXES_ROW - 1 || p.x < 0 || p.y > BOXES_COL - 1 || p.y < 0 || walled[p.x][p.y]) {
    return;
  }

  if (distance[p.x][p.y] > node.distance + 1) {
    cout << node.distance + 1 << " For " << p.x << " " << p.y << endl;
    distance[p.x][p.y] = node.distance + 1;
    prev[p.x][p.y] = {node.x, node.y};
  }
}

int dijkstra(Point start, vector<Point> &animationBuffer, bool (&walled)[BOXES_ROW][BOXES_COL]) {
  bool visitedRecord[BOXES_ROW][BOXES_COL] = {false};
  int distance[BOXES_ROW][BOXES_COL];
  Point prev[BOXES_ROW][BOXES_COL];

  for (int i = 0; i < BOXES_ROW; i++) {
    for (int j = 0; j < BOXES_COL; j++) {
      distance[i][j] = INT_MAX;
    }
  }

  distance[start.x][start.y] = 0;
  int loopCounter = 0;
  while (!allVisited(visitedRecord)) {
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
      cout << "Found " << distance[node.x][node.y] << endl;
      break;
    } else {
      animationBuffer.push_back({node.x, node.y});
    }
  }
  if (allVisited(visitedRecord)) {
    cout << "All" << endl;
  }
  return distance[endingPointX][endingPointY] + 1;
}

void drawGrid(RenderWindow *window, IntRect (&graph)[BOXES_ROW][BOXES_COL], bool (&walled)[BOXES_ROW][BOXES_COL]) {
  int startX = PADDING_LEFT_RIGHT;
  int startY = PADDING_TOP_BOTTOM;

  for (int i = 0; i < BOXES_ROW; i++) {
    for (int j = 0; j < BOXES_COL; j++) {
      int x = startX + i * SQUARE_BOX;
      int y = startY + j * SQUARE_BOX;

      sf::IntRect rect({x, y}, {SQUARE_BOX, SQUARE_BOX});
      sf::RectangleShape drawableRect(sf::Vector2f(SQUARE_BOX, SQUARE_BOX));
      drawableRect.setPosition(sf::Vector2f(x, y));
      
      // Wall drawing
      if (walled[i][j]) {
        drawableRect.setFillColor(sf::Color(128, 128, 128)); 
      } else {
        drawableRect.setFillColor(sf::Color::Green);
      }
      
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
      } else if (!walled[i][j]) {
        sf::RectangleShape drawableRect1(
            sf::Vector2f(SQUARE_BOX - 2 * BORDER, SQUARE_BOX - 2 * BORDER));
        drawableRect1.setPosition(sf::Vector2f(x + BORDER, y + BORDER));
        drawableRect1.setFillColor(sf::Color::White);
        window->draw(drawableRect1);
      }
    }
  }
}

void initGraph(IntRect (&graph)[BOXES_ROW][BOXES_COL]) {
  int startX = PADDING_LEFT_RIGHT;
  int startY = PADDING_TOP_BOTTOM;

  for (int j = 0; j < BOXES_ROW; j++) {
    for (int i = 0; i < BOXES_COL; i++) {
      int x = startX + i * SQUARE_BOX;
      int y = startY + j * SQUARE_BOX;
      IntRect rect({x + BORDER, y + BORDER},
                   {SQUARE_BOX - 2 * BORDER, SQUARE_BOX - 2 * BORDER});
    }
  }
}

void drawAnimation(RenderWindow *window, vector<Point> &animationBuffer,
                   int counter) {
  for (int i = 0; i < animationBuffer.size() && i < counter; i++) {
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
}

int main() {
  RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Path Finding Algorithm");
  IntRect graph[BOXES_ROW][BOXES_COL];
  bool animationStarted = false;
  bool walled[BOXES_ROW][BOXES_COL] = {false};

  // Speed control buttons
  RectangleShape startButton(Vector2f(200, 50));
  startButton.setPosition(sf::Vector2f(WIDTH / 2 - 100, HEIGHT - 70));
  startButton.setFillColor(Color::Green);

  RectangleShape resetButton(Vector2f(200, 50));
  resetButton.setPosition(sf::Vector2f(WIDTH / 2 - 100, HEIGHT - 140));
  resetButton.setFillColor(Color::Red);

  RectangleShape speedButton1x(Vector2f(80, 40));
  speedButton1x.setPosition(sf::Vector2f(WIDTH / 2 - 120, HEIGHT - 140));
  speedButton1x.setFillColor(Color::Cyan);

  RectangleShape speedButton05x(Vector2f(80, 40));
  speedButton05x.setPosition(sf::Vector2f(WIDTH / 2, HEIGHT - 140));
  speedButton05x.setFillColor(Color::Magenta);

  RectangleShape speedButton025x(Vector2f(80, 40));
  speedButton025x.setPosition(sf::Vector2f(WIDTH / 2 + 120, HEIGHT - 140));
  speedButton025x.setFillColor(Color::Yellow);

  // Text for speed buttons (YO CHAHI DON'T USE UNLESS TIMRO MA NI MERO JASTAI ERROR AAUXA HAHAHA)
  // Font font;
  // if (!font.loadFromFile("arial.ttf")) {
  //   cout << "Error loading font" << endl;
  //   return -1;
  // }

  // Text text1x("1x", font, 20);
  // text1x.setPosition(WIDTH / 2 - 110, HEIGHT - 130);
  // text1x.setFillColor(Color::Black);

  // Text text05x("0.5x", font, 20);
  // text05x.setPosition(WIDTH / 2 + 10, HEIGHT - 130);
  // text05x.setFillColor(Color::Black);

  // Text text025x("0.25x", font, 20);
  // text025x.setPosition(WIDTH / 2 + 130, HEIGHT - 130);
  // text025x.setFillColor(Color::Black);

  initGraph(graph);
  vector<Point> animationBuffer;
  int counter = 0;
  int speed = 0;
  int speedMode = 5; // Default speed mode

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<Event::Closed>())
        window.close();

      if (event->is<Event::MouseButtonPressed>()) {
        Vector2i mousePos = Mouse::getPosition(window);

         if (resetButton.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) {
          memset(walled, 0, sizeof(walled));
          animationBuffer.clear();
          animationStarted = false;
          counter = 0;
          speed = 0;
          speedMode = 5;
        }
         if (startButton.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
          animationStarted = true;
          animationBuffer.clear(); // Clear previous animation
          dijkstra({startingPointX, startingPointY}, animationBuffer, walled);
        }

        // Speed button interactions
        if (speedButton1x.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) {
          speedMode = 5; 
        }
        else if (speedButton05x.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) {
          speedMode = 10;
        }
        else if (speedButton025x.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) {
          speedMode = 20;
        }

        if (startButton.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))) {
          animationStarted = true;
          animationBuffer.clear(); 
          dijkstra({startingPointX, startingPointY}, animationBuffer, walled);
        }

        // Start point dragging
        if (mousePos.x >= PADDING_LEFT_RIGHT + startingPointX * SQUARE_BOX &&
            mousePos.x <= PADDING_LEFT_RIGHT + (startingPointX + 1) * SQUARE_BOX &&
            mousePos.y >= PADDING_TOP_BOTTOM + startingPointY * SQUARE_BOX &&
            mousePos.y <= PADDING_TOP_BOTTOM + (startingPointY + 1) * SQUARE_BOX) {
          
          isDragging_start = true;
          dragOffsetStartX = mousePos.x - (PADDING_LEFT_RIGHT + startingPointX * SQUARE_BOX);
          dragOffsetStartY = mousePos.y - (PADDING_TOP_BOTTOM + startingPointY * SQUARE_BOX);
        }
        // End point dragging
        else if (mousePos.x >= PADDING_LEFT_RIGHT + endingPointX * SQUARE_BOX &&
          mousePos.x <= PADDING_LEFT_RIGHT + (endingPointX + 1) * SQUARE_BOX &&
          mousePos.y >= PADDING_TOP_BOTTOM + endingPointY * SQUARE_BOX &&
          mousePos.y <= PADDING_TOP_BOTTOM + (endingPointY + 1) * SQUARE_BOX) {
          
          isDragging_end = true;
          dragOffsetEndX = mousePos.x - (PADDING_LEFT_RIGHT + endingPointX * SQUARE_BOX);
          dragOffsetEndY = mousePos.y - (PADDING_TOP_BOTTOM + endingPointY * SQUARE_BOX);
        }
        else {
          isbuildingWall = 1;
        }
      }

      if (event->is<Event::MouseButtonReleased>()) {
        isDragging_end = false;
        isDragging_start = false;
        isbuildingWall = 0;
      }

      if (event->is<Event::MouseMoved>()) {
        Vector2i mousePos = Mouse::getPosition(window);
        
        if (isDragging_start) {
        int newStartX = (mousePos.x - dragOffsetStartX - PADDING_LEFT_RIGHT) / SQUARE_BOX;
          int newStartY = (mousePos.y - dragOffsetStartY - PADDING_TOP_BOTTOM) / SQUARE_BOX;

          if (newStartX >= 0 && newStartX < BOXES_ROW && newStartY >= 0 && newStartY < BOXES_COL) {
            startingPointX = newStartX;
            startingPointY = newStartY;
          }
        }

        if (isDragging_end) {
          int newEndX = (mousePos.x - dragOffsetEndX - PADDING_LEFT_RIGHT) / SQUARE_BOX;
          int newEndY = (mousePos.y - dragOffsetEndY - PADDING_TOP_BOTTOM) / SQUARE_BOX;
          
          if (newEndX >= 0 && newEndX < BOXES_ROW && newEndY >= 0 && newEndY < BOXES_COL) {
            endingPointX = newEndX;
            endingPointY = newEndY;
          }
        }

        if (isbuildingWall) {
          int gridX = (mousePos.x - PADDING_LEFT_RIGHT) / SQUARE_BOX;
          int gridY = (mousePos.y - PADDING_TOP_BOTTOM) / SQUARE_BOX;

          if (gridX >= 0 && gridX < BOXES_ROW && gridY >= 0 && gridY < BOXES_COL) {
            if (!(gridX == startingPointX && gridY == startingPointY) && 
                !(gridX == endingPointX && gridY == endingPointY)) {
              walled[gridX][gridY] = !walled[gridX][gridY];
            }
          }
        }
      }

      window.clear(Color::Black);

      drawGrid(&window, graph, walled);

      window.draw(startButton);
      window.draw(resetButton);

      window.draw(speedButton1x);
      window.draw(speedButton05x);
      window.draw(speedButton025x);

      window.draw(text1x);
      window.draw(text05x);
      window.draw(text025x);

      if (animationStarted) {
        if (speed % speedMode == 0) {
          counter++;
        }
        drawAnimation(&window, animationBuffer, counter);
      }

      speed++;
      window.display();
    }
  }

  return 0;
}