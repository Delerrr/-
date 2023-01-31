#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_
#include <vector>
#include <string>
using namespace std;

/// <summary>
/// һ���򵥵Ķ�ά����ṹ
/// </summary>
struct Point2 {
public:
	double x;
	double y;
};

/// <summary>
/// ��վ
/// </summary>
struct Station {
	// ����
	Point2 coordinate;
	// ����/����/����
	string type;
	// �źŹ���ǿ��
	double signalStrength;
	// ���
	unsigned index;
};

/// <summary>
/// �洢��վ���ݵ����ݽṹ���Ĳ���
/// </summary>
class QuadTree {
private:
	// Ҷ�ڵ�洢վ���������ֵ�����������ֵʱ�ͽ��з���
	const unsigned maxStationsOfLeaf = 10;

	struct TreeNode {
		// ���±߽������ϱ߽��������ʾ�ýڵ��ʾ�ľ�������
		Point2 leftBottomBorder = {0, 0};
		Point2 rightTopBorder = {0, 0};

		// �Ƿ�ΪҶ�ڵ�
		bool isLeaf = false;
		// ����Ҷ�ڵ�������ĸ��ӽڵ�
		TreeNode* subNodes[4];
		// ��Ҷ�ڵ���洢վ��
		vector<Station> stations;

		// ��ǰ�ڵ����ȣ���0��ʼ
		unsigned depth = 0;

		TreeNode(unsigned _depth):depth(_depth){}
		TreeNode() {}
	};

	// ���ڵ�
	TreeNode rootNode;

	/// <summary>
	/// ���캯���ĸ�������
	/// </summary>
	void ConstructHelper(vector<Station>& stations, TreeNode& node, unsigned depth);

public: 
	// �����ļ��������Ĳ���
	QuadTree(string fileName);

	QuadTree() {
		rootNode.isLeaf = true;
	};

	//�������귶ΧѰ��վ��
	vector<Station> Find(Point2 leftBottom, Point2 rightTop);
};

#endif