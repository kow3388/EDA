# Complexity

根據algorithm的input size來預估執行此algorithm所需的time和space

## Overview

Time complexity: 這個algorithm執行所需的computational steps

Space complexity: 這個algorithm執行所需要的memory locations

## Complexity Measures

### Time Complexity

這個algorithm執行所需的computational steps

### Space Complexity

這個algorithm執行所需要的memory locations

## Asymptotic Notation

大多數在表示algorithm都是用Big O來表示 (因為我們比較care algorithm最差的情況)

### Big O

Big O是upper bound function

數學定義是當n很大時，會使得0 <= f(n) <= cg(n)，則f(n)是O(g(n))

白話來說就是f(n)最差最差情況下會是g(n) (worst case)

## P, NP, NP-complete & NP-hard

### P

P: 是指可以在polynomial time解決的問題

Polynomial time: 是指時間是在n的次方內的時間，例如O(n), O($n^{2}$), O(nlogn), ...

而O($2^{n}$)就不是polnomial time

通常polynomial time的problem都是被認為是有效解決的

### NP

NP: Nondeterministic Polynomial time

是指可以在polynomial time時間內驗證它的正確性

### NP-complete

問題本身是一個NP的問題，所有NP問題都可以在polynomial time "reduce"成此問題，則此問題是NP-complete

白話問意思是這個問題是NP裡最難的問題之一

Reduce的意思是說

假設有兩個decision problems L1 & L2，已知algorithm A2可以解L2，那A2是否可以用來解L1?

如果今天可以將input of L1透過一個polynomial time function F，將input轉換成L2的input

也就是F(input1) = input2

則可以說L2 at least as hard as L1

### NP-hard

此問題至少和NP-complete一樣難，且不一定屬於NP (不一定能被快速驗證)
