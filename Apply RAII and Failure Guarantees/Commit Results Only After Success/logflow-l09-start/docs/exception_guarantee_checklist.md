# Exception-Guarantee Checklist

## The three levels

| Level | Promise when something fails |
|---|---|
| Basic | nothing leaks and nothing is corrupt, but the state may have changed |
| Strong | the state is exactly what it was before the call, or the call fully succeeded |
| No-throw | the operation cannot fail |

## Making an update strong

1. Name the shared state the function changes. In `ingest_file` it is `totals`.
2. Do all the work that can fail on a local copy. Reading, parsing and counting go into `local`.
3. Commit at the very end with operations that cannot fail. Adding numbers to counters cannot throw.
4. Make sure nothing that can fail runs after the commit starts.
5. Test every failure path: copy the state before, trigger the failure, compare after.

## Fill in for `ingest_file` (before the change)

| Question | Your answer |
|---|---|
| What shared state does it change? | |
| Where does it change that state? | |
| Which line can throw while the state is half-changed? | |
| What does a rejected file leave behind? | |

## Answer key

| Question | Answer |
|---|---|
| Shared state | the `RunTotals` passed in by reference |
| Where it changes | inside the loop, one record at a time |
| Line that can throw | the `throw` for a malformed line, after earlier records were already counted |
| What a rejected file leaves behind | the partial counts of every record before the bad line |

## After the change

| Step | What happens | Can it throw? |
|---|---|---|
| Read and parse each line | counts go into `local` | yes, and `totals` is untouched |
| Loop ends | every line was valid | no |
| Commit | four additions into `totals` | no |
