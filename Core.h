#pragma once
#include "QuadTree.h"

/// <summary>
/// 根据当前坐标寻找可连接的信号最好的站点(不考虑伪基站)
/// </summary>
/// <param name="tree">存储所有站点的四叉树结构</param>
/// <param name="position">该点的位置坐标</param>
/// <returns>该站点, 如果没有可连接的站点，则返回站点的索引值为-1</returns>
Station FindBestStation(QuadTree& tree, Point2& position);
