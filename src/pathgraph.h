#ifndef PATHGRAPH_H_
#define PATHGRAPH_H_

#include <vector>

#include "level.h"

struct PathNode {
  int x_pos, y_pos;
  
  std::vector<PathNode*> children;

  /* Required by dijkstra implementation */
  unsigned int dij_distance;
  unsigned int dij_num_children;
  PathNode** dij_children;
  PathNode* dij_prev;

  PathNode(int, int);
  bool operator==(const PathNode& right) const;
};


class PathGraph {
  int width;
  int height;
  std::vector<PathNode> graph;
 public:
  PathGraph();
  PathGraph(const Level&);
  void linkChildren(PathNode*);
  const PathNode* getGraph() const { return &graph[0]; }
  PathNode* getGraph() { return &graph[0]; }
  int getGraphSize() const { return graph.size(); };
};

#endif
