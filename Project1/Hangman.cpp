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

	cout << "글자 맞추기 게임입니다." << endl;
	cout << matched << endl;

	while (true) {
		string s;
		cin >> s;
		
		if (s.size() > 1) {
			cout << "한 글자만 입력해주세요." << endl;
			continue;
		}

		if (regex_match(s, re)) {
			int i = answer.find(s);
			if (i < 0) {
				cout << "없는 글자입니다." << endl;
				continue;
			}
			else {
				matched.replace(i, 1, s);

				cout << matched << endl;
			}
		}
		else {
			cout << "알파벳만 입력해주세요." << endl;
			continue;
		}

		if (answer == matched) {
			cout << "승리했습니다!" << endl;
		}
	}
}

void main() {
	hangmanTest();
}