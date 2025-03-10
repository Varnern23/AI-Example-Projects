package Rubiks;
//Nathan Varner so and so
import static java.lang.Math.*;

import java.util.Random;
public class RubiksCube{
  public int[][] cubeState = new int [8][6];
  //This is a pretty brute force way of jandling this prompt that sacrifices scalability for efficiency. aka leet code answers that you go was that really neccesary when you coulda just used a loop but they are still in the top 1% of time efficency but on a lower scale.
  public int numTurn = 0;
  public void initializeCube(){
    //creates the cube model and gives it values
        cubeState[0] = new int[] {0, 0, 1, 1, 0, 0};
        cubeState[1] = new int[] {0, 0, 1, 1, 0, 0};
        cubeState[2] = new int[] {2, 2, 3, 3, 4, 4};
        cubeState[3] = new int[] {2, 2, 3, 3, 4, 4};
        cubeState[4] = new int[] {0, 0, 5, 5, 0, 0};
        cubeState[5] = new int[] {0, 0, 5, 5, 0, 0};
        cubeState[6] = new int[] {0, 0, 6, 6, 0, 0};
        cubeState[7] = new int[] {0, 0, 6, 6, 0, 0};
  }
  public int getNumTurn(){
    return numTurn;
  }
  public void cycle(int hold){
    //is in charges of cycling the sides that dont actually change any values just rotate them along the same face.
    if (hold == 0){
        int temp = cubeState[0][2];
        cubeState[0][2] = cubeState[0][3];
        cubeState[0][3] = cubeState[1][3];
        cubeState[1][3] = cubeState[1][2];
        cubeState[1][2] = temp;
        numTurn ++;
      // cycles the values representing the face of the cube that is just cycling the same four numbers/colors.
    } 
    else if (hold == 1){
        int temp = cubeState[4][2];
        cubeState[4][2] = cubeState[4][3];
        cubeState[4][3] = cubeState[5][3];
        cubeState[5][3] = cubeState[5][2];
        cubeState[5][2] = temp;
        numTurn ++;

    }
    else if (hold == 2){
        int temp = cubeState[2][0];
        cubeState[2][0] = cubeState[3][0];
        cubeState[3][0] = cubeState[3][1];
        cubeState[3][1] = cubeState[2][1];
        cubeState[2][1] = temp;
        numTurn ++;

    }
    else if (hold == 3){
        int temp = cubeState[2][4];
        cubeState[2][4] = cubeState[2][5];
        cubeState[2][5] = cubeState[3][5];
        cubeState[3][5] = cubeState[3][4];
        cubeState[3][4] = temp;
        numTurn ++;

    }
    else if (hold == 4){
        int temp = cubeState[2][2];
        cubeState[2][2] = cubeState[2][3];
        cubeState[2][3] = cubeState[3][3];
        cubeState[3][3] = cubeState[3][2];
        cubeState[3][2] = temp;
        numTurn ++;
    }
    else if (hold == 5){
        int temp = cubeState[2][2];
        cubeState[2][2] = cubeState[3][2];
        cubeState[3][2] = cubeState[3][3];
        cubeState[3][3] = cubeState[2][3];
        cubeState[2][3] = temp;
        numTurn ++;

    }
    // made conditionals depending on which type of the 6 possible turns we make. I mean six by the way because a clockwise turn of the bottom half leads to the same result as a counter clockwise turn on the top half.
  }
  public boolean cubeCheck(){
    int[][] cubeCheck = new int[8][6];
    boolean result = true;
    if (cubeState[0][2] != cubeState[1][2]){
      result = false;
    }
    else if (cubeState[0][2] != cubeState[1][3]){
      result = false;
    }
    else if (cubeState[0][2] != cubeState[0][3]){
      result = false;
    }
    else if (cubeState[2][0] != cubeState[2][1]){
      result = false;
    }
    else if (cubeState[2][0] != cubeState[3][0]){
      result = false;
    }
    else if (cubeState[2][0] != cubeState[3][1]){
      result = false;
    }
    else if (cubeState[2][2] != cubeState[2][3]){
      result = false;
    }
    else if (cubeState[2][2] != cubeState[3][3]){
      result = false;
    }
    else if (cubeState[2][2] != cubeState[3][2]){
      result = false;
    }
    else if (cubeState[2][4] != cubeState[2][5]){
      result = false;
    }
    else if (cubeState[2][4] != cubeState[3][5]){
      result = false;
    }
    else if (cubeState[2][4] != cubeState[3][4]){
      result = false;
    }
    else if (cubeState[4][2] != cubeState[4][3]){
      result = false;
    }
    else if (cubeState[4][2] != cubeState[5][2]){
      result = false;
    }
    else if (cubeState[4][2] != cubeState[5][3]){
      result = false;
    }
    else if (cubeState[6][2] != cubeState[6][3]){
      result = false;
    }
    else if (cubeState[6][2] != cubeState[7][2]){
      result = false;
    }
    else if (cubeState[6][2] != cubeState[7][3]){
      result = false;
    }
    if (result == true){
        System.out.println("The Cube is Solved!");
    }
    return result;
  }
  public void rotateTopC(){
    int tempOne = cubeState[6][2];
    int tempTwo = cubeState[6][3];
    cubeState[6][2] = cubeState[2][4];
    cubeState[6][3] = cubeState[2][5];
    cubeState[2][4] = cubeState[2][2];
    cubeState[2][5] = cubeState[2][3];
    cubeState[2][2] = cubeState[2][0];
    cubeState[2][3] = cubeState[2][1];
    cubeState[2][0] = tempOne;
    cubeState[2][1] = tempTwo;
    this.cycle(0);
    this.cubeCheck();
    //moves over every int in the array over to the right by two except the right face and back face where we take the right face values to the back face and the back face to the left face which we check via the use of a conditional checking the index. It then calls the cycle function.
    //Then calls checkSolve.
  }
  public void rotateBottomC(){
    int tempOne = cubeState[7][2];
    int tempTwo = cubeState[7][3];
    cubeState[7][2] = cubeState[3][4];
    cubeState[7][3] = cubeState[3][5];
    cubeState[3][4] = cubeState[3][2];
    cubeState[3][5] = cubeState[3][3];
    cubeState[3][2] = cubeState[3][0];
    cubeState[3][3] = cubeState[3][1];
    cubeState[3][0] = tempOne;
    cubeState[3][1] = tempTwo;
    this.cycle(1);
    this.cubeCheck();
    //does the same thing as rotateTopC but calls a different integer for the cycle call and is used on the 4th row(index 3) and 8th row of the cube modeling array.
  }
  public void rotateLeftC(){
    int tempOne = cubeState[6][2];
    int tempTwo = cubeState[7][2];
    cubeState[6][2] = cubeState[4][2];
    cubeState[7][2] = cubeState[5][2];
    cubeState[4][2] = cubeState[2][2];
    cubeState[5][2] = cubeState[3][2];
    cubeState[3][2] = cubeState[1][2];
    cubeState[2][2] = cubeState[0][2];
    cubeState[1][2] = tempTwo;
    cubeState[0][2] = tempOne;
    this.cycle(2);
    this.cubeCheck();
    //same system of rotation but no exceptions with this one that calls cycle with a certain value, but we are rotating column 3(index 2) by decreasing the row index by 2 and we once again use conditionals to see where we need to wrap around.
  }
  public void rotateRightC(){
    int tempOne = cubeState[6][3];
    int tempTwo = cubeState[7][3];
    cubeState[6][3] = cubeState[4][3];
    cubeState[7][3] = cubeState[5][3];
    cubeState[4][3] = cubeState[2][3];
    cubeState[5][3] = cubeState[3][3];
    cubeState[3][3] = cubeState[1][3];
    cubeState[2][3] = cubeState[0][3];
    cubeState[1][3] = tempTwo;
    cubeState[0][3] = tempOne;
    this.cycle(3);
    this.cubeCheck();
    //same as rotateLeftUp but instead increasing row index by 2 and using a different int in the cycle call.
  }
  public void rotateBackC(){
    int tempOne = cubeState[1][2];
    int tempTwo = cubeState[1][3];
    cubeState[1][2] = cubeState[3][1];
    cubeState[1][3] = cubeState[2][1];
    cubeState[3][1] = cubeState[4][2];
    cubeState[2][1] = cubeState[4][3];
    cubeState[4][2] = cubeState[3][4];
    cubeState[4][3] = cubeState[2][4];
    cubeState[3][4] = tempTwo;
    cubeState[2][4] = tempOne;
    this.cycle(5);
    this.cubeCheck();
    //this rotate is a bit different as we have to rotate vertically like previous rotates on the 2nd and 5th columns and horizontally on the 2nd and 5th so that all faces turn properly and also cycle is once again called to cycle the front face.
  }
        //pretty self explanatory stuff theres 6 rotation types and all of them were brute forced.
  public void rotateFrontC(){
    int tempOne = cubeState[1][2];
    int tempTwo = cubeState[1][3];
    cubeState[1][2] = cubeState[2][4];
    cubeState[1][3] = cubeState[3][4];
    cubeState[2][4] = cubeState[4][2];
    cubeState[3][4] = cubeState[4][3];
    cubeState[4][2] = cubeState[3][1];
    cubeState[4][3] = cubeState[2][1];
    cubeState[3][1] = tempTwo;
    cubeState[2][1] = tempOne;
    this.cycle(4);
    this.cubeCheck();
  }
  public void rotateTopCC(){
    this.rotateBottomC();
  }
  public void rotateBottomCC(){
    this.rotateTopCC();
  }
  public void rotateLeftCC(){
    this.rotateRightC();
  }
   public void rotateRightCC(){
    this.rotateLeftC();
  }
  public void rotateFrontCC(){
    this.rotateBackC();
  }
  public void rotateBackCC(){
    this.rotateFrontC();
  }
  public void printCube(){
    System.out.println("The front:");
    System.out.println("[" + cubeState[2][2] + "], [" + cubeState[2][3] + "]");
    System.out.println("[" + cubeState[3][2] + "], [" + cubeState[3][3] + "]");
    System.out.println("The left:");
    System.out.println("[" + cubeState[2][0] + "], [" + cubeState[2][1] + "]");
    System.out.println("[" + cubeState[3][0] + "], [" + cubeState[3][1] + "]");
    System.out.println("The right:");
    System.out.println("[" + cubeState[2][4] + "], [" + cubeState[2][5] + "]");
    System.out.println("[" + cubeState[3][4] + "], [" + cubeState[3][5] + "]");
    System.out.println("The back:");
    System.out.println("[" + cubeState[6][2] + "], [" + cubeState[6][3] + "]");
    System.out.println("[" + cubeState[7][2] + "], [" + cubeState[7][3] + "]");
    System.out.println("The top:");
    System.out.println("[" + cubeState[0][2] + "], [" + cubeState[0][3] + "]");
    System.out.println("[" + cubeState[1][2] + "], [" + cubeState[1][3] + "]");
    System.out.println("The bottom:");
    System.out.println("[" + cubeState[4][2] + "], [" + cubeState[4][3] + "]");
    System.out.println("[" + cubeState[5][2] + "], [" + cubeState[5][3] + "]");
  }
  private void applyMove(int move) {
    switch (move) {
        case 0 -> rotateTopC();
        case 1 -> rotateBottomC();
        case 2 -> rotateLeftC();
        case 3 -> rotateRightC();
        case 4 -> rotateFrontC();
        case 5 -> rotateBackC();
        case 6 -> rotateTopCC();
        case 7 -> rotateBottomCC();
        case 8 -> rotateLeftCC();
        case 9 -> rotateRightCC();
        case 10 -> rotateFrontCC();
        case 11 -> rotateBackCC();
        default -> System.out.println("Invalid move");
    }
}
  public void scramble(int numMoves) {
    Random rand = new Random();
    int[] moves = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    for (int i = 0; i < numMoves; i++) {
        int move;
        move = moves[rand.nextInt(moves.length)];  // Pick a random move
        applyMove(move);  // Execute the move
    }
    System.out.println("Cube scrambled with " + numMoves + " moves.");
}
  
}
