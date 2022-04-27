// Created by Ben on 25/03/2022
// Resource: https://www.youtube.com/watch?v=icZj67PTFhc

#pragma once

#include <deque>
#include <memory>
#include <vector>

#include "../../containers/Vector2.hpp"

namespace bcw
{
  class [[maybe_unused]] PathFinder
  {
   public:
    enum class AllowedDirections
    {
      BOTH,
      STRAIGHT,
      DIAGONAL
    };
    [[maybe_unused]] PathFinder(
      size_t _map_width, size_t _map_height,
      AllowedDirections allow_diagonal_movement = AllowedDirections::BOTH);
    ~PathFinder() = default;

    struct Node
    {
      bool obstacle = false;
      bool visited  = false;
      float global_goal;
      float local_goal;
      size_t x;
      size_t y;
      std::vector<Node*> neighbours;
      PathFinder::Node* parent;
    };

    [[maybe_unused]] [[nodiscard]] std::deque<bcw::Vector2>
    findPath(size_t start_x, size_t start_y, size_t end_x, size_t end_y);
    void setBlockingProperty(int x, int y, bool value);
    void printNodeTree();

   private:
    void connectNeighbours(Node* node, size_t x, size_t y);
    Node* findNode(size_t x, size_t y);
    std::deque<bcw::Vector2> backtrack(Node* end_node);

    std::unique_ptr<Node[]> nodes; // NOLINT c~~~
    size_t map_width;
    size_t map_height;
  };
} // namespace bcw

// ngl i don't know if using unsigned will be a problem again, but yeah if npcs are pathing
// to -1 infinity then that may be the problem!
