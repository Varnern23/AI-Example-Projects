import java.util.Arrays;

public class MDPSolver {
    static final int NUM_STATES = 16;
    static final int NUM_ACTIONS = 4;
    static final double PROB_SUCCESS = 0.7;
    static final double PROB_OPPOSITE = 0.2;
    static final double PROB_STAY = 0.1;
    static final double GAMMA = 0.95;
    static final double EPSILON = 1e-6;

    static final int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3;
    static final int[] ACTIONS = {UP, DOWN, LEFT, RIGHT};
    static final int[] dRow = {1, -1, 0, 0};
    static final int[] dCol = {0, 0, -1, 1};

    static double[][][] T = new double[NUM_STATES][NUM_STATES][NUM_ACTIONS];
    static double[] R = new double[NUM_STATES];

    public static void main(String[] args) {
        initializeRewards();
        initializeTransitions();

        System.out.println("=== Value Iteration ===");
        Result viResult = valueIteration();
        viResult.printResults();

        System.out.println("\n=== Policy Iteration ===");
        Result piResult = policyIteration();
        piResult.printResults();
    }

    static void initializeRewards() {
        Arrays.fill(R, 0.0);
        R[10] = 100;
        R[12] = 200;
        int[] fiftyStates = {1, 3, 5, 7, 9, 11, 13, 15};
        for (int s : fiftyStates) R[s] = 50;
    }

    static void initializeTransitions() {
        for (int s = 0; s < NUM_STATES; s++) {
            for (int a = 0; a < NUM_ACTIONS; a++) {
                for (int sp = 0; sp < NUM_STATES; sp++) T[sp][s][a] = 0.0;
                int succ = getNextState(s, a);
                T[succ][s][a] += PROB_SUCCESS;
                int opp = getOppositeAction(a);
                int oppState = getNextState(s, opp);
                T[oppState][s][a] += PROB_OPPOSITE;
                T[s][s][a] += PROB_STAY;
            }
        }
    }

    static int getNextState(int s, int action) {
        int row = s / 4;
        int col = s % 4;
        int newRow = row + dRow[action];
        int newCol = col + dCol[action];
        if (isValid(newRow, newCol)) {
            return newRow * 4 + newCol;
        } else {
            int bounce = getOppositeAction(action);
            int bounceRow = row + dRow[bounce];
            int bounceCol = col + dCol[bounce];
            if (isValid(bounceRow, bounceCol)) {
                return bounceRow * 4 + bounceCol;
            } else {
                return s;
            }
        }
    }

    static boolean isValid(int row, int col) {
        return row >= 0 && row < 4 && col >= 0 && col < 4;
    }

    static int getOppositeAction(int action) {
        switch (action) {
            case UP:    return DOWN;
            case DOWN:  return UP;
            case LEFT:  return RIGHT;
            case RIGHT: return LEFT;
            default:    return -1;
        }
    }

    static class Result {
        double[] U;
        int[] policy;
        Result(double[] U, int[] policy) { this.U = U; this.policy = policy; }

        void printResults() {
            System.out.println("Utilities:");
            for (int row = 3; row >= 0; row--) {
                for (int col = 0; col < 4; col++) {
                    int s = row * 4 + col;
                    System.out.printf("s%d: %.4f\t", s, U[s]);
                }
                System.out.println();
            }
            System.out.println("Policy:");
            for (int row = 3; row >= 0; row--) {
                for (int col = 0; col < 4; col++) {
                    int s = row * 4 + col;
                    System.out.printf("s%d: %s\t", s, actionToString(policy[s]));
                }
                System.out.println();
            }
        }
    }

    static String actionToString(int a) {
        switch (a) {
            case UP:    return "Up";
            case DOWN:  return "Down";
            case LEFT:  return "Left";
            case RIGHT: return "Right";
            default:    return "None";
        }
    }

    static Result valueIteration() {
        double[] U = new double[NUM_STATES];
        double[] UPrime = new double[NUM_STATES];
        Arrays.fill(U, 0.0);
        double delta;
        double threshold = EPSILON * (1 - GAMMA) / GAMMA;
        do {
            System.arraycopy(UPrime, 0, U, 0, NUM_STATES);
            delta = 0.0;
            for (int s = 0; s < NUM_STATES; s++) {
                double[] vals = new double[NUM_ACTIONS];
                for (int a : ACTIONS) {
                    for (int sp = 0; sp < NUM_STATES; sp++) vals[a] += T[sp][s][a] * U[sp];
                }
                double max = Arrays.stream(vals).max().getAsDouble();
                UPrime[s] = R[s] + GAMMA * max;
                delta = Math.max(delta, Math.abs(UPrime[s] - U[s]));
            }
        } while (delta >= threshold);
        int[] policy = new int[NUM_STATES];
        for (int s = 0; s < NUM_STATES; s++) {
            double best = Double.NEGATIVE_INFINITY;
            int bestA = 0;
            for (int a : ACTIONS) {
                double sum = 0;
                for (int sp = 0; sp < NUM_STATES; sp++) sum += T[sp][s][a] * U[sp];
                if (sum > best) { best = sum; bestA = a; }
            }
            policy[s] = bestA;
        }
        return new Result(U, policy);
    }

    static Result policyIteration() {
        double[] U = new double[NUM_STATES];
        int[] policy = new int[NUM_STATES];
        Arrays.fill(U, 0.0);
        Arrays.fill(policy, UP);
        boolean stable;
        do {
            for (int it = 0; it < 100; it++) {
                double[] nextU = new double[NUM_STATES];
                for (int s = 0; s < NUM_STATES; s++) {
                    double sum = 0;
                    int a = policy[s];
                    for (int sp = 0; sp < NUM_STATES; sp++) sum += T[sp][s][a] * U[sp];
                    nextU[s] = R[s] + GAMMA * sum;
                }
                U = nextU;
            }
            stable = true;
            for (int s = 0; s < NUM_STATES; s++) {
                int oldA = policy[s];
                double best = Double.NEGATIVE_INFINITY;
                int bestA = oldA;
                for (int a : ACTIONS) {
                    double sum = 0;
                    for (int sp = 0; sp < NUM_STATES; sp++) sum += T[sp][s][a] * U[sp];
                    if (sum > best) { best = sum; bestA = a; }
                }
                if (bestA != oldA) { policy[s] = bestA; stable = false; }
            }
        } while (!stable);
        return new Result(U, policy);
    }
}
