#include "stdafx.h"
using namespace std;


void print_vector(vector<int> vec) {
    for (int i: vec) {
        std::cout << i << " ";
    }
    std::cout << "\n\n";
};

int get_seed() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,10000000);
    return dist6(rng);
};

vector<int> select_from(vector<int> target, int num) {
    vector<int> result;
    std::sample(target.begin(), target.end(), back_inserter(result), num, mt19937(random_device()()));
    return result;
};

// duplicate detection
int detect_duplicate(vector<int> vec, int num = 3) {
    map<int, int> duplicate;
    vector<int> nums_sorted{vec};
    sort(begin(nums_sorted), end(nums_sorted));

    auto beg = begin(nums_sorted) + 1;
    for (;beg != end(nums_sorted); ++beg) {
        if (*beg == *(beg - 1)) {
            duplicate[*beg]++;
        }
    }

    int count = 0;

    for (const auto& i : duplicate) {
        if ((i.second + 1) >= num) {
            count++;
        }
    }
    return count;
};

// random selection
vector<int> random_selection_all(vector<int> fir, vector<int> sec, vector<int> thi, int num) {
    vector<int> out1 = select_from(fir, num);
    vector<int> out2 = select_from(sec, num);
    vector<int> out3 = select_from(thi, num);
    int dup1 = detect_duplicate(out1);
    int dup2 = detect_duplicate(out2);
    int dup3 = detect_duplicate(out3);
    vector<int> result;
    result.push_back(dup1);
    result.push_back(dup2);
    result.push_back(dup3);
    return result;
};

// pick every n
int pick_every_n(vector<int> vec, int num) {
    int s = vec.size();
    vector<int> temp;
    for (int i = 0; i < s; i = i + num) {
        temp.push_back(vec[i]);
    }
    return detect_duplicate(temp);
};

// pick every n
vector<int> pick_every_n_all(vector<int> fir, vector<int> sec, vector<int> thi, int num) {
    vector<int> result;
    result.push_back(pick_every_n(fir, num));
    result.push_back(pick_every_n(sec, num));
    result.push_back(pick_every_n(thi, num));
    return result;
};

int main(int argc, char* argv[]) {
    
    // initial loading process
    vector<int> cards1; // completely random
    vector<int> cards2; // "semi" random
    vector<int> cards3; // completely not random
    for (int k = 0; k < 3; k++) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 4; j++) {
                cards1.push_back(i + 10 * k + 1);
                cards2.push_back(i + 10 * k + 1);
                cards3.push_back(i + 10 * k + 1);
            }
        }
    }

    auto rng1 = default_random_engine (get_seed());
    auto rng2 = default_random_engine (get_seed());

    shuffle(begin(cards1), end(cards1), rng1);
    shuffle(begin(cards2), end(cards2), rng2);

    // keep half of the cards unshuffled
    size_t const half_si = cards1.size() / 2;
    vector<int> split_lo(cards2.begin(), cards2.begin() + half_si); // remains unchanged
    vector<int> split_hi(cards2.begin() + half_si, cards2.end()); // keeps shuffling
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    double entro1 = 0;
    double entro2 = 0;
    double entro3 = 0;
    int simuNum = 1000000;
    for (int i = 0; i < simuNum; i++) {
        auto r1 = default_random_engine (get_seed());
        auto r2 = default_random_engine (get_seed());
        shuffle(begin(cards1), end(cards1), r1);
        shuffle(begin(split_hi), end(split_hi), r2);
        vector<int> temp2(split_lo);
        temp2.insert(temp2.end(), split_hi.begin(), split_hi.end());
        vector<int> result = pick_every_n_all(cards1, temp2, cards3, 4);
        //vector<int> result = random_selection_all(cards1, temp2, cards3, 20);
        count1 = count1 + result[0];
        count2 = count2 + result[1];
        count3 = count3 + result[2];
    }
    cout << "count1: " << count1 << "\n";
    cout << "count2: " << count2 << "\n";
    cout << "count3: " << count3 << "\n"; 
};