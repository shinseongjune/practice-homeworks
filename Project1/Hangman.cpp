#include <iostream>
#include <string>
#include <regex>
#include <vector>

using namespace std;

void hangmanTest() {
	
	char alphabets[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	srand(time(NULL));

	regex re("[a-zA-Z]");
	vector<int> v;

	string answer;
	string matched = "_____";

	while (answer.size() < 5) {
		int r = rand() % 52;
		if (find(v.begin(), v.end(), r) != v.end()) {
			continue;
		}
		v.push_back(r);
		answer += alphabets[r];
	}

	cout << "���� ���߱� �����Դϴ�." << endl;
	cout << matched << endl;

	while (true) {
		string s;
		cin >> s;
		
		if (s.size() > 1) {
			cout << "�� ���ڸ� �Է����ּ���." << endl;
			continue;
		}

		if (regex_match(s, re)) {
			int i = answer.find(s);
			if (i < 0) {
				cout << "���� �����Դϴ�." << endl;
				continue;
			}
			else {
				matched.replace(i, 1, s);

				cout << matched << endl;
			}
		}
		else {
			cout << "���ĺ��� �Է����ּ���." << endl;
			continue;
		}

		if (answer == matched) {
			cout << "�¸��߽��ϴ�!" << endl;
		}
	}
}

void main() {
	hangmanTest();
}