#include <iostream>
#include <vector>
#include <string>

using namespace std;

static class Broadcast {
public:
	static const char* getChannel(int _channel) {
		switch (_channel) {
		case 1:
			return "무한도전";
		case 2:
			return "런닝맨";
		case 3:
			return "아이언맨";
		case 4:
			return "뉴스";
		default:
			return "노이즈";
		}
	}
};

class TV {
private:
	const char* brand;
	int channel;
	int volume;
	int price;

	void clampChannel() {
		if (channel > 4) channel = 1;
		else if (channel < 1) channel = 4;
	}

	void clampVolume() {
		if (volume > 10) volume = 10;
		else if (volume < 0) volume = 0;
	}
public:
	TV(const char* _brand = "삼성", int _channel = 1, int _volume = 0, int _price = 1500000) : brand(_brand), channel(_channel), volume(_volume), price(_price) { }

	void display() {
		cout << "───────────────────────────────────────" << endl;
		cout << "현재 채널 : " << channel << endl;
		printf("%s이(가) 나오고 있다. (볼륨 : %2d)\n", Broadcast::getChannel(channel), volume);
		cout << "───────────────────────────────────────" << endl;
	}

	int getChannel() {
		return channel;
	}

	void setChannel(int i) {
		try {
			if (i < 0) throw i;
			channel = i;
		}
		catch (int i) {
			cout << i << "같은 음수는 입력하실 수 없습니다." << endl;
		}
		display();
	}

	void plusChannel() {
		channel++;
		clampChannel();
		display();
	}

	void minusChannel() {
		channel--;
		clampChannel();
		display();
	}

	void plusVolume() {
		volume++;
		clampVolume();
		display();
	}

	void minusVolume() {
		volume--;
		clampVolume();
		display();
	}

	int getPrice() {
		return price;
	}

	void info() {
		cout << "--------------------" << endl;
		cout << "나의 TV" << endl;
		cout << "제조사 : " << brand << endl;
		cout << "가격 : " << price << "원" << endl;
		cout << "--------------------" << endl;
	}

	void info(int i) {
		cout << "--------------------" << endl;
		cout << i << "번째 TV" << endl;
		cout << "제조사 : " << brand << endl;
		cout << "가격 : " << price << "원" << endl;
		cout << "--------------------" << endl;
	}
};

class Place {
protected:
	const char* name;
public:
	const char* getName() {
		return name;
	}
};

class Home : public Place {
private:
	TV* shelf;
	bool isTVHere;
public:
	Home() {
		name = "집";
		shelf = NULL;
		isTVHere = false;
	}

	void SetTV(TV& tv) {
		shelf = &tv;
	}

	TV& getTV() {
		return *shelf;
	}

	bool getIsTVHere() {
		return isTVHere;
	}

	void setIsTVHere(bool b) {
		isTVHere = b;
	}
};

class Player {
private:
	int money;
	Home* home;
	Place* nowPlace;
public:
	Player(int _money, Home& _home, Place& place) : money(_money), home(&_home), nowPlace(&place) { }

	int getMoney() {
		return money;
	}

	bool spendMoney(int _money) {
		if (money < _money) {
			return false;
		}
		money -= _money;
		return true;
	}

	Home& getHome() {
		return *home;
	}

	void go(Place& place) {
		nowPlace = &place;
		cout << nowPlace->getName() << "에 도착했다." << endl;
		cout << "=====================================" << endl;
		cout << "=====================================" << endl;
	}

	const char* getNowPlace() {
		return nowPlace->getName();
	}
};


class Market : public Place {
private:
	vector<TV> store;
	vector<TV> showroom;
public:
	Market() {
		srand(time(NULL));
		const char* brands[] = { "삼성", "LG", "애플" };

		name = "가게";
		for (int i = 0; i < 9; i++) {
			int num = rand() % 3;
			TV tv(brands[num]);
			store.push_back(tv);
		}

		for (int i = 0; i < 3; i++) {
			int num = rand() % 3;
			TV tv(brands[num]);
			showroom.push_back(tv);
		}
	}

	void sellTV(Player& player, int i) {
		try {
			if (showroom.size() < 1) throw "TV없음";
			if (i < 1 || i >= showroom.size()) throw i;
			int num = i - 1;
			if (player.spendMoney(showroom[num].getPrice())) {
				cout << "TV를 배달합니다." << endl;
				cout << "배달중.........." << endl;
				player.getHome().SetTV(showroom[num]);
				player.getHome().setIsTVHere(true);
				cout << "배달 완료!" << endl;
				showroom.erase(showroom.begin() + num);
				if (store.size() > 0) {
					showroom.push_back(store.at(store.size() - 1));
					store.pop_back();
				}
			}
			else {
				cout << "잔액이 부족합니다." << endl;
			}
		}
		catch (const char* string) {
			cout << string << "!!" << endl;
			cout << "모든 TV가 판매되었습니다." << endl;
		}
		catch (int i) {
			cout << i << "번째 TV는 없습니다." << endl;
		}
	}

	void showroomInfo() {
		for (int i = 0; i < showroom.size(); i++) {
			showroom[i].info(i + 1);
		}
	}
};

void TVTest() {
	int input;
	bool isEnd = false;;

	Market market;
	Home home;
	Player player(2000000, home, market);

	cout << "===================TV구매/사용하기 시작=====================" << endl;
	cout << endl;


	while (!isEnd) {
		cout << "현재 위치 : " << player.getNowPlace() << endl;
		cout << "--------------------------------------" << endl;
		while (player.getNowPlace() == "가게") {
			cout << "현재 소지금 : " << player.getMoney() << endl;
			cout << "1. 전시장 보기 2. TV구매 3. 집으로 이동" << endl;
			cout << "입력>>";
			cin >> input;

			switch (input) {
			case 1:
				market.showroomInfo();
				break;
			case 2:
				cout << "몇 번째 TV를 구매하시겠습니까?" << endl;
				cout << "입력>>";
				cin >> input;
				market.sellTV(player, input);
				break;
			case 3:
				player.go(home);
				break;
			default:
				cout << "잘못된 입력입니다." << endl;
				break;
			}
		}
		while (!isEnd && player.getNowPlace() == "집") {
			cout << "나의 TV는?" << endl;
			if (home.getIsTVHere()) {
				home.getTV().info();

				cout << "1. 채널 입력 2. 채널 올리기 3. 채널 내리기" << endl;
				cout << "4. 볼륨 올리기 5. 볼륨 낮추기 6. 현재 채널 보기" << endl;
				cout << "7. 프로그램 종료" << endl;
				cout << "입력>>";
				cin >> input;

				switch (input) {
				case 1:
					cout << "채널을 입력하세요>>";
					cin >> input;
					home.getTV().setChannel(input);
					break;
				case 2:
					home.getTV().plusChannel();
					break;
				case 3:
					home.getTV().minusChannel();
					break;
				case 4:
					home.getTV().plusVolume();
					break;
				case 5:
					home.getTV().minusVolume();
					break;
				case 6:
					home.getTV().display();
					break;
				case 7:
					isEnd = true;
					break;
				default:
					cout << "잘못된 입력입니다." << endl;
					break;
				}
			}
			else {
				cout << "TV가 없다..." << endl;

				cout << "1. 가게로 이동 2. 프로그램 종료" << endl;
				cout << "입력>>";
				cin >> input;

				switch (input) {
				case 1:
					player.go(market);
					break;
				case 2:
					isEnd = true;
					break;
				default:
					cout << "잘못된 입력입니다." << endl;
					break;
				}
			}
		}
	}
}

void main() {
	TVTest();

	cout << "=======================================" << endl;
	cout << "프로그램을 종료합니다." << endl;
	cout << "=======================================" << endl;
}

