#include "QuadTree.h"
#include <iostream>
#include <fstream>
#include <algorithm>

/// <summary>
///�ж������Ƿ����
/// </summary>
bool EndInput(Point2 inputPoint) {
	const double EPSILON = 0.0001;
	return fabs(inputPoint.x + 1) < EPSILON && 
			fabs(inputPoint.y + 1) < EPSILON;
}

/// <summary>
//�ȽϺ�����
/// </summary>
bool XComp(Station arg1, Station arg2) {
	return arg1.coordinate.x < arg2.coordinate.x;
}

/// <summary>
//�Ƚ�������
/// </summary>
bool YComp(Station arg1, Station arg2) {
	return arg1.coordinate.y < arg2.coordinate.y;
}

QuadTree::QuadTree(string fileName) {
	ifstream ifs(fileName);
	if (!ifs.is_open()) {
		throw "���ļ�" + fileName + "ʧ��";
	}

	char buf[1024];
	//������ͷ���ļ���
	ifs.getline(buf, sizeof(buf));
	vector<Station> stations;
	Station inputStation;
	//������
	char comma;
	
	ifs >> inputStation.coordinate.x
		>> comma
		>> inputStation.coordinate.y;

	while (!EndInput(inputStation.coordinate))
	{
		//������
		ifs >> comma
			>> inputStation.stationType
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
/// ����ĳ�����
/// </summary>
vector<Station> QuadTree::TraverseNode(TreeNode node) {
	if (node.isLeaf) {
		return node.stations;
	}

	vector<Station> ret;
	for (TreeNode* i : node.subNodes) {
		vector<Station> temp = TraverseNode(*i);
		ret.insert(ret.end(), temp.begin(), temp.end());
	}

	return ret;
}


/// <summary>
/// ��������ĳ������
/// </summary>
/// <param name="index">Ϊ0,1,2,3, �ֱ��ʾ���½ǣ����ϣ������½ǣ����ϣ������Ͻǣ������������Ͻǣ�������</param>
/// <returns></returns>
vector<Station> QuadTree::TraverseTreeByOneDirection(int index) {
	return TraverseNodeByOneDirection(rootNode, index);
}

/// <summary>
/// ����ĳ����ĳ������
/// </summary>
/// <param name="node">Ҫ�����Ļ�׼���</param>
/// <param name="index">����Ϊ0,1,2,3, �ֱ��ʾ���½ǣ����ϣ������½ǣ����ϣ������Ͻǣ������������Ͻǣ�������</param>
/// <returns></returns>
vector<Station> QuadTree::TraverseNodeByOneDirection(TreeNode node, int index) {
	if (index > 3 || index < 0) throw "index ������";
	return TraverseNode(*(node.subNodes[index]));
}

/// <summary>
/// ���������ĸ�������
/// </summary>
/// <param name="node"></param>
void QuadTree::DestructHelper(TreeNode node) {
	if (node.isLeaf) {
		return;
	}

	for (TreeNode* subNodePtr : node.subNodes) {
		DestructHelper(*subNodePtr);
		delete subNodePtr;
	}
}

int times = 0;
/// <summary>
/// ���Һ����ĸ�������
/// </summary>
vector<Station> QuadTree::FindHelper(Point2 leftBottom, Point2 rightTop, TreeNode node) {
	times++;
	cout << "���Ҵ�����" << times << endl;
	vector<Station> ret;
	if (node.isLeaf) {
		ret.assign(node.stations.begin(), node.stations.end());
	} else {
		for (TreeNode* subNode : node.subNodes) {
			//����ӽڵ�����ͱ������������ص���������ӽڵ㣬���򲻲���
			if (IsOverlapped(subNode->leftBottomBorder, subNode->rightTopBorder, leftBottom, rightTop)) {
				vector<Station> temp(FindHelper(leftBottom, rightTop, *subNode));
				ret.insert(ret.end(), temp.begin(), temp.end());
			}
		}
	}

	return ret;
}

/// <summary>
/// �����������򣬲��Ҹþ��������ڵ�վ��
/// </summary>
vector<Station> QuadTree::Find(Point2 leftBottom, Point2 rightTop) {
 	return FindHelper(leftBottom, rightTop, rootNode);
}
 
int main() {
	string fileName;
	cout << "input file name:\n";
	cin >> fileName;
	QuadTree t(fileName);
	while (true) {
		Point2 leftBottom, rightTop;
		cin >> leftBottom.x >> leftBottom.y >> rightTop.x >> rightTop.y;
		times = 0;
		vector<Station> res(t.Find(leftBottom, rightTop));
		cout << "���ҽ���, res��С��" << res.size() << endl;
	}
}



