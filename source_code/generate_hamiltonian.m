(* ::Package:: *)

var = ToExpression[$ScriptCommandLine[[2;;4]]];

indepent[m_,n_,N_]:=If[N>=n,Total[Expand[Times@@(1-a@@@Subsets[Array[c,n],{2}])]/.(Thread[Array[c,n]->#]&/@Subsets[Range[N],{n}])],0];
cluster[m_,n_,N_]:=If[N>=m,Total[Expand[Times@@(a@@@Subsets[Array[c,m],{2}])]/.(Thread[Array[c,m]->#]&/@Subsets[Range[N],{m}])],0];
h[m_,n_,N_]:=indepent[m,n,N]+cluster[m,n,N];

hamiltonian=h@@var;

variables=Variables@hamiltonian;
terms=List@@hamiltonian;
coefficients=terms/.Thread[variables->1];
variablesInTerm=Variables/@terms/.Thread[variables->Range@Length@variables];
coefficientMatrix=Join[variablesInTerm,{coefficients}\[Transpose],2];

fname = "hamiltonians/H_"<>ToString@var[[1]]<>"_"<>ToString@var[[2]]<>"_"<>ToString@var[[3]]<>".txt";
s = OpenWrite[fname];
WriteLine[s,StringJoin@Riffle[ToString/@#," "]]&/@coefficientMatrix;
WriteLine[s,"#"];
Close[s];
Print[fname];
