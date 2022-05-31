#include <bits/stdc++.h>

using namespace std;
using ld = long double;
using ll = long long;


mt19937 rnd(293);
int B = 80;
int C = 100;
int DEPTH = 3;

class Player{
public:
    int id;
    ld favourP;
    int score = 0;
    map<int, ld> income;  // нам оказывают услуги
    map<int, ld> outcome; // мы оказываем услуги
    map<int, int> rejected; // список тех кто нас отверг    
    Player(){};
    bool possible() const {
        std::uniform_real_distribution<> dis(0, 1);
        auto res = dis(rnd);
        return res <= favourP;
    }
    
    vector<int> get_av(vector<int> path, int n) const {
        set<int> av;
        for (int i = 0; i < n; i += 1) av.insert(i);
        for (auto x : path) av.erase(x);
        vector<int> ids;
        for (auto x : av) ids.push_back(x);
        return ids;
    }
    
    virtual int choose_executor(vector<int> path, int n) const = 0;
    virtual int deside(int req_id, vector<int> path, int n) const = 0; // -1 => naher; -2 => help; >= 0 => id того кому*/

};

class StupidPlayer: public Player {
public:
    StupidPlayer(int id_, ld favourP_) {
        id = id_;
        favourP = favourP_;
    }
    virtual int choose_executor(vector<int> path, int n) const {  // просто рандомный чел из доступных вне пути
        auto ids = get_av(path, n);
        int sz = ids.size();
        if (sz == 0) return -1;
        int id_res = rnd() % sz;
        return ids[id_res];
    }
    virtual int deside(int req_id, vector<int> path, int n) const {
        int poss = possible(); //проверяем можем ли мы выполнить услугу
        if (poss) {
            return -2;
        }
        int exec = choose_executor(path, n); //выбираем исполнителя
        if (exec != -1) { // если выбрали исполнителя
            return exec;
        }
        return -1; //шлем нахер
    }
};


class BalancedPlayer: public Player {
public:
    BalancedPlayer(int id_, ld favourP_) {
        id = id_;
        favourP = favourP_;
    }
    virtual int choose_executor(vector<int> path, int n) const {  // просто рандомный чел из доступных вне пути
        auto ids = get_av(path, n);
        int sz = ids.size();
        if (sz == 0) return -1;
        int id_res = rnd() % sz;
        return ids[id_res];
    }
    virtual int deside(int req_id, vector<int> path, int n) const {
        int poss = possible(); //проверяем можем ли мы выполнить услугу
        if (score < 0) //если мы уже потратились, шлем нахер
            return -1;

        if (poss) {
            return -2;
        } 
        int exec = choose_executor(path, n); //выбираем исполнителя
        if (exec != -1) { // если выбрали исполнителя
            return exec;
        }
        return -1; //шлем нахер
    }
};

class CreditPlayer: public Player { //может немножко уходить в минус
public:
    int max_credit;
    CreditPlayer(int id_, ld favourP_, int max_credit_ = 2) {
        id = id_;
        favourP = favourP_;
        max_credit = max_credit_;
    }
    virtual int choose_executor(vector<int> path, int n) const {  // просто рандомный чел из доступных вне пути
        auto ids = get_av(path, n);
        int sz = ids.size();
        if (sz == 0) return -1;
        int id_res = rnd() % sz;
        return ids[id_res];
    }
    virtual int deside(int req_id, vector<int> path, int n) const {
        int poss = possible(); //проверяем можем ли мы выполнить услугу
        if (score < -max_credit * C) //если мы уже потратились, шлем нахер
            return -1;

        if (poss) {
            return -2;
        } 
        int exec = choose_executor(path, n); //выбираем исполнителя
        if (exec != -1) { // если выбрали исполнителя
            return exec;
        }
        return -1; //шлем нахер
    }
};

class TouchyPlayer: public Player {
public:
    TouchyPlayer(int id_, ld favourP_) {
        id = id_;
        favourP = favourP_;
    }
    virtual int choose_executor(vector<int> path, int n) const {  // просто рандомный чел из доступных вне пути
        auto ids = get_av(path, n);
        int sz = ids.size();
        if (sz == 0) return -1;
        int id_res = rnd() % sz;
        return ids[id_res];
    }
    virtual int deside(int req_id, vector<int> path, int n) const {
        int poss = possible(); //проверяем можем ли мы выполнить услугу

        if (rejected.count(req_id)) { // если нас этот человек отвергал, шлем нахер
            return -1;
        }

        if (poss) {
            return -2;
        } 
        int exec = choose_executor(path, n); //выбираем исполнителя
        if (exec != -1) { // если выбрали исполнителя
            return exec;
        }
        return -1; //шлем нахер
    }
};
class VindictivePlayer: public Player {
public:
    int limit = 3;
    VindictivePlayer(int id_, ld favourP_) {
        id = id_;
        favourP = favourP_;
    }
    virtual int choose_executor(vector<int> path, int n) const {  // просто рандомный чел из доступных вне пути
        auto ids = get_av(path, n);
        int sz = ids.size();
        if (sz == 0) return -1;
        int id_res = rnd() % sz;
        return ids[id_res];
    }
    virtual int deside(int req_id, vector<int> path, int n) const {
        int poss = possible(); //проверяем можем ли мы выполнить услугу
        int rq = 0;
        auto it = rejected.find(req_id);
        if (it != rejected.end())
            rq = it->second;

        if (rq >= 1) { // если нас этот человек отвергал, шлем нахер
            return -1;
        }

        if (poss) {
            return -2;
        } 
        int exec = choose_executor(path, n); //выбираем исполнителя
        if (exec != -1) { // если выбрали исполнителя
            return exec;
        }
        return -1; //шлем нахер
    }
};

class AdvancedCreditPlayer: public Player {
public:
    int max_credit;
    AdvancedCreditPlayer(int id_, ld favourP_, int max_credit_ = 2) {
        id = id_;
        favourP = favourP_;
        max_credit = max_credit_;
    }
    virtual int choose_executor(vector<int> path, int n) const {  // просто рандомный чел из доступных вне пути
        auto ids = get_av(path, n);
        int sz = ids.size();
        if (sz == 0) return -1;
        
        int ans = ids[0];
        vector<int> mx_ids = {ans};
        for (auto cand: ids) {
            int L = 0;
            if (income.count(cand)) L += income.at(cand);
            if (outcome.count(cand)) L -= outcome.at(cand);
            int R = 0; 
            if (income.count(ans)) R += income.at(ans);
            if (outcome.count(ans)) R -= outcome.at(ans);
            if (L > R) {
                ans = cand;
                mx_ids = {ans};
            } else if (L == R) {
                mx_ids.push_back(cand);
            }
        }
        int sz2 = mx_ids.size();
        return mx_ids[rnd() % sz2];
    }
     virtual int deside(int req_id, vector<int> path, int n) const {
        int poss = possible(); //проверяем можем ли мы выполнить услугу
        if (score < -max_credit * C) //если мы уже потратились, шлем нахер
            return -1;

        if (poss) {
            return -2;
        } 
        int exec = choose_executor(path, n); //выбираем исполнителя
        if (exec != -1) { // если выбрали исполнителя
            return exec;
        }
        return -1; //шлем нахер
    }
};


class Arena{
public:
    int n;
    int total_s = 0;
    vector<int> score_timeline = {0};
    vector<Player*> players;
    int days;
    Arena(vector<Player*> players_, int days_) {
        players = players_;
        n = players.size();
        days = days_;
    }
    int generate_id() {
        return rnd() % n;
    }
    void run() {
        for (int day = 0; day < days; day += 1) {
            int customer_id = generate_id();
            vector<int> path = {customer_id};
            int success = 0;
            while (true) {
                int req_id = path.back();
                int executor_id = players[customer_id]->choose_executor(path, n);
                if (executor_id == -1) {
                    break;
                }
                path.push_back(executor_id);
                int desicion = players[executor_id]->deside(req_id, path, n);
                if (desicion == -1) { // послал нахер
                    success = 0;
                    break;
                }
                if (desicion == -2) { //исполнил услугу
                    success = 1;
                    break;
                }
            }
            if (success == 0) {
                score_timeline.push_back(total_s);
                int sz = path.size();
                for (int i = 0; i + 1 < sz; i += 1) {
                    int id_req = path[i];
                    int id_exec = path[i + 1];
                    players[id_req]->rejected[id_exec] = 1;
                }
 
                continue;
            }
            int sz = path.size();
            for (int i = 0; i + 1 < sz; i += 1) {
                int id_req = path[i];
                int id_exec = path[i + 1];
                players[id_req]->income[id_exec] += 1;
                players[id_exec]->outcome[id_exec] += 1;

                players[id_req]->score += (i == 0 ? C : B); // добавляем профит за услугу
                total_s += (i == 0 ? C : B);
                players[id_exec]->score -= B; //отжимаем усилия на услугу
                total_s -= B;
            }
            score_timeline.push_back(total_s);
        }
    }

    int total_score() {
        int res = 0;
        for (int i = 0; i < n; i += 1) {
            res += players[i]->score;
        }
        return res;
    }
};

vector<Player*> generate_stupid(int N = 100) {
    vector<Player*> res;
    for (int i = 0; i < N; i += 1) {
        res.push_back(new StupidPlayer(i, 0.05));
    }
    return res;
}

vector<Player*> generate_balanced(int N = 100) {
    vector<Player*> res;
    for (int i = 0; i < N; i += 1) {
        res.push_back(new BalancedPlayer(i, 0.05));
    }
    return res;
}

vector<Player*> generate_credit(int N = 100) {
    vector<Player*> res;
    for (int i = 0; i < N; i += 1) {
        res.push_back(new CreditPlayer(i, 0.05, 2));
    }
    return res;
}

vector<Player*> gen(string type, int sz, int zero) {
    vector<Player*> res;
    for (int i = 0; i < sz; i += 1) {
        int id = i + zero;

        if (type == "stupid") {
            res.push_back(new StupidPlayer(id, 0.05));
        }
        if (type == "credit") {
            res.push_back(new CreditPlayer(id, 0.05, 2));
        }
        if (type == "balanced") {
            res.push_back(new BalancedPlayer(id, 0.05));
        }
        if (type == "touchy") {
            res.push_back(new TouchyPlayer(id, 0.05));
        }
        if (type == "vindictive") {
            res.push_back(new VindictivePlayer(id, 0.05));
        }
        if (type == "advanced_credit") {
            res.push_back(new AdvancedCreditPlayer(id, 0.05, 2));
        }
    }
    return res;
}

int main(int argc, char* argv[]) {
    unsigned sm = 0;
    for (int i = 0; i < argc; i += 1) {
        string s(argv[i]);
        for (auto x : s)
          sm += x;
    }
    string fname = "file_" + to_string(sm + rnd()) + ".txt";
    ofstream fout(fname);

    vector<int> BS = {10, 20, 40, 50, 70, 85, 90, 95, 99};
    for (auto BB : BS) {
        B = BB;
        vector<Player*> players = {};
        int iters = atoi(argv[1]);
        for (int i = 2; i < argc; i += 2) {
            string s(argv[i]);
            int sz = atoi(argv[i + 1]);
            auto generated = gen(s, sz, players.size());
            for (auto x : generated) {
                players.push_back(x);
            }
        }
        Arena arena(players, iters);
        arena.run();
        auto score = arena.total_s;
        auto mx = (C - B) * arena.days;
        cout << "B = " << B << " " << score << " from " << mx << "(" << score * 100.0 / mx << "%)" << endl;
        fout << iters << " " << B << " " << C << " " << arena.n << endl;
        for (auto x : arena.score_timeline)
            fout << x << " ";
        fout << endl;
    }
    fout << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
    
    cerr << 1.0 * clock() / CLOCKS_PER_SEC << endl;    
    cout << "Written to " << fname << endl;
}


