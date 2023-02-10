/* Sample solution to "Funny Games" from NCPC 2005
 * Algorithm: essentially continuous DP, keep track of winning intervals
 * Author: Per Austrin
 */ 
#include <bits/stdc++.h>


using namespace std;

std::ifstream cin2("in_funnygames.txt");

typedef pair<double, double> pdd;
// first call: fill in from [l, nwin] = [0, k] at l (dest.)
template <class It, class OIt>
OIt ival_union(It begin, It end, OIt dest) {
  sort(begin, end);
  // sort so lowest value goes first. with k = 0.5 0.25 that means 1/k is 2, 4 and we have highest k first.
  while(begin != end) {
    *dest = *begin++; // copy value 0. is a pair (1, 1/k)
    // shift begin to nr 2 (4). 
    // if new pair second is greater than current pair first, we update dest second with shifting begin.
    while(begin != end && begin->first < dest->second + 1e-8) {
        dest->second=std::max(dest->second, begin++->second);
    }
    ++dest;
  }
  return dest;
}   

int main(void) {
  int n, k;
  double x, f[10], maxf;
  double inv_f[10], inv_maxf;
  pdd win[10000], lose;
  // n test cases
  cin2 >> n;
  for (; n--; ) {
    // goal = x, k = number of weapons
    cin2 >> x >> k;
//    scanf("%lf%d", &x, &k);
    // array of 10000 "win" pairs and 1 lose pair.
    maxf = lose.second = 0;

    for (int i = 0; i < k; ++i) {
        // read weapons into f
        // set the first k entries to win to (1, 1/F_i)
        // set maxf to the highest F.
        cin2 >> f[i];
        inv_f[i] = 1 / f[i];
       //scanf("%lf", f + i), 
       win[i] = make_pair(1, inv_f[i]);
       maxf = std::max(maxf, f[i]);
       inv_maxf = 1 / maxf;
    }
    // lose.first = lose.second = 0
    // goal goes down to lose.second
    for (int nwin = k, l = 0; x > lose.second; ++l) {
        // ival_union returns an iterator. We find the offset and put into nwin.
        // nwin starts at k.
        // l starts at 0
        // [l, nwin] defines a range. 
        // we fill in the array at l.
        nwin = ival_union(win + l , win + nwin, win + l) - win;
        lose.second = (lose.first = win[l].second) * inv_maxf;
        if (l < nwin - 1) {
            lose.second=std::min(lose.second, win[l + 1].first);
        }

        for (int i = 0; i < k; ++i) {
            win[nwin++] = make_pair(lose.first * inv_f[i], lose.second * inv_f[i]);
        }
    }
    assert(fabs(x-lose.first) > 1e-6 && fabs(x-lose.second) > 1e-6);
    printf("%s\n", x < lose.first ? "Nils" : "Mikael");
  }
}
