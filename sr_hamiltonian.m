(* ::Package:: *)

var = ToExpression[$ScriptCommandLine[[2;;4]]];

r[m_,n_]:=\[Infinity];
r[1,n_]:=1;
r[m_,1]:=1;
r[2,n_]:=n;
r[m_,2]:=m;
r[3,3]=6;
r[3,4]=r[4,3]=9;
r[3,5]=r[5,3]=14;
r[3,6]=r[6,3]=18;
r[3,7]=r[7,3]=23;
r[3,8]=r[8,3]=28;
r[3,9]=r[9,3]=36;
r[4,4]=18;
r[4,5]=r[5,4]=25;

partitionAt[list_,p_]:=Part[list,#]&/@Span@@@({1,0}+#&/@Partition[{0}~Join~p,2,1])
makeRules[list_]:=Module[{signs=(Sign@list+1)/2},Flatten[Thread/@Thread[a@@@Subsets[#,{2}]&/@partitionAt[Range@Total@Abs@list,Accumulate@Abs@list]->signs]]]
findNext[m_,n_,N_]:=Part[Flatten[Delete[Reverse/@Reverse@Table[{i,j},{i,2,m},{j,2,Min[n,i]}],{1,1}],1],FirstPosition[Flatten[Delete[Reverse/@Reverse@Table[r[i,j]<=N,{i,2,m},{j,2,Min[n,i]}],{1,1}],1],True]][[1]]
split[m_,n_,N_]:=Module[{stack={{}},tmp,left,right,splits={}},
	While[
		Length@stack>0,
		tmp=Last@stack;
		stack=stack[[;;-2]];
		If[
			Abs[Total@Abs@tmp-N]<=2,
			AppendTo[splits,tmp],
			{left,right}={1,-1}*findNext[m,n,N-Total@Abs@tmp];
			stack=stack~Join~{Append[tmp,left],Append[tmp,right]}
		]
	];
	Select[splits,!MemberQ[#,m]&]
]

indepent[m_,n_,N_]:=If[N>=n,Total[Expand[Times@@(1-a@@@Subsets[Array[c,n],{2}])]/.(Thread[Array[c,n]->#]&/@Subsets[Range[N],{n}])],0];
cluster[m_,n_,N_]:=If[N>=m,Total[Expand[Times@@(a@@@Subsets[Array[c,m],{2}])]/.(Thread[Array[c,m]->#]&/@Subsets[Range[N],{m}])],0];
h[m_,n_,N_]:=indepent[m,n,N]+cluster[m,n,N];

originalHamiltonian=h@@var;
hamiltonians=originalHamiltonian/.makeRules/@split@@var;
numberOfSplits=Length@hamiltonians;

variables=Variables@originalHamiltonian;
terms=List@@#&/@hamiltonians;
coefficients=terms/.Thread[variables->1];
variablesInTerm=Variables/@#&/@terms/.Thread[variables->Range@Length@variables];
coefficientMatrix=Join[variablesInTerm[[#]],{coefficients[[#]]}\[Transpose],2]&/@Range@numberOfSplits;

fname = "hamiltonians/SR_H_"<>ToString@var[[1]]<>"_"<>ToString@var[[2]]<>"_"<>ToString@var[[3]]<>"_"<>ToString@numberOfSplits<>".txt";
s = OpenWrite[fname];
Do[WriteLine[s,StringJoin[Riffle[ToString/@#," "]]]&/@coefficientMatrix[[a]];WriteLine[s,"#"],{a,numberOfSplits}];
Close[s];
Print[fname];
