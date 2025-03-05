#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
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
int endingPointY = 20;

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

bool allVisited(bool (&visitedRecord)[BOXES_COL][BOXES_ROW]) {

  for (int i = 0; i < BOXES_COL; i++) {
    for (int j = 0; j < BOXES_COL; j++) {
      if (visitedRecord[i][j] == false)
        return false;
    }
  }

  return true;
}

struct MinimumPoint
minimumUnvisited(int (&distance)[BOXES_COL][BOXES_ROW],
                 bool (&visitedRecord)[BOXES_COL][BOXES_ROW]) {

  MinimumPoint shortest = {0, 0, INT_MAX};

  for (int i = 0; i < BOXES_COL; i++) {
    for (int j = 0; j < BOXES_COL; j++) {
      if (distance[i][j] < shortest.distance && !visitedRecord[i][j]) {
        shortest = {i, j, distance[i][j]};
      }
    }
  }
  return shortest;
}

void walk(Dir direction, MinimumPoint node,
          int (&distance)[BOXES_COL][BOXES_ROW],
          Point (&prev)[BOXES_COL][BOXES_ROW]) {

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

  if (p.x > BOXES_ROW || p.x < 0 || p.y > BOXES_COL || p.y < 0) {
    return;
  }

  if (distance[p.x][p.y] > node.distance + 1) {
    distance[p.x][p.y] = node.distance + 1;
    prev[p.x][p.y] = {node.x, node.y};
  }
}

int dijkstra(Point start, vector<Point> &animationBuffer) {
  bool visitedRecord[BOXES_COL][BOXES_ROW];
  int distance[BOXES_COL][BOXES_ROW];
  Point prev[BOXES_COL][BOXES_ROW];

  for (int i = 0; i < BOXES_COL; i++) {
    for (int j = 0; j < BOXES_COL; j++) {
      visitedRecord[i][j] = false;
    }
  }

  for (int i = 0; i < BOXES_COL; i++) {
    for (int j = 0; j < BOXES_COL; j++) {
      distance[i][j] = INT_MAX;
    }
  }

  distance[start.x][start.y] = 0;
  int loopCounter = 0;
  while (!allVisited(visitedRecord)) {
    loopCounter++;
    MinimumPoint node = minimumUnvisited(distance, visitedRecord);
    cout << node.x << " " << node.y << endl;

    if (node.distance == INT_MAX)
      break;

    walk(Dir::up, node, distance, prev);
    walk(Dir::down, node, distance, prev);
    walk(Dir::left, node, distance, prev);
    walk(Dir::right, node, distance, prev);

    visitedRecord[node.x][node.y] = true;
    if (node.x == endingPointX && node.y == endingPointY) {
      // found the node
      break;
    } else {
      animationBuffer.push_back({node.x, node.y});
    }
  }
  return loopCounter + distance[endingPointX][endingPointY] + 1;
}

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

void drawAnimation(RenderWindow *window, vector<Point> &animationBuffer) {

  for (int i = 0; i < animationBuffer.size(); i++) {

    Point p = animationBuffer[i];

    int startX = PADDING_LEFT_RIGHT;
    int startY = PADDING_TOP_BOTTOM;

    int x = startX + p.x * SQUARE_BOX;
    int y = startY + p.y * SQUARE_BOX;

    sf::RectangleShape drawableRect1(
        sf::Vector2f(SQUARE_BOX - 2 * BORDER, SQUARE_BOX - 2 * BORDER));
    drawableRect1.setPosition(sf::Vector2f(x + BORDER, y + BORDER));
    drawableRect1.setFillColor(sf::Color::Yellow);
  }
}

int main() {

  RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Path Finding Algorithm");
  IntRect graph[BOXES_COL][BOXES_ROW];

  initGraph(graph);
  vector<Point> animationBuffer;
  cout << dijkstra({startingPointX, startingPointY}, animationBuffer);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<Event::Closed>())
        window.close();
    }

    window.clear(Color::Black);

    // Draw Start
    drawGrid(&window, graph);

    // starting and ending point
    // drawAnimation(&window, animationBuffer);

    // Draw Stop
    window.display();
  }
}
