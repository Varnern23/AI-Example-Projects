import static java.lang.Math.*;
This statement allows direct access to static members of the Math class, such as abs(), sqrt(), pow(), etc., without the need to write Math.methodName(). For example, instead of Math.sqrt(25), one could simply write sqrt(25).
Alternatively, specific methods can be statically imported:
Java

import static java.lang.Math.sqrt;
This imports only the sqrt method, and it can be used directly without the class name.
Using import java.lang.Math.* or import static java.lang.Math.methodName is optional. The Math class is always accessible in Java programs without any import statement, due to it being part of the java.lang package.

Public Class RubiksCube(){
  private int cubeState[6][8];
  //stores an array of numbers to track the sides of the rubiks cube.
  private int numTurn = 0;
  private int size = 2;
  public void initializeCube(){
    //sets the cubeState to a solved Cube with a nested for loop. Also sets unused space to a value of -1.
  }
  public void cycle(int hold){
    if (hold == 0){
      // cycles the values representing the face of the cube that is just cycling the same four numbers/colors.
    } //etc...
    // make more conditionals depending on which type of the 6 possible turns we make. I mean six by the way because a clockwise turn of the bottom half leads to the same result as a counter clockwise turn on the top half.
  }
  public void checkSolve(){
    //Goes through the array and makes sure every 2x2 section of integers are all equal using a loop that increments by 2 and a nested for loop that increments by 2 so every top left square is checked and the surrounding squares are compared.
    //It then prints out something along the lines of "puzzle solves".
  }
  public void rotateTopC(){
    //moves over every int in the array over to the right by two except the right face and back face where we take the right face values to the back face and the back face to the left face which we check via the use of a conditional checking the index. It then calls the cycle function.
    //Then calls checkSolve.
  }
  public void rotateBottomC(){
    //does the same thing as rotateTopC but calls a different integer for the cycle call and is used on the 4th row(index 3) and 8th row of the cube modeling array.
  }
  public void rotateLeftCC(){
    //same system of rotation but no exceptions with this one that calls cycle with a certain value, but we are rotating column 3(index 2) by decreasing the row index by 2 and we once again use conditionals to see where we need to wrap around.
  }
  public void rotateRightCC(){
    //same as rotateLeftUp but instead increasing row index by 2 and using a different int in the cycle call.
  }
  public void rotateFrontC(){
    //this rotate is a bit different as we have to rotate vertically like previous rotates on the 2nd and 5th columns and horizontally on the 2nd and 5th so that all faces turn properly and also cycle is once again called to cycle the front face.
  }
  public void rotateBackC(){
    //This rotate works like the rotateFrontC but you just reverse the direction.
  }
  public void rotateTopCC(){
    //exact same as rotateBottomC
  }
  public void rotateBottomC(){
    //exact same as rotateTopCC
  }
  public void rotateLeftC(){
    //exact same as rotateRightCC
  }
  public void rotateFrontCC(){
    //exact same as rotateBackC
  }
  public void rotateBackCC(){
    //exact same as rotateFrontC
  }
  public void randomize(){
    int numberOfRotations;
    //numberOfRotations is a random number between 15 and 45 which will decide how many random rotations we will have.
    //we then create a loop that runs for numberOfRotations iterations where we then look for a random number 1-6 and choose a type of rotation based on what that random number is and then enacts that type of rotation.
  } 
//Example of rubiks cube model array
/*[001100](top face)
  [001100]
  [223344](front face)
  [223344]
  [005500](bottom face)
  [005500]
  [006600](back face)
  [006600]*/
  

  

}
