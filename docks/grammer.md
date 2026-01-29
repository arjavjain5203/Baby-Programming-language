$$
\begin{align}
[\text{Prog}] &\to [\text{Stmt}]^* \\
[\text{Stmt}] &\to 
    \begin{cases}
        \text{bye}([\text{expr}]);\\
        \text{hope}\space\text{ident}=[\text{expr}]; & \text{// Integer Declaration} \\
        \text{dillusion}\space\text{ident}=[\text{expr}]; & \text{// String Declaration} \\
        \text{ident}=[\text{expr}]; & \text{// Assignment} \\
        \text{maybe}([\text{expr}])[\text{scope}] (\text{ormaybe}([\text{expr}])[\text{scope}])^* (\text{moveon}[\text{scope}])? \\
        \text{wait}([\text{expr}])[\text{scope}] \\
        \text{tell\_me}([\text{expr}]);\\
        \text{then}; & \text{// Newline}\\
        \text{FuncDef} & \text{// Function Definition}
    \end{cases} \\
[\text{scope}] &\to \{[\text{Stmt}]^*\} \\ 
[\text{FuncDef}] &\to (\text{hope}|\text{dillusion})\space\text{ident}( ([\text{Type}] \space \text{ident} (, [\text{Type}] \space \text{ident})*)? ) [\text{scope}] \\
[\text{Type}] &\to \text{hope} | \text{dillusion} \\
[\text{expr}] &\to
    \begin{cases}
        [\text{Term}] \\
        [\text{BinExpr}]
    \end{cases} \\
    [\text{BinExpr}] &\to
    \begin{cases}
        [\text{expr}] * [\text{expr}] & \text{prec}=2 \\
        [\text{expr}] / [\text{expr}] & \text{prec}=2 \\
        [\text{expr}] + [\text{expr}] & \text{prec}=1 \\
        [\text{expr}] - [\text{expr}] & \text{prec}=1 \\
        [\text{expr}] == [\text{expr}] & \text{prec}=0 \\
        [\text{expr}] != [\text{expr}] & \text{prec}=0 \\
        [\text{expr}] < [\text{expr}] & \text{prec}=0 \\
        [\text{expr}] > [\text{expr}] & \text{prec}=0 \\
        [\text{expr}] <= [\text{expr}] & \text{prec}=0 \\
        [\text{expr}] >= [\text{expr}] & \text{prec}=0 \\
    \end{cases} \\
    [\text{Term}] &\to
    \begin{cases}
        \text{int\_lit} \\
        \text{string\_lit} \\
        \text{ident} \\
        \text{ident}( ([\text{expr}] (, [\text{expr}])*)? ) & \text{// Function Call} \\
        ([\text{expr}])
    \end{cases}
\end{align}
$$

**Comments:**
- Multiple lines: `hide ... hide`
- Single line: `secret ...`