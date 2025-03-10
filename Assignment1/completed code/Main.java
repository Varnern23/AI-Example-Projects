  import java.util.Scanner;
  public class Main {
            public static void main(String[] args) {
                RubiksCube cubert = new RubiksCube();
                Scanner reader = new Scanner(System.in);
                int temp;
                System.out.println("Here's a rubiks cube have a blast!");
                while(cubert.getNumTurn() == 0 || cubert.checkSolve() == false){
                    cubert.printCube();
                    temp = 0;
                    System.out.println("would you like to turn the front(1), back(2), left(3), right(4), top(5), or the bottom(6)?");
                    temp = reader.nextInt();
                    if (temp == 1){
                        int tempt = 0;
                        System.out.println("would you like to turn it clockwise(7) or counter clockwise(8)?");
                        tempt = reader.nextInt();
                        if (tempt == 7){
                            cubert.rotateFrontC();
                        }
                        else {
                            cubert.rotateFrontCC();
                        }
                    }
                     if (temp == 2){
                        int tempt = 0;
                        System.out.println("would you like to turn it clockwise(7) or counter clockwise(8)?");
                        tempt = reader.nextInt();
                        if (tempt == 7){
                            cubert.rotateBackC();
                        }
                        else {
                            cubert.rotateBackCC();
                        }
                    }
                     if (temp == 3){
                        int tempt = 0;
                        System.out.println("would you like to turn it clockwise(7) or counter clockwise(8)?");
                        tempt = reader.nextInt();
                        if (tempt == 7){
                            cubert.rotateLeftC();
                        }
                        else {
                            cubert.rotateLeftCC();
                        }
                    }
                     if (temp == 4){
                        int tempt = 0;
                        System.out.println("would you like to turn it clockwise(7) or counter clockwise(8)?");
                        tempt = reader.nextInt();
                        if (tempt == 7){
                            cubert.rotateRightC();
                        }
                        else {
                            cubert.rotateRightCC();
                        }
                    }
                     if (temp == 5){
                        int tempt = 0;
                        System.out.println("would you like to turn it clockwise(7) or counter clockwise(8)?");
                        tempt = reader.nextInt();
                        if (tempt == 7){
                            cubert.rotateTopC();
                        }
                        else {
                            cubert.rotateTopCC();
                        }
                    }
                     if (temp == 6){
                        int tempt = 0;
                        System.out.println("would you like to turn it clockwise(7) or counter clockwise(8)?");
                        tempt = reader.nextInt();
                        if (tempt == 7){
                            cubert.rotateBottomC();
                        }
                        else {
                            cubert.rotateBottomCC();
                        }
                    }
                }
            }
        }
