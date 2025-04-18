#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

// structures and some related functions

typedef struct {
    int *literals; 
    int size;       
} Clause;

typedef struct {
    int numVars;
    int numClauses;
    Clause *clauses; 
} CNF;

CNF* createCNF(int numVars, int numClauses) {
    CNF *formula = (CNF*) malloc(sizeof(CNF));
    formula->numVars = numVars;
    formula->numClauses = numClauses;
    formula->clauses = (Clause*) malloc(sizeof(Clause) * numClauses);
    return formula;
}

void freeCNF(CNF *formula) {
    if (!formula) return;
    for (int i = 0; i < formula->numClauses; i++) {
        free(formula->clauses[i].literals);
    }
    free(formula->clauses);
    free(formula);
}

int* copyAssignment(int *assignment, int numVars) {
    int *newAssign = (int*) malloc((numVars+1) * sizeof(int));
    for (int i = 0; i <= numVars; i++) {
        newAssign[i] = assignment[i];
    }
    return newAssign;
}

void printAssignment(int *assignment, int numVars) {
    printf("Assignment: ");
    for (int i = 1; i <= numVars; i++) {
        if (assignment[i] == -1)
            printf("x%d=UN ", i);
        else
            printf("x%d=%d ", i, assignment[i]);
    }
    printf("\n");
}

bool isClauseSatisfied(Clause *clause, int *assignment) {
    for (int i = 0; i < clause->size; i++) {
        int literal = clause->literals[i];
        int var = abs(literal);
        int value = assignment[var];
        if (value != -1) {
            if ((literal > 0 && value == 1) || (literal < 0 && value == 0))
                return true;
        }
    }
    return false;
}

int countUnassigned(Clause *clause, int *assignment, int *unitLiteral) {
    int count = 0;
    *unitLiteral = 0;
    for (int i = 0; i < clause->size; i++) {
        int literal = clause->literals[i];
        int var = abs(literal);
        if (assignment[var] == -1) {
            count++;
            *unitLiteral = literal;
        }
    }
    return count;
}
 
bool isClauseFalse(Clause *clause, int *assignment) {
    bool hasUnassigned = false;
    for (int i = 0; i < clause->size; i++) {
        int literal = clause->literals[i];
        int var = abs(literal);
        int value = assignment[var];
        if (value == -1) {
            hasUnassigned = true;
        } else {
            if ((literal > 0 && value == 1) || (literal < 0 && value == 0))
                return false; 
        }
    }
    return !hasUnassigned;
}

int evaluateFitness(CNF *formula, int *assignment) {
    int count = 0;
    for (int i = 0; i < formula->numClauses; i++) {
        if (isClauseSatisfied(&formula->clauses[i], assignment))
            count++;
    }
    return count;
}

bool allClausesSatisfied(CNF *formula, int *assignment) {
    for (int i = 0; i < formula->numClauses; i++) {
        if (!isClauseSatisfied(&formula->clauses[i], assignment))
            return false;
    }
    return true;
}

// Alphadoku Translation
#define SIZE 25       
#define LETTERS 25     
#define TOTALVARS (SIZE * SIZE * LETTERS)
#define VAR_INDEX(r, c, l) (((r) - 1) * SIZE * LETTERS + ((c) - 1) * LETTERS + (l))

void readAlphadokuPuzzle(const char *filename, char grid[SIZE][SIZE]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening puzzle file");
        exit(1);
    }
    for (int i = 0; i < SIZE; i++) {
        int j = 0;
        char c;
        while (j < SIZE && (c = fgetc(fp)) != EOF) {
            if (!isspace(c)) {
                grid[i][j++] = c;
            }
        }
        if (j != SIZE) {
            fprintf(stderr, "Error: row %d does not contain %d characters.\n", i+1, SIZE);
            exit(1);
        }
    }
    fclose(fp);
}

void writeClause(FILE *fp, int *literals, int count, int *clauseCount) {
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d ", literals[i]);
    }
    fprintf(fp, "0\n");
    (*clauseCount)++;
}

void generateSATfromPuzzle(char grid[SIZE][SIZE], const char *satFilename) {
    FILE *fp = fopen(satFilename, "w+");
    if (!fp) {
        perror("Error opening CNF file for writing");
        exit(1);
    }

    int clauseCount = 0; 
    fprintf(fp, "p cnf %d %d\n", TOTALVARS, 0);

    int lits[LETTERS];
    for (int r = 1; r <= SIZE; r++) {
        for (int c = 1; c <= SIZE; c++) {
            for (int l = 1; l <= LETTERS; l++) {
                lits[l - 1] = VAR_INDEX(r, c, l);
            }
            writeClause(fp, lits, LETTERS, &clauseCount);
            for (int l1 = 1; l1 <= LETTERS; l1++) {
                for (int l2 = l1 + 1; l2 <= LETTERS; l2++) {
                    int pair[2];
                    pair[0] = -VAR_INDEX(r, c, l1);
                    pair[1] = -VAR_INDEX(r, c, l2);
                    writeClause(fp, pair, 2, &clauseCount);
                }
            }
        }
    }

    int cells[SIZE];
    for (int r = 1; r <= SIZE; r++) {
        for (int l = 1; l <= LETTERS; l++) {
            for (int c = 1; c <= SIZE; c++) {
                cells[c - 1] = VAR_INDEX(r, c, l);
            }
            writeClause(fp, cells, SIZE, &clauseCount);
            for (int c1 = 1; c1 <= SIZE; c1++) {
                for (int c2 = c1 + 1; c2 <= SIZE; c2++) {
                    int rowPair[2];
                    rowPair[0] = -VAR_INDEX(r, c1, l);
                    rowPair[1] = -VAR_INDEX(r, c2, l);
                    writeClause(fp, rowPair, 2, &clauseCount);
                }
            }
        }
    }

    for (int c = 1; c <= SIZE; c++) {
        for (int l = 1; l <= LETTERS; l++) {
            for (int r = 1; r <= SIZE; r++) {
                cells[r - 1] = VAR_INDEX(r, c, l);
            }
            writeClause(fp, cells, SIZE, &clauseCount);
            for (int r1 = 1; r1 <= SIZE; r1++) {
                for (int r2 = r1 + 1; r2 <= SIZE; r2++) {
                    int colPair[2];
                    colPair[0] = -VAR_INDEX(r1, c, l);
                    colPair[1] = -VAR_INDEX(r2, c, l);
                    writeClause(fp, colPair, 2, &clauseCount);
                }
            }
        }
    }
    int boxCells[SIZE]; 
    for (int boxRow = 0; boxRow < 5; boxRow++) {
        for (int boxCol = 0; boxCol < 5; boxCol++) {
            for (int l = 1; l <= LETTERS; l++) {
                int idx = 0;
                for (int i = 1; i <= 5; i++) {
                    for (int j = 1; j <= 5; j++) {
                        int r = boxRow * 5 + i;
                        int c = boxCol * 5 + j;
                        boxCells[idx++] = VAR_INDEX(r, c, l);
                    }
                }
                writeClause(fp, boxCells, SIZE, &clauseCount);
                for (int i = 0; i < SIZE; i++) {
                    for (int j = i + 1; j < SIZE; j++) {
                        int boxPair[2];
                        boxPair[0] = -boxCells[i];
                        boxPair[1] = -boxCells[j];
                        writeClause(fp, boxPair, 2, &clauseCount);
                    }
                }
            }
        }
    }

    for (int r = 1; r <= SIZE; r++) {
        for (int c = 1; c <= SIZE; c++) {
            char ch = grid[r - 1][c - 1];
            if (ch != '_') { 
                int l = ch - 'A' + 1;
                int literal = VAR_INDEX(r, c, l);
                writeClause(fp, &literal, 1, &clauseCount);
            }
        }
    }
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "p cnf %d %d\n", TOTALVARS, clauseCount);
    fclose(fp);
    printf("DIMACS file generated: %s\nTotal variables: %d, Total clauses: %d\n", satFilename, TOTALVARS, clauseCount);
}

void printAlphadokuGrid(char grid[SIZE][SIZE]) {
    int row, col;
    for (row = 0; row < SIZE; row++) {
        if (row % 5 == 0) {
            for (col = 0; col < SIZE; col++) {
                if (col % 5 == 0)
                    printf("+---");
                else
                    printf("---");
            }
            printf("+\n");
        }
        
        for (col = 0; col < SIZE; col++) {
            if (col % 5 == 0)
                printf("| ");
            printf("%c ", grid[row][col]);
        }
        printf("|\n");
    }
    for (col = 0; col < SIZE; col++) {
        if (col % 5 == 0)
            printf("+---");
        else
            printf("---");
    }
    printf("+\n");
}
CNF* readCNF(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening CNF file");
        exit(1);
    }
    
    char line[256];
    int numVars = 0, numClauses = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == 'c') continue;
        if (line[0] == 'p') {
            if (sscanf(line, "p cnf %d %d", &numVars, &numClauses) != 2) {
                fprintf(stderr, "Error reading header line\n");
                exit(1);
            }
            break;
        }
    }
    CNF *formula = createCNF(numVars, numClauses);
    
    int clauseIndex = 0;
    while (fgets(line, sizeof(line), fp) && clauseIndex < numClauses) {
        if (line[0]=='c' || line[0]=='\n') continue; 
        int tmp, count = 0;
        char *ptr = line;
        while (sscanf(ptr, "%d", &tmp) == 1) {
            if (tmp == 0) break;
            count++;
            while (*ptr && *ptr != ' ' && *ptr != '\t') ptr++;
            while (*ptr && (*ptr == ' ' || *ptr == '\t')) ptr++;
        }
        if (count == 0) continue;

        formula->clauses[clauseIndex].size = count;
        formula->clauses[clauseIndex].literals = (int*) malloc(count * sizeof(int));
        ptr = line;
        int litIndex = 0;
        while (sscanf(ptr, "%d", &tmp) == 1) {
            if (tmp == 0) break;
            formula->clauses[clauseIndex].literals[litIndex++] = tmp;
            while (*ptr && *ptr != ' ' && *ptr != '\t') ptr++;
            while (*ptr && (*ptr == ' ' || *ptr == '\t')) ptr++;
        }
        clauseIndex++;
    }
    fclose(fp);
    return formula;
}
// dpll algo and helper functions
bool unitPropagation(CNF *formula, int *assignment, bool *conflict);
bool pureLiteralElimination(CNF *formula, int *assignment);

bool DPLL(CNF *formula, int *assignment) {
    bool conflict = false;
    while (unitPropagation(formula, assignment, &conflict) || pureLiteralElimination(formula, assignment)) {
        if (conflict) return false;
    }
    if (allClausesSatisfied(formula, assignment))
        return true;
    for (int i = 0; i < formula->numClauses; i++) {
        if (isClauseFalse(&formula->clauses[i], assignment))
            return false;
    }
    int var = 0;
    for (int i = 1; i <= formula->numVars; i++) {
        if (assignment[i] == -1) {
            var = i;
            break;
        }
    }
    if (var == 0) return false;
    
    int *assignmentCopy = copyAssignment(assignment, formula->numVars);
    assignment[var] = 1;
    if (DPLL(formula, assignment)) {
        free(assignmentCopy);
        return true;
    }
    memcpy(assignment, assignmentCopy, (formula->numVars+1)*sizeof(int));
    assignment[var] = 0;
    if (DPLL(formula, assignment)) {
        free(assignmentCopy);
        return true;
    }
    free(assignmentCopy);
    assignment[var] = -1; 
    return false;
}

bool unitPropagation(CNF *formula, int *assignment, bool *conflict) {
    bool changed = false;
    for (int i = 0; i < formula->numClauses; i++) {
        Clause *clause = &formula->clauses[i];
        if (isClauseSatisfied(clause, assignment))
            continue;
        int unitLiteral = 0;
        int unassignedCount = countUnassigned(clause, assignment, &unitLiteral);
        if (unassignedCount == 0) {
            *conflict = true;
            return false;
        }
        if (unassignedCount == 1) {
            int var = abs(unitLiteral);
            int required = (unitLiteral > 0) ? 1 : 0;
            if (assignment[var] == -1) {
                assignment[var] = required;
                changed = true;
            }
        }
    }
    return changed;
}

bool pureLiteralElimination(CNF *formula, int *assignment) {
    bool changed = false;
    for (int var = 1; var <= formula->numVars; var++) {
        if (assignment[var] != -1)
            continue;
        bool hasPos = false, hasNeg = false;
        for (int i = 0; i < formula->numClauses; i++) {
            Clause *clause = &formula->clauses[i];
            if (isClauseSatisfied(clause, assignment))
                continue;
            for (int j = 0; j < clause->size; j++) {
                if (abs(clause->literals[j]) == var) {
                    if (clause->literals[j] > 0)
                        hasPos = true;
                    else
                        hasNeg = true;
                }
            }
        }
        if (hasPos != hasNeg) {
            assignment[var] = hasPos ? 1 : 0;
            changed = true;
        }
    }
    return changed;
}
// walksat stuff
#define MAX_FLIPS 100000
#define PROBABILITY 0.5

int* WalkSAT(CNF *formula) {
    int numVars = formula->numVars;
    int *assignment = (int*) malloc((numVars+1) * sizeof(int));
    for (int i = 1; i <= numVars; i++) {
        assignment[i] = rand() % 2;
    }
    
    for (int flip = 0; flip < MAX_FLIPS; flip++) {
        if (allClausesSatisfied(formula, assignment))
            return assignment;
        int unsatIndices[formula->numClauses];
        int unsatCount = 0;
        for (int i = 0; i < formula->numClauses; i++) {
            if (!isClauseSatisfied(&formula->clauses[i], assignment)) {
                unsatIndices[unsatCount++] = i;
            }
        }
        if (unsatCount == 0) break;
        int clauseIndex = unsatIndices[rand() % unsatCount];
        Clause *clause = &formula->clauses[clauseIndex];
        
        if (((double)rand() / RAND_MAX) < PROBABILITY) {
            int literal = clause->literals[rand() % clause->size];
            int var = abs(literal);
            assignment[var] = 1 - assignment[var];
        } else {
            int bestVar = 0;
            int bestFitness = -1;
            for (int j = 0; j < clause->size; j++) {
                int var = abs(clause->literals[j]);
                assignment[var] = 1 - assignment[var];
                int fitness = evaluateFitness(formula, assignment);
                assignment[var] = 1 - assignment[var];
                if (fitness > bestFitness) {
                    bestFitness = fitness;
                    bestVar = var;
                }
            }
            if (bestVar != 0) {
                assignment[bestVar] = 1 - assignment[bestVar];
            }
        }
    }
    return assignment;
}
//hill climbing stuff
#define HC_MAX_ITER 10000
#define HC_RESTARTS 10
int* hillClimbing(CNF *formula) {
    int numVars = formula->numVars;
    int bestFitnessOverall = -1;
    int *bestAssignmentOverall = NULL;
    
    for (int restart = 0; restart < HC_RESTARTS; restart++) {
        int *assignment = (int*) malloc((numVars+1) * sizeof(int));
        for (int i = 1; i <= numVars; i++) {
            assignment[i] = rand() % 2;
        }
        int currentFitness = evaluateFitness(formula, assignment);
        bool improved = true;
        int iter = 0;
        while (improved && iter < HC_MAX_ITER) {
            improved = false;
            for (int var = 1; var <= numVars; var++) {
                assignment[var] = 1 - assignment[var];
                int newFitness = evaluateFitness(formula, assignment);
                if (newFitness > currentFitness) {
                    currentFitness = newFitness;
                    improved = true;
                    break;
                } else {
                    assignment[var] = 1 - assignment[var];
                }
            }
            iter++;
            if (currentFitness == formula->numClauses && allClausesSatisfied(formula, assignment)) {
                return assignment;
            }
        }
        if (currentFitness > bestFitnessOverall) {
            if (bestAssignmentOverall != NULL)
                free(bestAssignmentOverall);
            bestFitnessOverall = currentFitness;
            bestAssignmentOverall = copyAssignment(assignment, numVars);
        }
        free(assignment);
    }
    return bestAssignmentOverall;
}


//formatting helper functions
void convertAssignmentToGrid(int *assignment, char grid[SIZE][SIZE]) {
    for (int r = 1; r <= SIZE; r++){
        for (int c = 1; c <= SIZE; c++){
            char letter = '?';
            for (int l = 1; l <= LETTERS; l++){
                if (assignment[VAR_INDEX(r, c, l)] == 1){
                    letter = 'A' + l - 1;
                    break;
                }
            }
            grid[r-1][c-1] = letter;
        }
    }
}
CNF* copyCNFFormula(CNF* formula) {
    CNF* newFormula = createCNF(formula->numVars, formula->numClauses);
    for (int i = 0; i < formula->numClauses; i++) {
        newFormula->clauses[i].size = formula->clauses[i].size;
        newFormula->clauses[i].literals = (int*) malloc(newFormula->clauses[i].size * sizeof(int));
        memcpy(newFormula->clauses[i].literals, formula->clauses[i].literals, newFormula->clauses[i].size * sizeof(int));
    }
    return newFormula;
}
// some logic for making a cnf out of an alphadoku puzzle.
CNF* addBlockingClause(CNF *formula, int *assignment) {
    int newClauseCount = formula->numClauses + 1;
    CNF *newFormula = (CNF*) malloc(sizeof(CNF));
    newFormula->numVars = formula->numVars;
    newFormula->numClauses = newClauseCount;
    newFormula->clauses = (Clause*) malloc(newClauseCount * sizeof(Clause));
    for (int i = 0; i < formula->numClauses; i++) {
        newFormula->clauses[i].size = formula->clauses[i].size;
        newFormula->clauses[i].literals = (int*) malloc(newFormula->clauses[i].size * sizeof(int));
        memcpy(newFormula->clauses[i].literals, formula->clauses[i].literals, newFormula->clauses[i].size * sizeof(int));
    }
    int blockSize = SIZE * SIZE;
    newFormula->clauses[newClauseCount - 1].size = blockSize;
    newFormula->clauses[newClauseCount - 1].literals = (int*) malloc(blockSize * sizeof(int));
    int index = 0;
    for (int r = 1; r <= SIZE; r++){
        for (int c = 1; c <= SIZE; c++){
            int found = 0;
            for (int l = 1; l <= LETTERS; l++){
                if (assignment[VAR_INDEX(r, c, l)] == 1) {
                    newFormula->clauses[newClauseCount - 1].literals[index++] = -VAR_INDEX(r, c, l);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                newFormula->clauses[newClauseCount - 1].literals[index++] = 0;
            }
        }
    }
    return newFormula;
}

// main/ user info
void usage(const char *progname) {
    printf("Usage: %s <algorithm> <alphadoku_puzzle_file>\n", progname);
    printf("    algorithm: dpll | walksat | hill\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        usage(argv[0]);
        return 1;
    }
    const char *algorithm = argv[1];
    const char *puzzleFile = argv[2];

    srand((unsigned int) time(NULL));

    // start out by reading the txt file with the puzzle
    char grid[SIZE][SIZE];
    readAlphadokuPuzzle(puzzleFile, grid);
    printf("Alphadoku puzzle loaded from %s:\n", puzzleFile);
    printAlphadokuGrid(grid);

    // make the cnf file from the data in the txt file
    const char *satFilename = "alphadoku.cnf";
    generateSATfromPuzzle(grid, satFilename);

    // interpret the cnf data
    CNF *formula = readCNF(satFilename);
    printf("CNF formula loaded: %d variables, %d clauses\n", formula->numVars, formula->numClauses);

    clock_t start = clock();
    int *assignment = NULL;
    bool solved = false;
    if (strcmp(algorithm, "dpll") == 0) {
        assignment = (int*) malloc((formula->numVars+1)* sizeof(int));
        for (int i = 0; i <= formula->numVars; i++) {
            assignment[i] = -1;
        }
        if (DPLL(formula, assignment)) {
            printf("DPLL found a satisfying assignment.\n");
            solved = true;
        } else {
            printf("DPLL determined the formula is unsatisfiable.\n");
        }
    } else if (strcmp(algorithm, "walksat") == 0) {
        assignment = WalkSAT(formula);
        if (allClausesSatisfied(formula, assignment)) {
            printf("WalkSAT found a satisfying assignment.\n");
            solved = true;
        } else {
            printf("WalkSAT did not find a complete solution within MAX_FLIPS.\n");
        }
    } else if (strcmp(algorithm, "hill") == 0) {
        assignment = hillClimbing(formula);
        if (allClausesSatisfied(formula, assignment)) {
            printf("Hill Climbing found a satisfying assignment.\n");
            solved = true;
        } else {
            printf("Hill Climbing ended at a local maximum. Best fitness = %d / %d clauses\n",
                   evaluateFitness(formula, assignment), formula->numClauses);
        }
    } else {
        printf("Unknown algorithm: %s\n", algorithm);
        usage(argv[0]);
        freeCNF(formula);
        return 1;
    }
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);

    if (!solved) {
        printf("No solution\n");
        freeCNF(formula);
        free(assignment);
        return 0;
    }

    // make our solution into a grid to be printed
    char solvedGrid[SIZE][SIZE];
    convertAssignmentToGrid(assignment, solvedGrid);
    printf("Solution:\n");
    printAlphadokuGrid(solvedGrid);

    // check if solution is unique
    CNF *formulaWithBlock = addBlockingClause(formula, assignment);
    int *assignment2 = NULL;
    bool additionalSolution = false;
    if (strcmp(algorithm, "dpll") == 0) {
        assignment2 = (int*) malloc((formulaWithBlock->numVars+1)* sizeof(int));
        for (int i = 0; i <= formulaWithBlock->numVars; i++) {
            assignment2[i] = -1;
        }
        if (DPLL(formulaWithBlock, assignment2)) {
            additionalSolution = true;
        }
    } else if (strcmp(algorithm, "walksat") == 0) {
        assignment2 = WalkSAT(formulaWithBlock);
        if (allClausesSatisfied(formulaWithBlock, assignment2))
            additionalSolution = true;
    } else if (strcmp(algorithm, "hill") == 0) {
        assignment2 = hillClimbing(formulaWithBlock);
        if (allClausesSatisfied(formulaWithBlock, assignment2))
            additionalSolution = true;
    }

    if (additionalSolution) {
        char altGrid[SIZE][SIZE];
        convertAssignmentToGrid(assignment2, altGrid);
        printf("Additional solution found:\n");
        printAlphadokuGrid(altGrid);
    } else {
        printf("Solution is unique\n");
    }

    // final clean up
    free(assignment);
    free(assignment2);
    freeCNF(formula);
    freeCNF(formulaWithBlock);
    
    return 0;
}
