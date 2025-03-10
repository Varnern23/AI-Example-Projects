package Rubiks;
  import java.util.List;
  import java.util.Scanner;
  public class main {
            public static void main(String[] args) {
                RubiksCube cubert = new RubiksCube();
                cubert.initializeCube();
                Scanner reader = new Scanner(System.in);
                int temp;
                System.out.println("Here's a rubiks cube have a blast!");
                cubert.scramble(5);
                cubert.printCube();
                long start = System.currentTimeMillis();
                System.out.println("Solving cube........");
                List<Integer> solution = cubert.solve();
                temp = solution.size();
                long end = System.currentTimeMillis();
                cubert.printCube();
                System.out.println(temp);
                System.out.println(end-start);
                
            }
        }
