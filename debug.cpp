#include "Core.h"
#include <iostream>
#define DEBUG
#ifdef DEBUG
int main() {
	string fileName;
	cout << "input file name:\n";
	cin >> fileName;
	QuadTree t(fileName);
	while (true) {
		Point2 leftBottom, rightTop;
		cin >> leftBottom.x >> leftBottom.y;
		Station res = FindBestStation(t, leftBottom);
	}


}
#endif
