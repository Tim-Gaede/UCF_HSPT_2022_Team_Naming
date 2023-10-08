import itertools

INF = int(1e9) # infinity for our DP

# solve a single test case
def solve():
    # read in the input
    strs = input().split()

    # sanity check: ensure there are 3 non-empty strings of 
    # lowercase letters, each of length at most 100
    assert len(strs) == 3
    assert all(all("a" <= c <= "z" for c in s) for s in strs)
    assert all(1 <= len(s) <= 100 for s in strs)

    # store the lengths of the strings two different ways
    Ns = tuple(len(s) for s in strs)
    N0, N1, N2 = Ns

    # convert strings to integer arrays: a..z -> 0..25, padding the end with -1
    arrs = tuple(tuple(ord(c) - ord("a") for c in s) + (-1,) for s in strs)

    # build DP table of size (N0 + 1) x (N1 + 1) x (N2 + 1)
    dp = [[[INF] * (N2+1) for _ in range(N1+1)] for _ in range(N0+1)]
    # this table will store the optimal transition for each transition
    trans = [[[-1] * (N2+1) for _ in range(N1+1)] for _ in range(N0+1)]

    # run the dp from the end of the string to the front, 
    # since we always build off shorter strings
    for x in reversed(range(N0+1)):
        for y in reversed(range(N1+1)):
            for z in reversed(range(N2+1)):
                # base case: we are at the end of all 3 strings
                if (x, y, z) == Ns:
                    dp[x][y][z] = 0
                    continue

                # the letters we are currently looking at
                cs = (arrs[0][x], arrs[1][y], arrs[2][z])
                
                # the letter we place here must be one of the letters our pointers look at
                # otherwise, we place a letter but don't move forwards in any of the strings
                for c in cs:
                    if c == -1:
                        continue
                    
                    # each string's pointer gets moved up if the letter
                    # we are placing is the same as the letter at that position.
                    # in Python, True = 1 and False = 0 when adding to an integer
                    cost = 1 + dp[x + (cs[0] == c)][y + (cs[1] == c)][z + (cs[2] == c)]

                    # if this cost is lower than our previous best 
                    # OR the costs are the same, but this letter is smaller,
                    # then take this letter and cost
                    if (cost, c) < (dp[x][y][z], trans[x][y][z]):
                        dp[x][y][z] = cost
                        trans[x][y][z] = c

    # now, we follow our transition table to find the best path in our DP,
    # to build back our answer
    ans = []
    x, y, z = 0, 0, 0
    while dp[x][y][z] != 0:
        c = trans[x][y][z]
        ans.append(c)
        x += arrs[0][x] == c
        y += arrs[1][y] == c
        z += arrs[2][z] == c

    # convert back from ints to letters and print the answer
    ans = "".join(chr(c + ord("a")) for c in ans)
    print(ans)

if __name__ == "__main__":
    t = int(input())
    for _ in range(t):
        solve()