#include "Core.h"
#include <cmath>
#include <algorithm>
#include <iostream>
/// <summary>
/// �����ź�ǿ��
/// </summary>
/// <param name="radius">��Ч�뾶</param>
/// <param name="distance">ʵ�ʾ���</param>
/// <returns>������</returns>
double CalcSignal(double radius, double distance) {
	if (abs(distance - 0) < 0.001) distance = 0.001;
	return radius * radius / distance / distance;
}

//��׼ǿ��Ϊ1ʱ�ĸ������վ����Ч�뾶
double BaseRadius[4] = { 300.0, 1000.0, 5000.0, 40.0 };
string stationType[4] = { "����", "����", "����", "α��վ" };

/// <summary>
/// ���ݵ�ǰ����Ѱ�ҿ����ӵ��ź���õ�վ��(������α��վ)
/// </summary>
/// <param name="tree">�洢����վ����Ĳ����ṹ</param>
/// <param name="position">�õ��λ������</param>
/// <returns>��վ��, ���û�п����ӵ�վ�㣬�򷵻�վ�������ֵΪ-1</returns>
Station FindBestStation(QuadTree& tree, Point2& position) {
	Station ret;
	//�Ƿ��ҵ������ӵ�վ��
	bool find = false;
	//��������������վ�������źű���
	double maxSignal = 0;
	//�����뾶
	double maxRadius = *max_element(BaseRadius, BaseRadius + 4) * sqrt(tree.maxBaseSignalStrength);

	//��������
	Point2 leftBottom(position.x - maxRadius, position.y - maxRadius);
	Point2 rightTop(position.x + maxRadius, position.y + maxRadius);

	vector<Station> stations(tree.Find(leftBottom, rightTop));

	for (Station station : stations) {
		for (int i = 0; i < 3; ++i) {
			//����Чͨ�ŷ�Χ�ڣ����ź�Ϊ��ǿ
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
