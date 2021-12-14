/*----------------------- code table----------------------------
	space   ->  10
	a       ->  11
	b       ->  12
	c       ->  13
	d       ->  14
	e       ->  15
	f       ->  16
	g       ->  17
	h       ->  18
	i       ->  19
	j       ->  20
	k       ->  21
	l       ->  22
	m       ->  23
	n       ->  24
	o       ->  25
	p       ->  26
	q       ->  27
	r       ->  28
	s       ->  29
	t       ->  30
	u       ->  31
	v       ->  32
	w       ->  33
	x       ->  34
	y       ->  35
	z       ->  36
	A       ->  37
	B       ->  38
	C       ->  39
	D       ->  40
	E       ->  41
	F       ->  42
	G       ->  43
	H       ->  44
	I       ->  45
	J       ->  46
	K       ->  47
	L       ->  48
	M       ->  49
	N       ->  50
	O       ->  51
	P       ->  52
	Q       ->  53
	R       ->  54
	S       ->  55
	T       ->  56
	U       ->  57
	V       ->  58
	W       ->  59
	X       ->  60
	Y       ->  61
	Z       ->  62
	\n      ->  63
*///---------------------------------------------------

#include <thread>
#include <fstream>
#include <mutex>
#include <iostream>
#include <queue>
#include <string>

using namespace std;

thread *thrs;
string *out;
queue<string> tasks;
const static string alphabet = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\n";  //Наш алфавит
mutex mtx;

void encrypte_message(const string &str, int index) {
    if (str.length() % 2 != 0) {
        unique_lock<mutex> u_lock(mtx); //mtx.lock()
        cout << "check the file.";
        exit(0);
    }
    string temp;
    string res;
    for (char i : str) {
        temp += i;
        if (temp.length() >= 2) {
            res += alphabet[stoi(temp) - 10];
            temp.clear();
        }
    }
    out[index] = res;
}

void make_tasks(unsigned int i, const string &msg) {
    int remainder = msg.length() % i;
    int subSlength = msg.length() / i;
    unsigned int RemainingItems = 0;
    if (remainder == 0 && subSlength % 2 != 0) {
        subSlength--;
        RemainingItems = i;
    } else if (remainder != 0 && subSlength % 2 == 0) {
        RemainingItems = remainder;
    } else if (remainder != 0 && subSlength % 2 != 0) {
        subSlength --;
        RemainingItems = i;
        RemainingItems += remainder;

    }
    for (int j = 0; j < i; j++) {
        if (j == i - 1) {
            tasks.push(msg.substr(j * subSlength, subSlength + RemainingItems));
            break;
        }
        tasks.push(msg.substr(j * subSlength, subSlength));
    }

}

void decoding(unsigned int i, const string &encryptedMessage) {
    out = new string[i];
    make_tasks(i, encryptedMessage);
    thrs = new thread[i];
    int index = 0;
    while (true) {
        string curr = tasks.front();
        tasks.pop();
        thrs[index] = thread(encrypte_message, curr, index);
        index++;
        if (tasks.empty()) {
            for (int j = 0; j < i; j++)
                thrs[j].join();
            return;
        }
    }
}

string read_encrypted_message(const string &path) {
    string strs;
    ifstream ifs;
    ifs.open(path);
    if (!ifs) {
        cout << "Unable to open file";
        exit(1);
    }
    string line;
    while (getline(ifs, line)) {
        strs += line;
    }
    ifs.close();
    return strs;
}

int main(int argc, char **args) {
    if (argc != 2) {
        cout << "input mistake"
             << endl;
        exit(0);
    }
    string encryptedMessage = read_encrypted_message(args[1]);
    if (encryptedMessage.length() % 2 != 0) {
        cout << "check your file" << endl;
        exit(0);
    }
    unsigned int numOfThread = (encryptedMessage.length() / 4);
    decoding(numOfThread, encryptedMessage); // start decoding
    cout << "decoded:" << endl;
    for (int j = 0; j < numOfThread; j++) {
        cout << out[j];
    }
    return 0;
}