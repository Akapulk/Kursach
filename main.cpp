#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <algorithm>


using namespace std;
int wins, flag;


struct Player {
	string name;
	string country;
	int year;
	int wins;
	Player(const string& country = "", const string& name = "", int year = 0, int wins = 0) :
		country(country), name(name), year(year), wins(wins) {}
	friend istream& operator >> (istream& in, Player& p) {
		in >> p.country >> p.name >> p.year >> p.wins;
		return in;
	}
};


class Observer
{
  public:
    virtual void update(int value, ofstream&) = 0;
	virtual Player& get() = 0;
};
 

class Subject
{
	vector<Observer*> m_views;
	ofstream out;
	int num;
public:
	Subject() { num = 0; } 
	void attach(Observer *obs)
	{
		m_views.push_back(obs);
		num++; 
	}
	void play(int val)
	{
		out.open(to_string(rand()) + to_string(rand()) + ".txt");
		notify();
		for (int i = 0; i < val; ++i) {
		
			vector<int> chances; 
			int speed, luck;
			for (int j = 0; j < num; ++j)
			{
				speed = rand() % 11; 
				luck = rand() % 11;
				chances.push_back(speed*luck); 
			}
			int maxch = 0; 
			int winner = 0;
			for (int j = 0; j < num; ++j)
			{
				if (maxch < chances.at(j)) 
				{
					maxch = chances.at(j);
						winner = j; 
				}
			}
			for (int j = 0; j < num; ++j)
			{
				if (winner == j) 
					(m_views.at(j))->get().wins += 1; 
				else
					(m_views.at(j))->get().wins += 0; 
			}
			out << "Match: " << i + 1 << "\n";
			cout << endl << "Match: " << i + 1 << endl;
			cout << "Pobeshdaet ";
			cout << (m_views.at(winner))->get().name << endl;
			cout << "Igrok\tPobed" << endl;
			notify();
			
		}
	}
	
	void notify()
	{
		if (flag == 0)
			cout << "Sootnoshenie pobed do matcha:" << endl;
			sort(m_views.begin(), m_views.end(), [](Observer* a, Observer* b) {return a->get().wins > b->get().wins; });
			for (int i = 0; i < m_views.size(); ++i) {
				m_views[i]->update(i, out);
				cout << "\n"; out << '\n';
		}
	}
};

class AllObserver : public Observer
{
	Player player;
public:
	AllObserver(Subject *model, const Player& p) :player(p)
	{
		model->attach(this);
	}
	Player& get() { return player; }
	void update(int position, ofstream& out)
	{
		cout << player.name << "\t\t" << player.wins << "\t";
		out << player.name  << "\t\t" << player.wins << "\t";
	}
};

class MainObserver: public Observer
{
	Player player; 
	int last_wins;
  public:
	  MainObserver(Subject *model, const Player& p) : player(p), last_wins(p.wins)
    {
        model->attach(this);
    }
	  Player& get() { return player; }
      void update(int position, ofstream& out)
    {
		 bool is_win = false;
		 if (last_wins != player.wins) {
			 last_wins = player.wins;
			 is_win = true;
			 wins++;
		 }
		 if (flag == 0)
		 {
			 out << player.name << "\t\t" << player.wins;
			 cout << player.name << "\t\t" << player.wins;
			 flag = 1;
		 }
		 else
		 {
			 out << player.name << "\t\t" << player.wins << "\t" << (is_win ? " Win" : " Los");
			 cout << player.name << "\t\t" << player.wins << "\t" << (is_win ? " Win" : " Los");
		 }

    }
};
 
int main()
{
  Subject subj;
  int game=0;
  int cnt;
  int buff; 
  ifstream fin("all.txt"); 
  ifstream in("all.txt");
  fin >> buff;
  in >> cnt;
  vector<Player> players(cnt);
  for (auto& p : players) {
	  in >> p;
  }
  cout << "Dobavit igrakov v bazu? (1/0): ";
  char ch;
  cin >> ch;
  switch (ch)
  {
  case '1':

	  cout << "\nVvedite kol novih igracov: "; cin >> cnt;
	  for (int i = 0; i < cnt; ++i) {
		  string name, country;
		  int year, wins;
		  cout << "Ima: "; cin >> name;
		  cout << "Stran: "; cin >> country;
		  cout << "Pobedi: "; cin >> wins;
		  cout << "God: "; cin >> year;
		  players.push_back(Player{country, name, year, wins});
	  }
	  break;

  case '0':

	  break;
}
  cout << "igroki:" << endl;
  for (int i = 0; i < players.size(); ++i)
  {
	  cout << players.at(i).name << endl;
  }
  cout << "Vvidite № igroka (ot 0 do " << players.size() - 1 << ") : ";
  cin >> cnt;
 
  vector<Observer*> obs;
  for (int i = 0; i < players.size(); ++i) {
	  if (i != cnt) {
		  obs.push_back(new AllObserver(&subj, players[i]));
	  }
	  else {
		  obs.push_back(new MainObserver(&subj, players[i]));
	  }
  }
  wins = 0;
  flag = 0;
  cout << "\nKol match: "; 
  cin >> game;
  subj.play(game);
  if (wins > 1)
	  cout << "You win!" << endl;
  else
	  cout << "You los!" << endl;

  system("pause>>void");
}