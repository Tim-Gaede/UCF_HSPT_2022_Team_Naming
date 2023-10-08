import java.util.*;

public class naming{
    //Define infinity - our upper limit
    final static int INF = (int)1e9;

    public static void main(String[] args) {
        //Declare and initialize scanner
        Scanner in = new Scanner(System.in);

        //Scan in number of teams
        int numTeams = in.nextInt();

        //Loop through the teams
        for(int team = 0; team < numTeams; team++){
            //Scan in each team member's name
            String name1 = in.next(), name2 = in.next(), name3 = in.next();
            //Get the length of each name
            int l1 = name1.length(), l2 = name2.length(), l3 = name3.length();

            //Add a delimiter to the end of each name to easily detect the end
            name1 += '$'; name2 += '$'; name3 += '$';
            
            //To solve this problem we are going to use dynammic programming:
            //Our states will be defined as the index in our first name, second name, and third name, respectively.
            //Our transitions will be taking one (or more if they are the same) letters.
            //When we take a letter that is like looking forward in our dp table incrementing the respective indeces.
            //After running the dp, we have to build back our string:
            //To do this keep track of what character provides the optimal result.
            //A result is optimal if it produces the shortest string and takes the lowest lexigraphical character as a tie braker.

            //create the dp table which will store the length of the longest common supersequence
            int dp[][][] = new int[l1+1][l2+1][l3+1];
            //create the buildBack table which will store the optimal choice at a current state
            char buildBack[][][] = new char[l1+1][l2+1][l3+1];

            //Iniatialize each element of the dp table to infinity
            for(int i = 0; i <= l1; i++)
                for(int j = 0; j <= l2; j++)
                    Arrays.fill(dp[i][j], INF);
            
            //Initialize each element of the buildBack table to the delimiter
            for(int i = 0; i <= l1; i++)
                    for(int j = 0; j <= l2; j++)
                        Arrays.fill(buildBack[i][j], '$');
            
            //loop through each state
            //We work from the end of our strings to the beginning
            for(int i = l1; i >= 0; i--){
                for(int j = l2; j >= 0; j--){
                    for(int k = l3; k >= 0; k--){

                        //If we are at the end of our strings, store 0
                        if(i == l1 && j == l2 && k == l3){
                            dp[i][j][k] = 0;
                            continue;
                        }
                        
                        //get our current characters
                        char curr[] = {name1.charAt(i), name2.charAt(j), name3.charAt(k)};

                        //loop through each of our current letters
                        for(char c : curr){

                            //if we hit the delimiter, continue
                            if(c == '$')
                                continue;
                            
                            //get each of our next indices
                            //to get this we only want to look forward if
                            //our current letter matches the character
                            //from our loop
                            int nextI = i + boolToInt(c == curr[0]),
                                nextJ = j + boolToInt(c == curr[1]),
                                nextK = k + boolToInt(c == curr[2]);
                            
                            //our potential value is our next state + 1
                            int potVal = dp[nextI][nextJ][nextK] + 1;

                            //if our potential value is better, take it
                            if(potVal < dp[i][j][k]){
                                dp[i][j][k] = potVal;
                                buildBack[i][j][k] = c;
                            }
                            //if our potential value is just as good,
                            //see if this character creates a string
                            //that is lexographically lower
                            else if(potVal == dp[i][j][k] && c < buildBack[i][j][k]){
                                buildBack[i][j][k] = c;
                            }
                        }
                    }
                }
            }

            //Build back our answer
            String ans = "";
            int i = 0, j = 0, k = 0;
            //While we do not hit the end of our dp table,
            //add the respective character from the build back table
            //and only increment and index if the character in the name
            //matches the character from the buildBack table
            while(dp[i][j][k] != 0){
                char c = buildBack[i][j][k];
                ans += c;
                i += boolToInt(name1.charAt(i) == c);
                j += boolToInt(name2.charAt(j) == c);
                k += boolToInt(name3.charAt(k) == c);
            }

            //Print out answer
            System.out.println(ans);
        }

        //Close our scanner
        in.close();
    }

    //Returns 1 if true and 0 if false
    public static int boolToInt(boolean b){
        return b ? 1 : 0;
    }
}
