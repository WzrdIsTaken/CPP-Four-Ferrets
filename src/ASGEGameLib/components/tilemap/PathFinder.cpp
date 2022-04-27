// Created by Ben on 25/03/2022
// Resource: https://www.youtube.com/watch?v=icZj67PTFhc

#include "../../../../include/ASGEGameLib/components/tilemap/PathFinder.hpp"
#include <iostream>
#include <list>

bcw::PathFinder::PathFinder(
  const size_t _map_width, const size_t _map_height, // NOLINT
  const AllowedDirections allowed_directions) :
  map_width(_map_width),
  map_height(_map_height)
{
  nodes = std::make_unique<Node[]>(map_width * map_height); // NOLINT do what i want

  for (size_t x = 0; x < map_width; x++)
  {
    for (size_t y = 0; y < map_height; y++)
    {
      Node* node     = findNode(x, y);
      node->x        = x;
      node->y        = y;
      node->obstacle = false;
      node->parent   = nullptr;
      node->visited  = false;

      connectNeighbours(node, x, y);

      /*
      // Straight Connections
      if (
        allowed_directions == AllowedDirections::BOTH ||
        allowed_directions == AllowedDirections::STRAIGHT)
      {
        if (y > 0)
        {
          node->neighbours.push_back(&nodes[(y - 1) * map_width + (x + 0)]);
        }
        if (y < map_height - 1)
        {
          node->neighbours.push_back(&nodes[(y + 1) * map_width + (x + 0)]);
        }
        if (x > 0)
        {
          node->neighbours.push_back(&nodes[(y + 0) * map_width + (x - 1)]);
        }
        if (x < map_width - 1)
        {
          node->neighbours.push_back(&nodes[(y + 0) * map_width + (x + 1)]);
        }
      }

      // Diagonal Connections
      if (
        allowed_directions == AllowedDirections::BOTH ||
        allowed_directions == AllowedDirections::DIAGONAL)
      {
        if (y > 0 && x > 0)
        {
          node->neighbours.push_back(&nodes[(y - 1) * map_width + (x - 1)]);
        }
        if (y < map_height - 1 && x > 0)
        {
          node->neighbours.push_back(&nodes[(y + 1) * map_width + (x - 1)]);
        }
        if (y > 0 && x < map_width - 1)
        {
          node->neighbours.push_back(&nodes[(y - 1) * map_width + (x + 1)]);
        }
        if (y < map_height - 1 && x < map_width - 1)
        {
          node->neighbours.push_back(&nodes[(y + 1) * map_width + (x + 1)]);
        }
      }*/
    }
  }
}

std::deque<bcw::Vector2> bcw::PathFinder::findPath(
  const size_t start_x, const size_t start_y, const size_t end_x, const size_t end_y)
{
  // Goals
  Node* start_node = findNode(start_x, start_y);
  Node* end_node   = findNode(end_x, end_y);

  // Reset all the nodes to a known state
  for (size_t x = 0; x < map_width; x++)
  {
    for (size_t y = 0; y < map_height; y++)
    {
      Node* node        = findNode(x, y);
      node->visited     = false;
      node->global_goal = INFINITY;
      node->local_goal  = INFINITY;
      node->parent      = nullptr;
    }
  }

  // Cool lambda functions
  auto distance = [](Node* a, Node* b)
  {
    return sqrtf(static_cast<float>((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y)));
  };

  auto heuristic = [distance](Node* a, Node* b) { return distance(a, b); };

  // Setup starting conditions
  Node* current_node{};
  start_node->local_goal  = 0.0F;
  start_node->global_goal = heuristic(start_node, end_node);

  std::list<Node*> untested_nodes;
  untested_nodes.push_back(start_node);

  // Actually solve!
  while (!untested_nodes.empty()) // && (greedy_search && current_node != end_node)
  {
    untested_nodes.sort([](const Node* lhs, const Node* rhs)
                        { return lhs->global_goal < rhs->global_goal; });

    while (!untested_nodes.empty() && untested_nodes.front()->visited)
    {
      untested_nodes.pop_front();
    }

    if (untested_nodes.empty())
    {
      break;
    }

    current_node          = untested_nodes.front();
    current_node->visited = true;

    for (auto* neighbour_node : current_node->neighbours)
    {
      if (!neighbour_node->visited && !neighbour_node->obstacle)
      {
        untested_nodes.push_back(neighbour_node);
      }

      float heuristic_local_goal =
        current_node->local_goal + distance(current_node, neighbour_node);
      if (heuristic_local_goal < neighbour_node->local_goal)
      {
        neighbour_node->parent     = current_node;
        neighbour_node->local_goal = heuristic_local_goal;

        neighbour_node->global_goal =
          neighbour_node->local_goal + heuristic(neighbour_node, end_node);
      }
    }
  }

  return backtrack(end_node);
}

void bcw::PathFinder::setBlockingProperty(int x, int y, bool value)
{
  findNode(static_cast<uint64_t>(x), static_cast<uint64_t>(y))->obstacle = value;
}

bcw::PathFinder::Node* bcw::PathFinder::findNode(size_t x, size_t y)
{
  return &nodes[y * map_width + x];
}

std::deque<bcw::Vector2> bcw::PathFinder::backtrack(bcw::PathFinder::Node* end_node)
{
  std::deque<bcw::Vector2> path;
  Node* p               = end_node;
  const float TILE_SIZE = 32.0F;

  while (p->parent != nullptr)
  {
    path.push_front({ static_cast<float>(p->parent->x) * TILE_SIZE,
                      static_cast<float>(p->parent->y) * TILE_SIZE });
    p = p->parent;
  }

  return path;
}

void bcw::PathFinder::connectNeighbours(Node* node, size_t x, size_t y)
{
  if (y > 0)
  {
    node->neighbours.push_back(&nodes[(y - 1) * map_width + (x + 0)]);
  }
  if (y < map_height - 1)
  {
    node->neighbours.push_back(&nodes[(y + 1) * map_width + (x + 0)]);
  }
  if (x > 0)
  {
    node->neighbours.push_back(&nodes[(y + 0) * map_width + (x - 1)]);
  }
  if (x < map_width - 1)
  {
    node->neighbours.push_back(&nodes[(y + 0) * map_width + (x + 1)]);
  }
  if (y > 0 && x > 0)
  {
    node->neighbours.push_back(&nodes[(y - 1) * map_width + (x - 1)]);
  }
  if (y < map_height - 1 && x > 0)
  {
    node->neighbours.push_back(&nodes[(y + 1) * map_width + (x - 1)]);
  }
  if (y > 0 && x < map_width - 1)
  {
    node->neighbours.push_back(&nodes[(y - 1) * map_width + (x + 1)]);
  }
  if (y < map_height - 1 && x < map_width - 1)
  {
    node->neighbours.push_back(&nodes[(y + 1) * map_width + (x + 1)]);
  }
}

void bcw::PathFinder::printNodeTree()
{
  for (size_t i = 0; i < size_t(map_width * map_height); ++i)
  {
    if (nodes[i].obstacle)
    {
      std::cout << "X";
    }
    else
    {
      std::cout << " ";
    }
    if ((i + 1) % map_width == 0)
    {
      std::cout << "\n";
    }
  }
}
