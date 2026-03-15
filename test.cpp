#include <bits/stdc++.h>
using namespace std;

using ll = long long;
#define endl "\n"
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());


// random tu l toi r
ll random(ll l, ll r) {
    return uniform_int_distribution<ll>(l, r)(rng);
}

// random xau
string randomString (ll size) {
    string fig = "qwertyuiopasdfghhjlzxcvb nmQWERTYUIOPASDFGHJKLZXCMNVB ";
    string randomString = "";
    for (int i = 0 ; i < size; i++) {
        randomString += fig[random(0, fig.size() - 1)];
    }
    return randomString;
}

// Cai dat
const ll TEST = 20;
const string fileCpp = "code";

signed main() {
    string _compileCommands = "g++ " + fileCpp + ".cpp -o code";
    system(_compileCommands.c_str());

    for (int tc = 1; tc <= TEST; tc++) {
        string inp = "TEST/test" + to_string(tc) + ".INP";
        string out = "TEST/test" + to_string(tc) + ".OUT";

        ofstream fi(inp);
        ll a = random(1, 100);
        ll b = random(1, 100);

        fi << a << " " << b << endl;


        fi.close();
        string _IOCommands = fileCpp + ".exe < " + inp + " > " + out;
        system(_IOCommands.c_str());
        cout << "Generated test " << tc << ": " + _IOCommands << endl;
    }

    return 0;
}
