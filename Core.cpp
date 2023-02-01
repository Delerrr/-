#include "Core.h"
#include <cmath>
#include <algorithm>
#include <iostream>
/// <summary>
/// 计算信号强度
/// </summary>
/// <param name="radius">有效半径</param>
/// <param name="distance">实际距离</param>
/// <returns>计算结果</returns>
double CalcSignal(double radius, double distance) {
	if (abs(distance - 0) < 0.001) distance = 0.001;
	return radius * radius / distance / distance;
}

//基准强度为1时的各区域基站的有效半径
double BaseRadius[4] = { 300.0, 1000.0, 5000.0, 40.0 };
string stationType[4] = { "城区", "乡镇", "高速", "伪基站" };

/// <summary>
/// 根据当前坐标寻找可连接的信号最好的站点(不考虑伪基站)
/// </summary>
/// <param name="tree">存储所有站点的四叉树结构</param>
/// <param name="position">该点的位置坐标</param>
/// <returns>该站点, 如果没有可连接的站点，则返回站点的索引值为-1</returns>
Station FindBestStation(QuadTree& tree, Point2& position) {
	Station ret;
	//是否找到可连接的站点
	bool find = false;
	//搜索区域中所有站点的最大信号倍率
	double maxSignal = 0;
	//搜索半径
	double maxRadius = *max_element(BaseRadius, BaseRadius + 4) * sqrt(tree.maxBaseSignalStrength);

	//搜索区域
	Point2 leftBottom(position.x - maxRadius, position.y - maxRadius);
	Point2 rightTop(position.x + maxRadius, position.y + maxRadius);

	vector<Station> stations(tree.Find(leftBottom, rightTop));

	for (Station station : stations) {
		for (int i = 0; i < 3; ++i) {
			//在有效通信范围内，且信号为最强
			if (station.stationType == stationType[i] 
				&& station.coordinate.Dis(position) <= BaseRadius[i]
				&& CalcSignal(BaseRadius[i], station.coordinate.Dis(position)) > maxSignal)
			{
				find = true;
				maxSignal = CalcSignal(BaseRadius[i], station.coordinate.Dis(position));
				ret = station;
			}
		}
	}

	if (!find) {
		ret.index = -1;
	}

	cout << "signal: " << maxSignal << endl;
	return ret;
}
