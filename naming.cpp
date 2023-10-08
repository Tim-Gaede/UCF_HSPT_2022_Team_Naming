#include <bits/stdc++.h>
using namespace std;

// we will use this big value later in the code
const int A_BIG_VALUE = 1e9;

int main(){
    // number of test cases
    int t;
    cin >> t;

    // floop over each test case and solve each individually
    for(int testNum = 0; testNum < t; ++testNum){
        // grab our strings
        string a, b, c;
        cin >> a >> b >> c;

        // useful variables
        int aLen = size(a), bLen = size(b), cLen = size(c);

        // our solution will use the concept of partial solutions; this concept is also known
        // as dynamic programming (DP); we will start by building small partial solutions, and
        // then using these smaller partial solutions to build bigger and bigger partial
        // solutions; the partial solutions we will be building in this problem are to construct
        // the shortest and lexicographically smallest string that contains the suffix of a from
        // [aIdx, aLen - 1], the suffix of b from [bIdx, bLen - 1], and the suffix of c from
        // [cIdx, cLen - 1] where aIdx, bIdx, and cIdx represent the starting index of the suffixes
        // of a, b, and c respectively; we can construct a recursive relationship between one of these
        // partial solutions and others; perhaps we are trying to build the partial solution for suffixes
        // [aIdx, aLen - 1], [bIdx, bLen - 1], and [cIdx, cLen - 1] of a, b, and c respectively; we can
        // start building this partial solution by placing the first letter; if we place a letter that
        // doesn't match any of a[aIdx], b[bIdx], or c[cIdx], then that would be suboptimal as we could
        // take the solution that that would build and remove the character that none of the strings use;
        // so we know that an optimal first letter for this partial solution is one of a[aIdx], b[bIdx],
        // or c[cIdx]; so let's say the letter we decide to place for our first letter to this partial
        // solution is the character x; well then we can advance our indices that match this character,
        // and use the smaller partial solution; for example, let's say that both a[aIdx] == 'x' and
        // b[bIdx] == 'x'; then we can say that a potential partial solution for suffixes [aIdx, aLen - 1],
        // [bIdx, bLen - 1], and [cIdx, cLen - 1] of a, b, and c could potentially be "x" + (the partial
        // solution for [aIdx + 1, aLen - 1], [bIdx + 1, bLen - 1], [cIdx, cLen - 1]); notice that aIdx
        // and bIdx were advanced since the suffixes of a and b can use the 'x' that we placed and now we
        // must have the suffixes of [aIdx + 1, aLen - 1], [bIdx + 1, bLen - 1], and [cIdx, cLen - 1] as
        // subsequences in our remaining string, which is an exact description of smaller partial solution
        // with aIdx and bIdx advanced; we will also remember to try all of the relevant characters to place
        // first in our partial solution, as placing some characters now might be better in the future

        // we will store the length of every partial solution in a 3D array called dp of size (aLen + 1) by
        // (bLen + 1) by (cLen + 1), and a partial solution will be referenced by dp[aIdx][bIdx][cIdx]

        // we will also store the best first character to each partial solution so that we can build back our
        // answer after building all of our partial solutions

        // we will also note that our dp allows us to have partial solutions with empty suffixes; for example,
        // if aIdx == aLen, then we are considering the suffix of a in the range [aLen, aLen - 1] inclusive;
        // this is an empty range; we will be sure to be cautious about this when accessing a[aIdx] and
        // similarly for b and c

        // we can now define our base case at dp[aLen][bLen][cLen] (all empty suffixes) = 0, as the partial
        // solution for these suffixes is the empty string ""

        // make dem tables
        vector<vector<vector<int>>> dp(aLen + 1, vector<vector<int>>(bLen + 1, vector<int>(cLen + 1)));
        vector<vector<vector<char>>> best(aLen + 1, vector<vector<char>>(bLen + 1, vector<char>(cLen + 1)));

        // we will work through our suffixes in reverse order; what this ensures is that all smaller partial
        // solutions are built first; in particular, for any partial solution that relies on another, the one
        // relied on will be built first
        for(int aIdx = aLen; aIdx >= 0; --aIdx){
            for(int bIdx = bLen; bIdx >= 0; --bIdx){
                for(int cIdx = cLen; cIdx >= 0; --cIdx){
                    // this is our base case; empty string
                    if(aIdx == aLen && bIdx == bLen && cIdx == cLen){
                        dp[aIdx][bIdx][cIdx] = 0;
                        // our best first character doesn't really exist in this case, so we will omit filling
                        // this character in the table
                        continue;
                    }

                    // otherwise, we must place a character here; the only optimal characters are in the set
                    // of first characters of our 3 suffixes, assuming they exist

                    // we will store our possible first characters in a set; this ensures that there are no
                    // duplicate characters in our list which is not necessary just convenient, and it also
                    // ensures that we iterate over them in sorted order
                    set<char> possFirstChars;
                    if(aIdx < aLen) possFirstChars.insert(a[aIdx]);
                    if(bIdx < bLen) possFirstChars.insert(b[bIdx]);
                    if(cIdx < cLen) possFirstChars.insert(c[cIdx]);

                    // now we will consider the partial solutions of each first character and choose the one that
                    // gives us the best answer (the smallest length, and of those the smallest first character to
                    // ensure we are staying lexicographically lowest)
                    
                    // we will initialize our current partial solution to have a really big length; the magnitude of
                    // this value is not too important as long as it is greater than any possible partial solution we
                    // could build in our dp table so that it immediately gets overwritten
                    int bestCurrSolLen = A_BIG_VALUE;
                    char bestCurrSolFirstChar = 0;

                    // floop over our possible first characters for this partial solution
                    for(char possFirstChar : possFirstChars){
                        // since they are in sorted order, the only time we could find a better partial solution
                        // now then the one we have already considered is if our length is lower; if our length
                        // is the same as the one stored in bestCurrSolLen, then bestCurrSolFirstChar will be smaller

                        // we will create variables for what the indices in our new partial solution will be
                        int newAIdx = aIdx;
                        int newBIdx = bIdx;
                        int newCIdx = cIdx;
                        // if the string at its index matches what character we are placing, we will bump it
                        if(aIdx < aLen && a[aIdx] == possFirstChar) ++newAIdx;
                        if(bIdx < bLen && b[bIdx] == possFirstChar) ++newBIdx;
                        if(cIdx < cLen && c[cIdx] == possFirstChar) ++newCIdx;


                        // we will use this character (+1 to our length), and then the length of the next partial
                        // solution
                        int possSolLen = 1 + dp[newAIdx][newBIdx][newCIdx];
                        
                        // update our best current partial solution if we find a smaller one here
                        if(possSolLen < bestCurrSolLen){
                            bestCurrSolLen = possSolLen;
                            bestCurrSolFirstChar = possFirstChar;
                        }
                    }

                    // after the floop, our best partial solution data will be stored in our variables and we
                    // can drop them into our table for later
                    dp[aIdx][bIdx][cIdx] = bestCurrSolLen;
                    best[aIdx][bIdx][cIdx] = bestCurrSolFirstChar;
                }
            }
        }

        // we will now walk through our dp table starting from what should be our biggest partial solution, which
        // is actually the answer to the entire problem: dp[0][0][0]; we will take the transitions that we already
        // found as our best transitions and send it

        // we can print each character as we traverse our table and that will be our answer
        int aIdx = 0, bIdx = 0, cIdx = 0;
        // continue while we don't have all empty suffixes
        while(!(aIdx == aLen && bIdx == bLen && cIdx == cLen)){
            // take our best transition
            char firstChar = best[aIdx][bIdx][cIdx];

            // print this character
            cout << firstChar;

            // bump our indices perhaps
            if(aIdx < aLen && a[aIdx] == firstChar) ++aIdx;
            if(bIdx < bLen && b[bIdx] == firstChar) ++bIdx;
            if(cIdx < cLen && c[cIdx] == firstChar) ++cIdx;
        }

        // don't forget dat new line character
        cout << "\n";
    }

    return 0;
}
/*

samples for copy/pasta convenience

3
daniel jacob lior
tyler kyle david
atharva anderson ahmad

*/
