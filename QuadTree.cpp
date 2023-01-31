#include "QuadTree.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#define DEBUG
#ifdef DEBUG
int times = 0;
#endif
/// <summary>
/// 判断点point是否在leftBottom和rightTop组成的区域里
/// </summary>
/// <param name="point">需要判断的点的坐标</param>
/// <param name="leftBottom">区域左下角坐标</param>
/// <param name="rightTop">区域右上角坐标</param>
bool IsInRegion(Point2 point, Point2 leftBottom, Point2 rightTop, int index) {
	if (leftBottom.x > rightTop.x || leftBottom.y > rightTop.y) throw "区域参数错误";

	//为考虑边界情况（point恰好在两区域的边界线处），需要分四种情况讨论
	bool ret = point.x >= leftBottom.x && point.y >= leftBottom.y && point.x < rightTop.x && point.y < rightTop.y;
	switch (index) {
	case 0:
		break;
	case 1:
		ret = point.x >= leftBottom.x && point.y >= leftBottom.y && point.x <= rightTop.x && point.y < rightTop.y;
		break;
	case 2:
		ret = point.x >= leftBottom.x && point.y >= leftBottom.y && point.x <= rightTop.x && point.y <= rightTop.y;
		break;
	case 3:
		ret = point.x >= leftBottom.x && point.y >= leftBottom.y && point.x < rightTop.x&& point.y <= rightTop.y;
		break;
	}
	return ret;
}

/// <summary>
///判断输入是否结束
/// </summary>
bool EndInput(Point2 inputPoint) {
	const double EPSILON = 0.0001;
	return fabs(inputPoint.x + 1) < EPSILON && 
			fabs(inputPoint.y + 1) < EPSILON;
}

/// <summary>
//比较横坐标
/// </summary>
bool XComp(Station arg1, Station arg2) {
	return arg1.coordinate.x < arg2.coordinate.x;
}

/// <summary>
//比较纵坐标
/// </summary>
bool YComp(Station arg1, Station arg2) {
	return arg1.coordinate.y < arg2.coordinate.y;
}

QuadTree::QuadTree(string fileName) {
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		throw "打开文件" + fileName + "失败";
	}

	char buf[1024];
	//读掉开头的文件名
	ifs.getline(buf, sizeof(buf));
	vector<Station> stations;
	Station inputStation;
	//处理逗号
	char comma;
	
	ifs >> inputStation.coordinate.x
		>> comma
		>> inputStation.coordinate.y;

	while (!EndInput(inputStation.coordinate))
	{
		//处理逗号
		ifs >> comma
			>> inputStation.type
			>> inputStation.signalStrength
			>> comma
			>> inputStation.index;

		stations.push_back(inputStation);

		ifs >> inputStation.coordinate.x
			>> comma
			>> inputStation.coordinate.y;
	}
	ifs.close();

	rootNode.leftBottomBorder.x = (*min_element(stations.begin(), stations.end(), XComp)).coordinate.x;
	rootNode.leftBottomBorder.y = (*min_element(stations.begin(), stations.end(), YComp)).coordinate.y;
	rootNode.rightTopBorder.x = (*max_element(stations.begin(), stations.end(), XComp)).coordinate.x;
	rootNode.rightTopBorder.y = (*max_element(stations.begin(), stations.end(), YComp)).coordinate.y;

	ConstructHelper(stations, rootNode, rootNode.depth);
}

void QuadTree::ConstructHelper(vector<Station>& stations, TreeNode& node, unsigned depth) {
	Point2 leftBottomBorder = node.leftBottomBorder;
	Point2 rightTopBorder = node.rightTopBorder;
	if (stations.size() <= maxStationsOfLeaf) {
		node.isLeaf = true;
		node.stations.assign(stations.begin(), stations.end());
		return;
	}

	vector<Station> substations[4];

	const int length = sizeof(node.subNodes) / sizeof(rootNode.subNodes[0]);
	for (int subIndex = 0; subIndex < length; ++subIndex) {
		node.subNodes[subIndex] = new TreeNode(depth + 1);
	}
	node.subNodes[0]->leftBottomBorder = leftBottomBorder;
	node.subNodes[0]->rightTopBorder.x = (leftBottomBorder.x + rightTopBorder.x) / 2;
	node.subNodes[0]->rightTopBorder.y = (leftBottomBorder.y + rightTopBorder.y) / 2;
	node.subNodes[1]->leftBottomBorder.x = node.subNodes[0]->rightTopBorder.x;
	node.subNodes[1]->leftBottomBorder.y = leftBottomBorder.y;
	node.subNodes[1]->rightTopBorder.x = rightTopBorder.x;
	node.subNodes[1]->rightTopBorder.y = node.subNodes[0]->rightTopBorder.y;
	node.subNodes[2]->leftBottomBorder = node.subNodes[0]->rightTopBorder;
	node.subNodes[2]->rightTopBorder = rightTopBorder;
	node.subNodes[3]->leftBottomBorder.x = leftBottomBorder.x;
	node.subNodes[3]->leftBottomBorder.y = node.subNodes[0]->rightTopBorder.y;
	node.subNodes[3]->rightTopBorder.x = node.subNodes[0]->rightTopBorder.x;
	node.subNodes[3]->rightTopBorder.y = rightTopBorder.y;

	for (Station i : stations) {
		for (int j = 0; j < 4; ++j) {
			if (IsInRegion(i.coordinate, node.subNodes[j]->leftBottomBorder, node.subNodes[j]->rightTopBorder, j)) {
				substations[j].push_back(i);
				continue;
			}
		}
	}
	for (int i = 0; i < 4; ++i) {
		ConstructHelper(substations[i], *(node.subNodes[i]), depth + 1);
	}		
}

/// <summary>
/// 遍历某个结点
/// </summary>
vector<Station> QuadTree::TraverseNode(TreeNode node) {
	if (node.isLeaf) {
#ifdef DEBUG
		//for (Station i : node.stations) cout << i.index << endl;
#endif
		return node.stations;
	}

	vector<Station> ret;
	for (TreeNode* i : node.subNodes) {
		vector<Station> temp = TraverseNode(*i);
#ifdef DEBUG
				times++;
			cout << times << endl;
#endif // DEBUG
		ret.insert(ret.end(), temp.begin(), temp.end());
	}

	return ret;
}


/// <summary>
/// 遍历树的某个方向
/// </summary>
/// <param name="index">为0,1,2,3, 分别表示左下角（西南）、右下角（东南）、右上角（东北）、左上角（西北）</param>
/// <returns></returns>
vector<Station> QuadTree::TraverseOneDirection(int index) {
	if (index > 3 || index < 0) throw "index 不合理";
	return TraverseNode(*(rootNode.subNodes[index]));
}

int main() {
	cout << "输入文件名\n";
	string filename;
	cin >> filename;
	QuadTree t(filename);
	vector<Station> res(t.TraverseOneDirection(0));
	cout << "ok";
}