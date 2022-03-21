#include <iostream>
#include <vector>
#include <string>

using namespace std;

static class Broadcast {
public:
	static const char* getChannel(int _channel) {
		switch (_channel) {
		case 1:
			return "���ѵ���";
		case 2:
			return "���׸�";
		case 3:
			return "���̾��";
		case 4:
			return "����";
		default:
			return "������";
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
	TV(const char* _brand = "�Ｚ", int _channel = 1, int _volume = 0, int _price = 1500000) : brand(_brand), channel(_channel), volume(_volume), price(_price) { }

	void display() {
		cout << "������������������������������������������������������������������������������" << endl;
		cout << "���� ä�� : " << channel << endl;
		printf("%s��(��) ������ �ִ�. (���� : %2d)\n", Broadcast::getChannel(channel), volume);
		cout << "������������������������������������������������������������������������������" << endl;
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
			cout << i << "���� ������ �Է��Ͻ� �� �����ϴ�." << endl;
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
		cout << "���� TV" << endl;
		cout << "������ : " << brand << endl;
		cout << "���� : " << price << "��" << endl;
		cout << "--------------------" << endl;
	}

	void info(int i) {
		cout << "--------------------" << endl;
		cout << i << "��° TV" << endl;
		cout << "������ : " << brand << endl;
		cout << "���� : " << price << "��" << endl;
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
		name = "��";
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
		cout << nowPlace->getName() << "�� �����ߴ�." << endl;
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
		const char* brands[] = { "�Ｚ", "LG", "����" };

		name = "����";
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
			if (showroom.size() < 1) throw "TV����";
			if (i < 1 || i >= showroom.size()) throw i;
			int num = i - 1;
			if (player.spendMoney(showroom[num].getPrice())) {
				cout << "TV�� ����մϴ�." << endl;
				cout << "�����.........." << endl;
				player.getHome().SetTV(showroom[num]);
				player.getHome().setIsTVHere(true);
				cout << "��� �Ϸ�!" << endl;
				showroom.erase(showroom.begin() + num);
				if (store.size() > 0) {
					showroom.push_back(store.at(store.size() - 1));
					store.pop_back();
				}
			}
			else {
				cout << "�ܾ��� �����մϴ�." << endl;
			}
		}
		catch (const char* string) {
			cout << string << "!!" << endl;
			cout << "��� TV�� �ǸŵǾ����ϴ�." << endl;
		}
		catch (int i) {
			cout << i << "��° TV�� �����ϴ�." << endl;
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

	cout << "===================TV����/����ϱ� ����=====================" << endl;
	cout << endl;


	while (!isEnd) {
		cout << "���� ��ġ : " << player.getNowPlace() << endl;
		cout << "--------------------------------------" << endl;
		while (player.getNowPlace() == "����") {
			cout << "���� ������ : " << player.getMoney() << endl;
			cout << "1. ������ ���� 2. TV���� 3. ������ �̵�" << endl;
			cout << "�Է�>>";
			cin >> input;

			switch (input) {
			case 1:
				market.showroomInfo();
				break;
			case 2:
				cout << "�� ��° TV�� �����Ͻðڽ��ϱ�?" << endl;
				cout << "�Է�>>";
				cin >> input;
				market.sellTV(player, input);
				break;
			case 3:
				player.go(home);
				break;
			default:
				cout << "�߸��� �Է��Դϴ�." << endl;
				break;
			}
		}
		while (!isEnd && player.getNowPlace() == "��") {
			cout << "���� TV��?" << endl;
			if (home.getIsTVHere()) {
				home.getTV().info();

				cout << "1. ä�� �Է� 2. ä�� �ø��� 3. ä�� ������" << endl;
				cout << "4. ���� �ø��� 5. ���� ���߱� 6. ���� ä�� ����" << endl;
				cout << "7. ���α׷� ����" << endl;
				cout << "�Է�>>";
				cin >> input;

				switch (input) {
				case 1:
					cout << "ä���� �Է��ϼ���>>";
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
					cout << "�߸��� �Է��Դϴ�." << endl;
					break;
				}
			}
			else {
				cout << "TV�� ����..." << endl;

				cout << "1. ���Է� �̵� 2. ���α׷� ����" << endl;
				cout << "�Է�>>";
				cin >> input;

				switch (input) {
				case 1:
					player.go(market);
					break;
				case 2:
					isEnd = true;
					break;
				default:
					cout << "�߸��� �Է��Դϴ�." << endl;
					break;
				}
			}
		}
	}
}

void main() {
	TVTest();

	cout << "=======================================" << endl;
	cout << "���α׷��� �����մϴ�." << endl;
	cout << "=======================================" << endl;
}

