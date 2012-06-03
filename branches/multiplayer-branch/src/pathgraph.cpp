#include <algorithm>
#include <cmath>
#include <iostream>

#include "pathgraph.h"

PathNode::PathNode(int x, int y)
  : x_pos(x), y_pos(y), dij_children(NULL), dij_prev(NULL) {
}

bool PathNode::operator==(const PathNode& right) const {
  return(x_pos == right.x_pos && y_pos == right.y_pos);
}

PathGraph::PathGraph() : width(0), height(0) {
  std::cerr << "PathGraph constructor()" << std::endl;
}

PathGraph::PathGraph(const Level& level)
  : width(level.getLength()), height(level.getHeight()) {
  graph.reserve(width * height);
 
  for(int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      if(level.getTile(x, y) == 0) {
	graph.push_back(PathNode(x, y));
      }
    }
  }

  std::vector<PathNode>::iterator iter = graph.begin();
  while(iter != graph.end()) {
    linkChildren(&*iter);
    ++iter;
  }
}

void PathGraph::linkChildren(PathNode* node) {
  for(int j = -1; j < 2; j++) {
    for(int i = -1; i < 2; i++) {
      if(i==0 && j==0) continue; 
      if(i!=0 && j!=0) {
	if(std::find(graph.begin(), graph.end(), PathNode(node->x_pos + 0, node->y_pos + j))==graph.end() ||
	   std::find(graph.begin(), graph.end(), PathNode(node->x_pos + i, node->y_pos + 0))==graph.end()) {
	  continue;
	}
      }
      std::vector<PathNode>::iterator child;
      child = std::find(graph.begin(), graph.end(), PathNode(node->x_pos + i, node->y_pos + j));
      if(*child == *node) continue;
      if(child != graph.end()) {
	PathNode* new_child = &*child;
	node->children.push_back(new_child);
      }
    }
  }
  node->dij_num_children = node->children.size();
  node->dij_children = &(node->children[0]);
}

