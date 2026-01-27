$$
\begin{align}
[\text{Prog}] &\to [\text{Stmt}]^* \\
[\text{Stmt}] &\to 
    \begin{cases}
        exit(\text{expr});\\
        let\space\text{ident}=[\text{expr}];
    \end{cases} \\
[\text{expr}] &\to 
    \begin{cases}
        [\text{Term}] \\
        [\text{BinExpr}]
    \end{cases} \\
    [\text{BinExpr}] &\to
    \begin{cases}
        [\text{expr}] * [\text{expr}] & \text{prec}=1 \\
        [\text{expr}] / [\text{expr}] & \text{prec}=1 \\
        [\text{expr}] - [\text{expr}] & \text{prec}=0 \\
        [\text{expr}] + [\text{expr}] & \text{prec}=0 \\
    \end{cases}
    [text{Term}] &\to
    \begin{cases}
        \text{int_lit} \\
        \text{ident} \\
        ([\text{expr}])
    \end{cases}
\end{align}
$$