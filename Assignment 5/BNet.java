import java.util.*;

public class BNet {

    static class Variable {
        String name;
        List<String> parents;
        Map<Map<String, Boolean>, Double> cpt = new HashMap<>();
        Variable(String name, List<String> parents) {
            this.name = name;
            this.parents = parents;
        }
        double getProbability(boolean value, Map<String, Boolean> evidence) {
            Map<String, Boolean> key = new HashMap<>();
            for (String p : parents) key.put(p, evidence.get(p));
            double pTrue = cpt.get(key);
            return value ? pTrue : 1.0 - pTrue;
        }
    }

    List<Variable> vars = new ArrayList<>();

    public BNet() {
        Variable B = new Variable("B", List.of());
        B.cpt.put(Map.of(), 0.02);

        Variable E = new Variable("E", List.of());
        E.cpt.put(Map.of(), 0.03);

        Variable A = new Variable("A", List.of("B","E"));
        A.cpt.put(Map.of("B", true,  "E", true),  0.97);
        A.cpt.put(Map.of("B", true,  "E", false), 0.92);
        A.cpt.put(Map.of("B", false, "E", true),  0.36);
        A.cpt.put(Map.of("B", false, "E", false), 0.03);

        Variable J = new Variable("J", List.of("A"));
        J.cpt.put(Map.of("A", true),  0.85);
        J.cpt.put(Map.of("A", false), 0.07);

        Variable M = new Variable("M", List.of("A"));
        M.cpt.put(Map.of("A", true),  0.69);
        M.cpt.put(Map.of("A", false), 0.02);

        vars.add(B); vars.add(E); vars.add(A); vars.add(J); vars.add(M);
    }

    public double jointExact(Map<String, Boolean> query, Map<String, Boolean> evidence) {
        Map<String, Boolean> both = new HashMap<>(evidence);
        both.putAll(query);
        double num = enumerateAll(vars, both);
        if (evidence.isEmpty()) return num;
        double denom = enumerateAll(vars, evidence);
        return num / denom;
    }

    public double jointApprox(Map<String, Boolean> query, Map<String, Boolean> evidence, int N) {
        int countE = 0, countQandE = 0;
        Random rnd = new Random();
        for (int i = 0; i < N; i++) {
            Map<String, Boolean> sample = priorSample(rnd);
            if (consistent(sample, evidence)) {
                countE++;
                if (consistent(sample, query)) countQandE++;
            }
        }
        return countE == 0 ? 0.0 : (double) countQandE / countE;
    }

    private double enumerateAll(List<Variable> list, Map<String, Boolean> evidence) {
        if (list.isEmpty()) return 1.0;
        Variable Y = list.get(0);
        List<Variable> rest = list.subList(1, list.size());
        if (evidence.containsKey(Y.name)) {
            return Y.getProbability(evidence.get(Y.name), evidence)
                 * enumerateAll(rest, evidence);
        } else {
            double sum = 0;
            for (boolean val : List.of(true, false)) {
                Map<String, Boolean> ext = new HashMap<>(evidence);
                ext.put(Y.name, val);
                sum += Y.getProbability(val, evidence) * enumerateAll(rest, ext);
            }
            return sum;
        }
    }

    private Map<String, Boolean> priorSample(Random rnd) {
        Map<String, Boolean> sample = new HashMap<>();
        for (Variable v : vars) {
            sample.put(v.name, rnd.nextDouble() < v.getProbability(true, sample));
        }
        return sample;
    }

    private boolean consistent(Map<String, Boolean> sample, Map<String, Boolean> clamp) {
        for (var e : clamp.entrySet()) {
            if (!sample.get(e.getKey()).equals(e.getValue())) return false;
        }
        return true;
    }

    public static void main(String[] args) {
        if (args.length < 1) {
            System.err.println("Usage: java BNet Q1 [Q2 ...] [given E1 ...]");
            System.exit(1);
        }

        Map<String, Boolean> query    = new LinkedHashMap<>();
        Map<String, Boolean> evidence = new LinkedHashMap<>();
        boolean seenGiven = false;

        for (String tok : args) {
            if (tok.equalsIgnoreCase("given")) {
                seenGiven = true;
                continue;
            }
            String var = tok.substring(0, 1);
            boolean val = tok.charAt(1) == 't';
            String name = switch (var) {
                case "B" -> "B";
                case "E" -> "E";
                case "A" -> "A";
                case "J" -> "J";
                case "M" -> "M";
                default  -> throw new IllegalArgumentException("Unknown var " + var);
            };
            if (!seenGiven) query.put(name, val);
            else           evidence.put(name, val);
        }

        BNet bn = new BNet();

        double exact  = bn.jointExact(query, evidence);
        double sample = bn.jointApprox(query, evidence, 10000);

        System.out.printf("Exact P(%s | %s) = %.6f%n",
                          format(query), format(evidence), exact);
        System.out.printf("Approx P(%s | %s) ≈ %.6f%n",
                          format(query), format(evidence), sample);
    }

    private static String format(Map<String, Boolean> m) {
        if (m.isEmpty()) return "{}";
        StringBuilder sb = new StringBuilder("{");
        boolean first = true;
        for (var e : m.entrySet()) {
            if (!first) sb.append(", ");
            sb.append(e.getKey()).append("=").append(e.getValue()?"t":"f");
            first = false;
        }
        return sb.append("}").toString();
    }
}
