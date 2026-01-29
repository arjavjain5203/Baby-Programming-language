$$
\begin{align}
[\text{Prog}] &\to [\text{Stmt}]^* \\
[\text{Stmt}] &\to 
    \begin{cases}
        \text{bye}(\text{expr});\\
        \text{hope}\space\text{ident}=[\text{expr}];\\
        \text{if}([\text{expr}])[\text{scope}]\\
        \text{else if}([\text{expr}])[\text{scope}]\\
        \text{else}[\text{scope}]\\
        \text{while}([\text{expr}])[\text{scope}]\\
        \text{tell_me}([\text{expr}]);\\
    \end{cases} \\
[\text{scope}] &\to \{\text[{Stmt}]^*\} \\ 
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