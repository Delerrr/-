#pragma once
#include "QuadTree.h"

/// <summary>
/// ���ݵ�ǰ����Ѱ�ҿ����ӵ��ź���õ�վ��(������α��վ)
/// </summary>
/// <param name="tree">�洢����վ����Ĳ����ṹ</param>
/// <param name="position">�õ��λ������</param>
/// <returns>��վ��, ���û�п����ӵ�վ�㣬�򷵻�վ�������ֵΪ-1</returns>
Station FindBestStation(QuadTree& tree, Point2& position);
