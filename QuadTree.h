#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_
#include <vector>
#include <string>
using namespace std;

/// <summary>
/// 一个简单的二维坐标结构
/// </summary>
struct Point2 {
public:
	double x;
	double y;
};

/// <summary>
/// 基站
/// </summary>
struct Station {
	// 坐标
	Point2 coordinate;
	// 城区/乡镇/高速
	string type;
	// 信号功率强度
	double signalStrength;
	// 编号
	unsigned index;
};

/// <summary>
/// 存储基站数据的数据结构：四叉树
/// </summary>
class QuadTree {
private:
	// 叶节点存储站点数的最大值，当大于这个值时就进行分裂
	const unsigned maxStationsOfLeaf = 10;

	struct TreeNode {
		// 左下边界点和右上边界点用来表示该节点表示的矩形区域
		Point2 leftBottomBorder = {0, 0};
		Point2 rightTopBorder = {0, 0};

		// 是否为叶节点
		bool isLeaf = false;
		// 不是叶节点则必有四个子节点
		TreeNode* subNodes[4];
		// 是叶节点则存储站点
		vector<Station> stations;

		// 当前节点的深度，从0开始
		unsigned depth = 0;

		TreeNode(unsigned _depth):depth(_depth){}
		TreeNode() {}
	};

	// 根节点
	TreeNode rootNode;

	/// <summary>
	/// 构造函数的辅助函数
	/// </summary>
	void ConstructHelper(vector<Station>& stations, TreeNode& node, unsigned depth);

	/// <summary>
	/// 遍历某个结点
	/// </summary>
	vector<Station> TraverseNode(TreeNode node);

public: 
	// 根据文件来构建四叉树
	QuadTree(string fileName);

	/// <summary>
	/// 遍历树的某个方向, 其中index为0,1,2,3,分别表示左下角（西南）、右下角（东南）、右上角（东北）、左上角（西北）
	/// </summary>
	vector<Station> TraverseOneDirection(int index);

	QuadTree() {
		rootNode.isLeaf = true;
	};

	//根据坐标范围寻找站点
	vector<Station> Find(Point2 leftBottom, Point2 rightTop);
};

#endif