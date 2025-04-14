#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

// neccesary structures 

typedef struct {
    int *literals;  
    int size;       
} Clause;

typedef struct {
    int numVars;
    int numClauses;
    Clause *clauses; 
} CNF;

// general/utility functions
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

//going through the clauses
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



// CNF reader and such
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

// DPLL algo

// DPLL functions.
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

// WalkSAT

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
    // No satisfying assignment was found but the best option is still returned.
    return assignment;
}

// Hill Climbing

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

// user/main stuff

void usage(const char *progname) {
    printf("Usage: %s <algorithm> <inputfile>\n", progname);
    printf("    algorithm: dpll | walksat | hill\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        usage(argv[0]);
        return 1;
    }
    const char *algorithm = argv[1];
    const char *filename = argv[2];
    srand((unsigned int) time(NULL));
    CNF *formula = readCNF(filename);
    printf("CNF formula loaded: %d variables, %d clauses\n", formula->numVars, formula->numClauses);
    clock_t start = clock();
    int *assignment = NULL;
    if (strcmp(algorithm, "dpll") == 0) {
        assignment = (int*) malloc((formula->numVars+1)* sizeof(int));
        for (int i = 0; i <= formula->numVars; i++) {
            assignment[i] = -1;
        }
        if (DPLL(formula, assignment)) {
            printf("DPLL found a satisfying assignment.\n");
        } else {
            printf("DPLL determined the formula is unsatisfiable.\n");
        }
    } else if (strcmp(algorithm, "walksat") == 0) {
        assignment = WalkSAT(formula);
        if (allClausesSatisfied(formula, assignment))
            printf("WalkSAT found a satisfying assignment.\n");
        else
            printf("WalkSAT did not find a complete solution within MAX_FLIPS.\n");
    } else if (strcmp(algorithm, "hill") == 0) {
        assignment = hillClimbing(formula);
        if (allClausesSatisfied(formula, assignment))
            printf("Hill Climbing found a satisfying assignment.\n");
        else
            printf("Hill Climbing ended at a local maximum. Best fitness = %d / %d clauses\n",
                   evaluateFitness(formula, assignment), formula->numClauses);
    } else {
        printf("Unknown algorithm: %s\n", algorithm);
        usage(argv[0]);
        freeCNF(formula);
        return 1;
    }
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);
    printAssignment(assignment, formula->numVars);
    free(assignment);
    freeCNF(formula);
    return 0;
}
