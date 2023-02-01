#pragma once
#include <string>
#include <cmath>
using namespace std;
/*
* ����һЩ�������ʵĽṹ�ͺ���
*/
/// <summary>
/// һ���򵥵Ķ�ά����ṹ
/// </summary>
class Point2 {
public:
	double x;
	double y;
	Point2() {
		x = 0;
		y = 0;
	}
	Point2(double _x, double _y) : x(_x), y(_y) {}
	double Dis(Point2 other) {
		return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
	}
};

/// <summary>
/// ��վ
/// </summary>
struct Station {
	// ����
	Point2 coordinate;
	// ����/����/����
	string stationType;
	// �źŹ���ǿ��
	double signalStrength;
	// ���
	unsigned index;
};

/// <summary>
/// �жϵ�point�Ƿ���leftBottom��rightTop��ɵ�������
/// </summary>
/// <param name="point">��Ҫ�жϵĵ������</param>
/// <param name="leftBottom">�������½�����</param>
/// <param name="rightTop">�������Ͻ�����</param>
bool IsInRegion(Point2 point, Point2 leftBottom, Point2 rightTop, int index) {
	if (leftBottom.x > rightTop.x || leftBottom.y > rightTop.y) throw "�����������";

	//Ϊ���Ǳ߽������pointǡ����������ı߽��ߴ�������Ҫ�������������
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
/// �ж��������������Ƿ��ص�
/// </summary>
/// <param name="leftBottom1"></param>
/// <param name="rightTop1"></param>
/// <param name="leftBottom2"></param>
/// <param name="rightTop2"></param>
/// <returns></returns>
bool IsOverlapped(Point2 leftBottom1, Point2 rightTop1, Point2 leftBottom2, Point2 rightTop2) {
	return !(
		leftBottom2.x > rightTop1.x ||
		rightTop2.x < leftBottom1.x ||
		leftBottom2.y > rightTop1.y ||
		rightTop2.y < leftBottom1.y
		);
}
